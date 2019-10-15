#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pf("command")
{
    ui->setupUi(this);
    this->activForm(true);
    this->stopapp = true;
    this->tcommand = nullptr;
    this->tcommand_work = false;

    time_srand();
    init_random_generator();
    this->dt.setArrayBR(true);
    this->dir = "";
    //this->dir = "D:/ProgramProjects/NeuroProject/NeuroProject-3/tradeprofit_10_cartpole/cartpole2";
    //this->dir = "C:/Ilya/Flash/!ProgramProjects/NeuroProject/NeuroProject-3/trade_test";
    //this->dir = "C:/Ilya/Flash/!ProgramProjects/NeuroProject/NeuroProject-3/tradeprofit_10_cartpole/cartpole2";
    this->construct.setCurrentDir(dir.toStdString());
    this->pf.setPath(this->dir);
    this->init_files();
    this->comitet.construct = &this->construct;
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

    this->construct.setCurrentDir(dir.toStdString());
    this->pf.setPath(this->dir);
}

void MainWindow::on_pushButton_train_clicked()
{
    this->run(true);
}

void MainWindow::on_pushButton_func_clicked()
{
    this->run(false);
}


void MainWindow::init_files()
{
    this->construct.name_outdata = "outdata.csv";
    this->construct.name_input = "indata.csv";

    this->construct.fin.name_examples = "in_examples.txt";
    this->construct.fin.name_neuronet = "in_neuronet.txt";
    this->construct.fin.name_trainbp = "in_trainbp.txt";

    this->construct.fout.name_examples = "out_examples.csv";
    this->construct.fout.name_neuronet = "out_neuronet.csv";
    this->construct.fout.name_trainbp = "out_trainbp.csv";
}

bool MainWindow::init_load(int variantZad, int variantTrain, bool isTrain, bool isComitet)
{
    bool isLoad = true;

    //Задача
    if(variantZad == 0) //Общая задача
    {
        //this->construct.zad = new NExamples<NTypeValue>;
        //this->construct.name_input = "indata.csv";
    }
    else if(variantZad == 1) //Задача Trade High/Low
    {
        this->construct.zad = new NTradeTg<NTypeValue>;
        this->construct.name_input = "bars.csv";
    }
    else if(variantZad == 2) //Задача Trade Profit
    {
        this->construct.zad = new NTradeProfit<NTypeValue>;
        this->construct.name_input = "bars.csv";
    }
    else if(variantZad == 3) //Задача Cart Pole
    {
        this->construct.zad = new NCartPole<NTypeValue>;
    }

    //Обучение
    if(variantTrain == 0) //Общее обучение
    {
        this->construct.train_type = NTrainType::NTrainBPType;
    }
    else if(variantTrain == 1) //Обучение агента
    {
        this->construct.train_type = NTrainType::NAgentType;
    }

    this->init_struct(isTrain, isComitet);
    return isLoad;
}

void MainWindow::init_struct(bool isTrain, bool isComitet)
{
    if(isComitet)
    {
        this->comitet.zad = this->construct.zad;
        this->comitet.init_load(this->dir.toStdString(), "in_comitet.txt", isTrain);
    }
    else
    {
        this->construct.init_constructor(isTrain);
    }
}

