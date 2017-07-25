#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "neuroproject.h"
#include "narray.h"
#include "nmatrix.h"
#include "idatacsv.h"
#include "datacsvtrade.h"
#include "datacsvaprox.h"

#include "nexample.h"
#include "inlayer.h"
#include "nlayertanh.h"
#include "nlayersoftsign.h"
#include "nlayerarctg.h"
#include "nlayerlinear.h"

#include "ineuronet.h"
#include "naprox.h"
#include "ntradetg.h"
#include "trainbp.h"

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
    dt.read("D:\\ProgramProjects\\NeuroProject\\NeuroProject-2\\test.csv", ",");
    //dt.read("D:\\Ilya\\Prog\\NeuroProject-2\\test.csv", ",");
    //dt.read("D:\\Flash\\Project\\Neuro\\NeuroProject\\test.csv", ",");
    dt.calculate();
    NMatrix<NTypeValue>& tab = dt.getMatrix();

    // Конструктор нейросети
    NArray<int> n_num; //int val_num;
    NArray<NLayerType> n_lay_type; //NLayerType val_lay_type;
    n_num.push(4*8);
    n_num.push(4); n_lay_type.push(NLayerType::NFuncSoftsign);
    n_num.push(2); n_lay_type.push(NLayerType::NFuncSoftsign);
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
    /*val = 2.6104; net.lay[0]->weigth.set(val, 0, 0);
    val = 0.1643; net.lay[0]->weigth.set(val, 0, 1);
    val = -0.9911; net.lay[0]->weigth.set(val, 0, 2);
    val = 2.7722; net.lay[0]->weigth.set(val, 0, 3);

    val = 5.0491; net.lay[0]->bias.set(val, 0);
    val = 3.0623; net.lay[0]->bias.set(val, 1);
    val = -1.4778; net.lay[0]->bias.set(val, 2);
    val = -4.7279; net.lay[0]->bias.set(val, 3);

    val = -4.4472; net.lay[1]->weigth.set(val, 0, 0);
    val = 6.2498; net.lay[1]->weigth.set(val, 1, 0);
    val = 2.3899; net.lay[1]->weigth.set(val, 2, 0);
    val = 6.0804; net.lay[1]->weigth.set(val, 3, 0);

    val = 5.4451; net.lay[1]->bias.set(val, 0);*/

    /*val = -0.77; net.lay[0]->weigth.set(val, 0, 0);
    val = -0.4; net.lay[0]->weigth.set(val, 0, 1);
    val = 3.89; net.lay[0]->weigth.set(val, 0, 2);
    val = -3.33; net.lay[0]->weigth.set(val, 0, 3);

    val = -4.31; net.lay[0]->bias.set(val, 0);
    val = 3.4; net.lay[0]->bias.set(val, 1);
    val = -4.64; net.lay[0]->bias.set(val, 2);
    val = -3.91; net.lay[0]->bias.set(val, 3);

    val = -1.62; net.lay[1]->weigth.set(val, 0, 0);
    val = 4.99; net.lay[1]->weigth.set(val, 1, 0);
    val = 4.17; net.lay[1]->weigth.set(val, 2, 0);
    val = 3.59; net.lay[1]->weigth.set(val, 3, 0);

    val = 1.13; net.lay[1]->bias.set(val, 0);*/

    /*val = 1.0; net.lay[0]->weigth.set(val, 0, 0);
    val = 0.0; net.lay[0]->bias.set(val, 0);*/

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
        bp.setMinError(0.0025);
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
            net.setOutRun(&(net.exam[i]->outrun));
            net.postrun();
            val = net.exam[i]->outrun[0];
            str += "out = "; valNum.setNum(val); str += valNum + "\n";
            val = net.outpostrun[0];
            str += "outpost = "; valNum.setNum(val); str += valNum + "\n";
        }
    }
    else
    {
        str = "Error";
    }
    ui->textBrowser->setText(str);
}
