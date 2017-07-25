#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "neuroproject.cpp"
#include "narray.cpp"
#include "nmatrix.cpp"
#include "idatacsv.cpp"
#include "datacsvtrade.cpp"
#include "datacsvaprox.cpp"

#include "nexample.cpp"
#include "inlayer.cpp"
#include "nlayertanh.cpp"
#include "nlayersoftsign.cpp"
#include "nlayerarctg.cpp"
#include "nlayerlinear.cpp"

#include "ineuronet.cpp"
#include "naprox.cpp"
#include "ntradetg.cpp"
#include "trainbp.cpp"

typedef double NTypeValue;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //ui->textBrowser->setText("Hello world!");

    // Чтение из файла CSV примеров
    //DataCSVAprox<NTypeValue> dt;
    DataCSVTrade<NTypeValue> dt;
    dt.read("D:\\ProgramProjects\\NeuroProject\\NeuroProject-1\\test.csv", ",");
    dt.calculate();
    NMatrix<NTypeValue>& tab = dt.getMatrix();

    // Конструктор нейросети
    NArray<int> n_num; //int val_num;
    NArray<NLayerType> n_lay_type; //NLayerType val_lay_type;
    n_num.push(4*8);
    n_num.push(4); n_lay_type.push(NLayerType::NFuncTanh);
    n_num.push(2); n_lay_type.push(NLayerType::NFuncTanh);
    n_num.push(1); n_lay_type.push(NLayerType::NFuncArctg);
    //NAprox<NTypeValue> net;
    NTradeTg<NTypeValue> net;
    net.setPeriod(1);
    net.setLenIn(8);
    net.setPT(NPriceType::PriceHigh);
    net.setValueWeight(0.5);
    net.ginit(n_num, n_lay_type);

    // Задание весов
    NTypeValue val;
    /*val = -0.77; net.lay.get(0)->weigth.set(val, 0, 0);
    val = -0.4; net.lay.get(0)->weigth.set(val, 0, 1);
    val = 3.89; net.lay.get(0)->weigth.set(val, 0, 2);
    val = -3.33; net.lay.get(0)->weigth.set(val, 0, 3);

    val = -4.31; net.lay.get(0)->bias.set(val, 0);
    val = 3.4; net.lay.get(0)->bias.set(val, 1);
    val = -4.64; net.lay.get(0)->bias.set(val, 2);
    val = -3.91; net.lay.get(0)->bias.set(val, 3);

    val = -1.62; net.lay.get(1)->weigth.set(val, 0, 0);
    val = 4.99; net.lay.get(1)->weigth.set(val, 1, 0);
    val = 4.17; net.lay.get(1)->weigth.set(val, 2, 0);
    val = 3.59; net.lay.get(1)->weigth.set(val, 3, 0);

    val = 1.13; net.lay.get(1)->bias.set(val, 0);*/

    // Предобработка и выполнение примеров
    //NTypeValue enrg;
    QString str, valNum;
    if(tab.getLenRow() > 0)
    {
        net.setKoefTg(10000);
        net.setKoefPrice(0.1);
        net.setKoefVolume(0.1);
        net.prerun(tab);

        net.runExamples(NSetType::NSetTrain);
        net.funcRegularization();

        str += "Energy = "; valNum.setNum(net.getEnergyAver()); str += valNum + "\n";
        str += "EnergySum = "; valNum.setNum(net.getEnergySum()); str += valNum + "\n";
        str += "EnergyRegularization = "; valNum.setNum(net.getEnergyRegularization()); str += valNum + "\n";
        str += "\n";

        TrainBP<NTypeValue> bp;
        bp.net = &net;
        bp.setInertia(0.10);
        bp.setSpeedLearning(0.02);
        bp.setSpeedRegularization(0.02);
        bp.setMaxEpoches(100000);
        bp.setMinError(0.05);
        bp.setMinErrorChange(0.0);
        bp.init();
        bp.train();
        net.funcRegularization();

        str += "Epoches = "; valNum.setNum(bp.getEpoches()); str += valNum + "\n";
        str += "StabEnergy = "; valNum.setNum(bp.getStabEnergy()); str += valNum + "\n";
        str += "Energy = "; valNum.setNum(net.getEnergyAver()); str += valNum + "\n";
        str += "EnergySum = "; valNum.setNum(net.getEnergySum()); str += valNum + "\n";
        str += "EnergyRegularization = "; valNum.setNum(net.getEnergyRegularization()); str += valNum + "\n";
        str += "ChangeEnergy = "; valNum.setNum(bp.getChangeEnergy()); str += valNum + "\n";
        str += "\n";

        for(int i = 0; i < net.exam.getLength(); i++)
        {
            net.setOutRun(&(net.exam.get(i)->outrun));
            val = net.exam.get(i)->outrun.get(0);
            str += "outrun = "; valNum.setNum(val); str += valNum + "\n";
            net.postrun();
            val = net.outpostrun.get(0);
            str += "outpostrun = "; valNum.setNum(val); str += valNum + "\n";
            val = net.exam.get(i)->output.get(0);
            str += "out = "; valNum.setNum(val); str += valNum + "\n";
        }
    }
    else
    {
        str = "Error";
    }
    ui->textBrowser->setText(str);
}
