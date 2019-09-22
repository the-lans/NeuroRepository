#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pf("command")
{
    ui->setupUi(this);
    this->stopapp = false;
    this->tcommand = nullptr;
    this->tcommand_work = false;
    this->name_param = "tradeparam.txt";
    this->dir = "";
    //this->dir = "D:/Ilya/Prog/NeuroProject-3/comitet_class";

    //time_srand();
    //init_random_generator();
    this->dt.setArrayBR(true);
    this->pf.setPath(this->dir);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_dir_clicked()
{
    this->dir = QFileDialog::getExistingDirectory(this,
                               QString::fromUtf8("Открыть папку"),
                               this->dir,
                               QFileDialog::ShowDirsOnly
                               | QFileDialog::DontResolveSymlinks);
    this->pf.setPath(this->dir);
}

void MainWindow::on_pushButton_run_clicked()
{
    vector<string> strvec;
    QString str = "", valNum;
    bool er = false;

    if(this->dir == "") {return;}
    this->activForm(false);

    split(strvec, this->dir.toStdString(), "/");
    valNum = QString::fromStdString(strvec.back()); str += valNum + "\n";
    ui->textBrowser->setText(str);

    //Загрузка параметров системы
    string parent = "";
    this->dt.clear();
    this->dt.read(this->dir.toStdString() + "\\" + this->name_param);
    this->tparam.loadECSV(this->dt, parent);

    //Загрузка входного файла с архивными данными
    parent = "";
    this->dt.clear();
    this->mtrx_bars.clear();
    this->dt.read(this->dir.toStdString() + "\\" + this->tparam.name_inarch);
    this->mtrx_bars.loadECSV(this->dt, parent);
    this->mtrx_bars.reversRow();

    //Проверки
    if(this->tparam.stepExmTime < 1) {ui->textBrowser->append("False stepExmTime!\n"); er = true;}
    if(this->tparam.wind > this->mtrx_bars.getLenRow()) {ui->textBrowser->append("False size: wind > lenRow!\n"); er = true;}
    if(er) {this->activForm(true); return;}

    // Основной цикл
    this->stopapp = false;
    this->tcommand_work = true;
    this->tcommand = new std::thread(&MainWindow::runCommand, this);
}

void MainWindow::on_pushButton_stop_clicked()
{
    this->stopapp = true;
    //while(this->tcommand_work) {QThread::msleep(500);} //Ожидание завершения
    if(this->tcommand != nullptr)
    {
        if(this->tcommand->joinable()) {this->tcommand->join();}
        delete this->tcommand;
        this->tcommand = nullptr;
    }

    this->activForm(true);
}

void MainWindow::activForm(bool bl)
{
    //this->ui->textBrowser->setEnabled(bl);
    this->ui->pushButton_dir->setEnabled(bl);
    this->ui->pushButton_run->setEnabled(bl);
    this->ui->pushButton_stop->setEnabled(!bl);
}

void MainWindow::runCommand()
{
    bool blLoop = true;
    NCommandNet command;
    QString comstr, txtstr;
    float search_time;
    int period_train, numIteration = 0;

    //Начало Pipe
    if(!this->pf.exists()) {this->pf.create();}
    this->pf.open();
    comstr = QString::fromStdString(to_string(NCommandNet::NComBegExtern)); this->pf.writeWBufFormat(comstr);
    if(!this->pf.write()) {ui->textBrowser->append("Error write pipe begin!\n"); this->stopapp = true;}

    //Первоначальные данные для обучения
    this->mtrx_addbars.clear();
    this->mtrx_addbars.resize(this->tparam.wind, this->mtrx_bars.getLenColumn());
    this->dt.create(this->dir.toStdString() + "\\" + this->tparam.name_input);
    this->updateData(this->tparam.wind);

    while(!this->stopapp && numIteration < this->tparam.countIteration)
    {
        blLoop = this->pf.waitForLoop(10);
        if(!blLoop) {continue;}

        if(this->pf.read())
        {
            to_value(command, this->pf.mes.command.toStdString());
            if(command == NCommandNet::NComEndTrain)
            {
                //Замер времени на обучение
                this->timer.stop();
                search_time = this->timer.deltamin();
                period_train = (int)(search_time / (this->tparam.period * this->tparam.timeframe_min)) + 1;
                ui->textBrowser->append("Time train = " + QString::fromStdString(this->timer.toString()) + "\n");

                //Обновление данных
                //this->updateData(period_train);
                this->updateData(this->tparam.stepExmTime);

                //Запуск функционирования
                comstr = QString::fromStdString(to_string(NCommandNet::NComRunFunc)); this->pf.writeWBufFormat(comstr);
                if(!this->pf.write()) {ui->textBrowser->append("Error write pipe: NComRunFunc!\n");}

                //Вывод результата
                ui->textBrowser->append("Run func\n");
                numIteration++;
            }

            else if(command == NCommandNet::NComUpdateOut)
            {
                //Запуск обучения
                this->timer.start();
                comstr = QString::fromStdString(to_string(NCommandNet::NComRunTrain)); this->pf.writeWBufFormat(comstr);
                if(!this->pf.write()) {ui->textBrowser->append("Error write pipe: NComRunTrain!\n");}

                //Вывод результата
                txtstr.setNum(numIteration+1);
                ui->textBrowser->append("#" + txtstr + ". Begin train...");
            }

            else if(command == NCommandNet::NComBeg)
            {
                //Запуск обучения
                this->timer.start();
                comstr = QString::fromStdString(to_string(NCommandNet::NComRunTrain)); this->pf.writeWBufFormat(comstr);
                if(!this->pf.write()) {ui->textBrowser->append("Error write pipe: NComRunTrain!\n");}

                //Вывод результата
                txtstr.setNum(numIteration+1);
                ui->textBrowser->append("#" + txtstr + ". Begin train...");
            }

            else if(command == NCommandNet::NComEnd)
            {
                this->stopapp = true;
            }
        }

        else if(!this->pf.eof())
        {
            ui->textBrowser->append("Error read!\n");
        }
    }

    //Конец Pipe
    ui->textBrowser->append("Finish!");
    comstr = QString::fromStdString(to_string(NCommandNet::NComFinish)); this->pf.writeWBufFormat(comstr);
    if(!this->pf.write()) {ui->textBrowser->append("Error write pipe: NComFinish!\n");}

    comstr = QString::fromStdString(to_string(NCommandNet::NComEndExtern)); this->pf.writeWBufFormat(comstr);
    if(!this->pf.write()) {ui->textBrowser->append("Error write pipe end!\n");}
    this->pf.close();

    this->tcommand_work = false;
    //this->activForm(true);
}

void MainWindow::updateData(int num)
{
    NArray<string> valvec;
    string parent = "";

    this->dt.clear();
    this->mtrx_addbars.clear();
    for(int ind = 0; ind < num; ind++)
    {
        this->mtrx_bars.popRow(valvec);
        this->mtrx_addbars.pushRow(valvec);
    }
    this->mtrx_addbars.saveECSV(this->dt, parent);
    //this->dt.write(this->dir.toStdString() + "\\" + this->tparam.name_input);
    this->dt.appendObj(this->dir.toStdString() + "\\" + this->tparam.name_input);
}
