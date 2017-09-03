#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->dt.setArrayBR(true);
    this->dir = "K:\\!ProgramProjects\\NeuroProject\\NeuroProject-2\\files";
    //this->dir = "D:\\Ilya\\Prog\\NeuroProject-2";
    //this->dir = QDir::currentPath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //ui->textBrowser->setText("Hello world!");

    // Инициализация
    net.ginit(net.lenNeurons, net.typeLay);
    bp.init();

    // Обучение
    NTypeValue val;
    QString str, valNum;
    if(this->zad.getLength() > 0)
    {
        net.runExamples(NSetType::NSetTrain);
        net.funcRegularization();
        str += "Energy = "; valNum.setNum(zad.getEnergyAver()); str += valNum + "\n";
        str += "EnergySum = "; valNum.setNum(zad.getEnergySum()); str += valNum + "\n";
        str += "EnergyRegularization = "; valNum.setNum(net.getEnergyRegularization()); str += valNum + "\n";
        str += "\n";

        bp.train(); //Процесс обучения

        net.funcRegularization();
        str += "Epoches = "; valNum.setNum(bp.getEpoches()); str += valNum + "\n";
        str += "StabEnergy = "; valNum.setNum(bp.getStabEnergy()); str += valNum + "\n";
        str += "Energy = "; valNum.setNum(zad.getEnergyAver()); str += valNum + "\n";
        str += "EnergySum = "; valNum.setNum(zad.getEnergySum()); str += valNum + "\n";
        str += "EnergyRegularization = "; valNum.setNum(net.getEnergyRegularization()); str += valNum + "\n";
        str += "ChangeEnergy = "; valNum.setNum(bp.getChangeEnergy()); str += valNum + "\n";
        str += "\n";

        // Выходные данные
        for(int i = 0; i < zad.getLength(); i++)
        {
            zad.outrun = &(zad[i]->outrun);
            zad.postrun();
            val = zad[i]->outrun[0];
            str += "out = "; valNum.setNum(val); str += valNum + "\n";
            val = zad.outpostrun[0];
            str += "outpost = "; valNum.setNum(val); str += valNum + "\n";
        }
    }
    else
    {
        str = "Error";
    }

    ui->textBrowser->setText(str);
}

void MainWindow::on_pushButton_load_clicked()
{
    bool isLoad;
    string parent = "";
    /*this->dir = QFileDialog::getExistingDirectory(this,
                               QString::fromUtf8("Открыть папку"),
                               this->dir,
                               QFileDialog::ShowDirsOnly
                               | QFileDialog::DontResolveSymlinks);
    if(this->dir == "") {return;}*/

    // Чтение из файла CSV примеров
    //dt.read(QString::fromStdString(this->dir + "test.csv"), ",");
    //dt.read(this->dir + "test.csv", ",");
    //dt.read(this->dir + "test.csv", ",");
    //dt.calculate();
    //NMatrix<NTypeValue>& tab = dt.getMatrix();

    // Параметры задачи
    zad.deinit();
    zad.setPeriod(1);
    zad.setLenIn(8);
    zad.setPT(NPriceType::PriceHigh);
    zad.setKoefTg(10000);
    zad.setKoefPrice(0.1);
    zad.setKoefVolume(0.1);

    // Конструктор нейросети
    net.deinit();
    net.examples = &zad;
    net.lenNeurons.push(4*8);
    net.lenNeurons.push(4); net.typeLay.push(NLayerType::NFuncSoftsign);
    net.lenNeurons.push(2); net.typeLay.push(NLayerType::NFuncSoftsign);
    net.lenNeurons.push(1); net.typeLay.push(NLayerType::NFuncArctg);
    net.setValueWeight(0.5);

    // Параметры обучения
    bp.deinit();
    bp.net = &net;
    bp.setInertia(0.10);
    bp.setSpeedLearning(0.02);
    bp.setSpeedRegularization(0.02);
    bp.setMaxEpoches(100000);
    bp.setMinError(0.0025);
    bp.setMinErrorChange(0.0);

    isLoad = this->dt.read(this->dir.toStdString() + "\\in_examples.txt");
    if(isLoad) {this->zad.loadECSV(this->dt, parent);}

    isLoad = this->dt.read(this->dir.toStdString() + "\\in_neuronet.txt");
    if(isLoad) {this->net.loadECSV(this->dt, parent);}

    isLoad = this->dt.read(this->dir.toStdString() + "\\in_trainbp.txt");
    if(isLoad) {this->bp.loadECSV(this->dt, parent);}

    tab.clear();
    isLoad = this->dt.read(this->dir.toStdString() + "\\bars.csv");
    if(isLoad)
    {
        this->tab.loadECSV(this->dt, parent);
        zad.prerun(tab);
    }

    // Инициализация
    if(net.lay.getLength() == 0) {net.ginit(net.lenNeurons, net.typeLay);}
    bp.init();
}

void MainWindow::on_pushButton_save_clicked()
{
    bool isSave;
    string parent = "";
    /*this->dir = QFileDialog::getExistingDirectory(this,
                               QString::fromUtf8("Открыть папку"),
                               this->dir,
                               QFileDialog::ShowDirsOnly
                               | QFileDialog::DontResolveSymlinks);
    if(this->dir == "") {return;}*/

    this->dt.clear();
    this->zad.saveECSV(this->dt, parent);
    isSave = this->dt.write(this->dir.toStdString() + "\\out_examples.csv");

    this->dt.clear();
    this->net.saveECSV(this->dt, parent);
    isSave = this->dt.write(this->dir.toStdString() + "\\out_neuronet.csv");

    this->dt.clear();
    this->bp.saveECSV(this->dt, parent);
    isSave = this->dt.write(this->dir.toStdString() + "\\out_trainbp.csv");
}