void MainWindow::run(bool isTrain)
{
    bool isLoad;
    NExample<NTypeValue>* exm;
    string valvec;
    vector<string> strvec;
    QString str = "", valNum;

    if(this->dir == "") {return;}

    split(strvec, this->dir.toStdString(), "/");
    valNum = QString::fromStdString(strvec.back()); str += valNum + "\n\n";

    // Задача
    isLoad = this->init_load(ui->comboZad->currentIndex(), ui->comboTrain->currentIndex(), isTrain, false);
    if(!isLoad) {return;}
    this->construct.zad->prerun();

    //Проверки
    if(!this->construct.net->check())
    {
        valNum.setNum(static_cast<int>(this->construct.net->getCheck()));
        str += "Error code: " + valNum + "\n";
        ui->textBrowser->setText(str);
        return;
    }

    // Обучение
    if(this->construct.zad->getLength() > 0)
    {
        this->construct.net->funcRegularization();
        this->construct.net->runExamples(NSetType::NSetTrain);
        str += "Train\n";
        str += "Energy = "; valNum.setNum(this->construct.zad->getEnergyAver()); str += valNum + "\n";
        str += "EnergySum = "; valNum.setNum(this->construct.zad->getEnergySum()); str += valNum + "\n";
        str += "EnergyMax = "; valNum.setNum(this->construct.zad->getEnergyMax()); str += valNum + "\n";
        str += "EnergyRegularization = "; valNum.setNum(this->construct.net->getEnergyRegularization()); str += valNum + "\n";
        str += "\n";

        if(isTrain)
        {
            this->construct.bp->init_train();
            this->construct.train(); //Процесс обучения
            str += "Time = "; valNum.setNum(this->construct.search_time); str += valNum + "\n";
            str += "Epoches = "; valNum.setNum(this->construct.bp->getEpoches()); str += valNum + "\n";
            str += "IterGame = "; valNum.setNum(this->construct.bp->getIterGame()); str += valNum + "\n";
            str += "StabEnergy = "; valNum.setNum(this->construct.bp->getStabEnergy()); str += valNum + "\n";
            str += "ChangeEnergy = "; valNum.setNum(this->construct.bp->getChangeEnergy()); str += valNum + "\n";
            str += "Energy = "; valNum.setNum(this->construct.bp->geTrain.energyAver); str += valNum + "\n";
            str += "EnergySum = "; valNum.setNum(this->construct.bp->geTrain.energySum); str += valNum + "\n";
            str += "EnergyMax = "; valNum.setNum(this->construct.bp->geTrain.energyMax); str += valNum + "\n";
            str += "EnergyRegularization = "; valNum.setNum(this->construct.net->getEnergyRegularization()); str += valNum + "\n";
            str += "\n";
        }

        str += "Test\n";
        if(isTrain)
        {
            str += "Energy = "; valNum.setNum(this->construct.bp->geTest.energyAver); str += valNum + "\n";
            str += "EnergySum = "; valNum.setNum(this->construct.bp->geTest.energySum); str += valNum + "\n";
            str += "EnergyMax = "; valNum.setNum(this->construct.bp->geTest.energyMax); str += valNum + "\n";
        }
        else
        {
            this->construct.net->runExamples(NSetType::NSetTest);
            str += "Energy = "; valNum.setNum(this->construct.zad->getEnergyAver()); str += valNum + "\n";
            str += "EnergySum = "; valNum.setNum(this->construct.zad->getEnergySum()); str += valNum + "\n";
            str += "EnergyMax = "; valNum.setNum(this->construct.zad->getEnergyMax()); str += valNum + "\n";    
        }
        str += "EnergyRegularization = "; valNum.setNum(this->construct.net->getEnergyRegularization()); str += valNum + "\n";
        str += "\n";

        this->construct.net->runExamples(NSetType::NSetValidate);
        str += "Validate\n";
        str += "Energy = "; valNum.setNum(this->construct.zad->getEnergyAver()); str += valNum + "\n";
        str += "EnergySum = "; valNum.setNum(this->construct.zad->getEnergySum()); str += valNum + "\n";
        str += "EnergyMax = "; valNum.setNum(this->construct.zad->getEnergyMax()); str += valNum + "\n";
        str += "EnergyRegularization = "; valNum.setNum(this->construct.net->getEnergyRegularization()); str += valNum + "\n";
        str += "\n";

        // Выходные данные
        if(!isTrain || this->construct.bp->getCountTime() == 1)
        {
            this->construct.zad->postrun(this->construct.net->getShiftout());

            this->construct.zad->runEnergyClass(NSetType::NSetTrain);
            str += "Train EnergyClass = "; valNum.setNum(this->construct.zad->getEnergyClass()); str += valNum + " / "; valNum.setNum(this->construct.zad->getTotalClass()); str += valNum + "\n";
            this->construct.zad->runEnergyClass(NSetType::NSetTest);
            str += "Test EnergyClass = "; valNum.setNum(this->construct.zad->getEnergyClass()); str += valNum + " / "; valNum.setNum(this->construct.zad->getTotalClass()); str += valNum + "\n";
            this->construct.zad->runEnergyClass(NSetType::NSetValidate);
            str += "Validate EnergyClass = "; valNum.setNum(this->construct.zad->getEnergyClass()); str += valNum + " / "; valNum.setNum(this->construct.zad->getTotalClass()); str += valNum + "\n";
            str += "\n";

            for(int pos = this->construct.zad->getBeginset(); pos < this->construct.zad->getEndset(); pos++)
            {
                exm = this->construct.zad->at(pos);
                str += "pos = "; valNum.setNum(pos+1); str += valNum + "\n";
                to_array_string(strvec, exm->output); concat(valvec, strvec, "; ");
                str += "out = "; valNum = QString::fromStdString(valvec); str += valNum + "\n";
                to_array_string(strvec, exm->outrun); concat(valvec, strvec, "; ");
                str += "outrun = "; valNum = QString::fromStdString(valvec); str += valNum + "\n";
                to_array_string(strvec, exm->outpostrun); concat(valvec, strvec, "; ");
                str += "outpost = "; valNum = QString::fromStdString(valvec); str += valNum + "\n\n";
            }
         }
    }
    else
    {
        str += "Error";
    }

    ui->textBrowser->setText(str);

    // Сохранение
    this->construct.deinit_constructor(isTrain);
    if(this->construct.zad != nullptr && !this->construct.fout.isFileExamples) {ui->textBrowser->append("\nSave error: out_examples\n");}
    if(this->construct.net != nullptr && !this->construct.fout.isFileNeuronet) {ui->textBrowser->append("\nSave error: out_neuronet\n");}
    if(this->construct.bp != nullptr && !this->construct.fout.isFileTrainBP) {ui->textBrowser->append("\nSave error: out_trainbp\n");}
}

