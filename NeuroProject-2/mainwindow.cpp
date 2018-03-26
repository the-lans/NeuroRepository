#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    time_srand();
    init_random_generator();
    this->dt.setArrayBR(true);
    this->dir = QDir::currentPath();
    //this->dir = "I:/!ProgramProjects/NeuroProject/NeuroProject-2/trade_test";
    //this->dir = "D:/ProgramProjects/NeuroProject/NeuroProject-2/trade_test";
    //this->dir = "D:/Ilya/Prog/NeuroProject-3/tradeprofit_2_week";

    this->construct.fin.current_dir = dir.toStdString();
    this->construct.fout.current_dir = dir.toStdString();
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

    this->construct.fin.current_dir = dir.toStdString();
    this->construct.fout.current_dir = dir.toStdString();
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
    this->construct.fin.name_examples = "in_examples.txt";
    this->construct.fin.name_neuronet = "in_neuronet.txt";
    this->construct.fin.name_trainbp = "in_trainbp.txt";

    this->construct.fout.name_examples = "out_examples.csv";
    this->construct.fout.name_neuronet = "out_neuronet.csv";
    this->construct.fout.name_trainbp = "out_trainbp.csv";
}

void MainWindow::run(bool isTrain)
{
    bool isLoad;
    string parent = "";
    NExample<NTypeValue>* exm;
    string valvec;
    vector<string> strvec;
    QString str, valNum;

    this->init_files();
    //if(!isTrain) {this->construct.doShift();}
    if(this->dir == "") {return;}

    split(strvec, this->dir.toStdString(), "/");
    valNum = QString::fromStdString(strvec.back()); str += valNum + "\n\n";

    // Задача
    int variant = ui->comboZad->currentIndex();
    if(variant == 0) //Общая задача
    {
        this->construct.init_constructor(isTrain);
        this->construct.zad->prerun();
    }
    else if(variant == 1) //Задача Trade High/Low
    {
        this->construct.zad = new NTradeTg<NTypeValue>;
        this->construct.init_constructor(isTrain);

        // Примеры
        isLoad = dt.read(dir.toStdString() + "/bars.csv");
        if(isLoad)
        {
            ((NTradeTg<NTypeValue>*)this->construct.zad)->dataTab.loadECSV(dt, parent);
            this->construct.zad->prerun();
            dt.clear();
        }
    }

    //Проверки
    if(!this->construct.net->check())
    {
        valNum.setNum((int)this->construct.net->getCheck());
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
            this->construct.train(); //Процесс обучения
            str += "Time = "; valNum.setNum(this->construct.search_time); str += valNum + "\n";
            str += "Epoches = "; valNum.setNum(this->construct.bp->getEpoches()); str += valNum + "\n";
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
            this->construct.zad->postrun();

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
