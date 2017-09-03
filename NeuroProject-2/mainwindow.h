#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtGui>
#include <QFileDialog>

#include "narray.h"
#include "nmatrix.h"
#include "noperators.h"
#include "neuroproject.h"

#include "dataecsv.h"
#include "nmatrixecsv.h"
//#include "idatacsv.h"
//#include "datacsvtrade.h"
//#include "datacsvaprox.h"

#include "ineuronet.h"
#include "naprox.h"
#include "ntradetg.h"
#include "trainbp.h"

typedef double NTypeValue;

namespace Ui {
    class MainWindow;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_load_clicked();
    void on_pushButton_save_clicked();

private:
    Ui::MainWindow *ui;
    //DataCSVAprox<NTypeValue> dt;
    //DataCSVTrade<NTypeValue> dt;
    DataECSV dt;
    QString dir;
    NMatrixECSV<NTypeValue> tab;
    //NAprox<NTypeValue> zad;
    NTradeTg<NTypeValue> zad;
    INeuroNet<NTypeValue> net;
    TrainBP<NTypeValue> bp;
};

#endif // MAINWINDOW_H