void MainWindow::on_pushButton_run_clicked()
{
    bool isLoad;
    bool isTrain = true;
    NError erCheck;
    vector<string> strvec;
    QString str = "", valNum;

    if(this->dir == "") {return;}

    split(strvec, this->dir.toStdString(), "/");
    valNum = QString::fromStdString(strvec.back()); str += valNum + "\n";
    ui->textBrowser->setText(str);

    // Инициализация задачи
    isLoad = this->init_load(ui->comboZad->currentIndex(), ui->comboTrain->currentIndex(), isTrain, true);
    if(!isLoad) {return;}
    this->comitet.zad->prerun();
    this->comitet.update_examples();

    //Проверки
    if(!this->comitet.check())
    {
        for(int ind = 0; ind < this->comitet.getLength(); ind++)
        {
            erCheck = this->comitet.erCheck[ind];
            if(erCheck != NError::NErrorNone)
            {
                valNum.setNum(static_cast<int>(erCheck));
                str += "Error code " + QString::fromStdString(this->comitet.names[ind]) + ": " + valNum + "\n";
            }
        }
        ui->textBrowser->setText(str);
        return;
    }

    // Основной цикл
    this->activForm(false);
    this->comitet.initProccess();
    if(this->comitet.outdata_create(this->dir.toStdString()))
    {
        this->stopapp = false;
        this->tcommand_work = true;
        this->tcommand = new std::thread(&MainWindow::runCommand, this);
        //this->runCommand();
    }
    //this->activForm(true);
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
    this->ui->comboZad->setEnabled(bl);
    this->ui->pushButton_dir->setEnabled(bl);
    this->ui->pushButton_run->setEnabled(bl);
    this->ui->pushButton_func->setEnabled(bl);
    this->ui->pushButton_train->setEnabled(bl);
    this->ui->pushButton_stop->setEnabled(!bl);
}

void MainWindow::runCommand()
{
    bool blLoop, blTrain = false;
    //bool isOneStep = true;
    NCommandNet command;
    QString comstr, txtstr;
    int indParallel, numIteration = 0;

    if(!this->pf.exists()) {this->pf.create();}
    this->pf.open();
    comstr = QString::fromStdString(to_string(NCommandNet::NComBeg)); this->pf.writeWBufFormat(comstr);
    if(!this->pf.write()) {ui->textBrowser->append("Error write pipe begin!\n"); this->stopapp = true;}

    while(!this->stopapp)
    {
        blLoop = this->pf.waitForLoop(10);
        if(!blLoop) {continue;}

        if(this->pf.read())
        {
            to_value(command, this->pf.mes.command.toStdString());
            if(command == NCommandNet::NComRunTrain)
            {
                //Загрузка данных для обучения
                blTrain = true;
                txtstr.setNum(numIteration+1);
                ui->textBrowser->append("#" + txtstr + ". Prerun...");
                this->timer.start();
                this->comitet.load_zad(this->dir.toStdString());
                this->comitet.zad->prerun();
                if(this->comitet.getBlShiftEnd())
                {
                    this->comitet.zad->setEndset(this->comitet.zad->getValidset());
                    this->comitet.zad->portionend();
                }
                this->comitet.update_examples();

                //Обучение
                ui->textBrowser->append("Begin train...");
                this->comitet.init_train();
                //if(isOneStep) {this->comitet.init_train(); isOneStep = false;}
                if(this->pf.mes.param.length() > 0)
                {
                    indParallel = this->comitet.thread_train(this->pf.mes.param[0].toStdString());
                    this->comitet.waitProccess(indParallel);
                }
                else
                {
                    this->comitet.thread_train();
                    this->comitet.waitManager();
                }

                //Сохранение данных
                if(this->comitet.getBlNCSave()) {this->comitet.nc_save(this->dir.toStdString(), true);} //Сохраняем текущие данные комитета
                comstr = QString::fromStdString(to_string(NCommandNet::NComEndTrain)); this->pf.writeWBufFormat(comstr);
                if(!this->pf.write()) {ui->textBrowser->append("Error write pipe: NComEndTrain!\n"); this->stopapp = true;}
                this->timer.stop();

                //Замер времени на обучение
                if(blTrain)
                {
                    this->comitet.timer.addBeg(this->timer.getStartTime());
                    this->comitet.timer.addEnd(this->timer.getEndTime());
                }

                //Вывод результата
                ui->textBrowser->append("Time train = " + QString::fromStdString(this->comitet.timer.toString()) + "\n");
                numIteration++;
            }

            else if(command == NCommandNet::NComRunFunc)
            {
                //Загрузка данных для функционирования
                ui->textBrowser->append("Begin func...");
                this->comitet.load_zad(this->dir.toStdString());
                this->comitet.zad->prerun();
                if(this->comitet.getBlShiftEnd())
                {
                    this->comitet.zad->setBeginset(this->comitet.zad->getValidset());
                    this->comitet.zad->setTestset(this->comitet.zad->getValidset());
                    this->comitet.zad->portionend();
                }
                this->comitet.update_examples();

                //Функционирование
                if(this->pf.mes.param.length() > 0) {this->comitet.runFuncName(this->pf.mes.param[0].toStdString(), NSetType::NSetValidate, true);}
                else {this->comitet.runFunc(NSetType::NSetValidate, true);}
                //cout << "Func = " << ((NTradeProfit<NTypeValue>*)(this->comitet.nets[0]->tmp_examples))->dataZad.date.getLength() << endl;

                //Сохраняем выходные данные комитета
                this->comitet.outdata_save(this->dir.toStdString());
                comstr = QString::fromStdString(to_string(NCommandNet::NComUpdateOut)); this->pf.writeWBufFormat(comstr);
                if(!this->pf.write()) {ui->textBrowser->append("Error write pipe: NComUpdateOut!\n"); this->stopapp = true;}
                blTrain = false;

                //Вывод результата
                ui->textBrowser->append("Update out\n");
            }

            else if(command == NCommandNet::NComNCSave)
            {
                //Сохраняем текущие данные комитета
                ui->textBrowser->append("Begin save...");
                this->comitet.nc_save(this->dir.toStdString(), true);
                comstr = QString::fromStdString(to_string(NCommandNet::NComIsSave)); this->pf.writeWBufFormat(comstr);
                if(!this->pf.write()) {ui->textBrowser->append("Error write pipe: NComIsSave!\n");}

                //Вывод результата
                ui->textBrowser->append("Is save\n");
            }

            else if(command == NCommandNet::NComFinish)
            {
                this->stopapp = true;
                blTrain = false;
            }
        }

        else if(!this->pf.eof())
        {
            ui->textBrowser->append("Error read!\n");
        }
    }

    ui->textBrowser->append("Finish!");
    comstr = QString::fromStdString(to_string(NCommandNet::NComEnd)); this->pf.writeWBufFormat(comstr);
    if(!this->pf.write()) {ui->textBrowser->append("Error write pipe end!\n");}
    this->pf.close();

    this->tcommand_work = false;
    //this->activForm(true);
}
