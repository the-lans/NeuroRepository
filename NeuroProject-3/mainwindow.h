#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <QMainWindow>
#include <QString>
#include <QtGui>
#include <QFileDialog>

#include "pipefile.h"
#include "neuroconstructor.h"
#include "neurocomitet.h"
#include "ntradetg.h"
#include "ntradeprofit.h"
#include "ncartpole.h"
#include "smarttimer.h"

typedef float NTypeValue;

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
    void on_pushButton_dir_clicked();
    void on_pushButton_train_clicked();
    void on_pushButton_func_clicked();
    void on_pushButton_run_clicked();
    void on_pushButton_stop_clicked();

private:
    void init_files();
    bool init_load(int variantZad, int variantTrain, bool isTrain, bool isComitet);
    void init_struct(bool isTrain, bool isComitet);
    void run(bool isTrain);
    void activForm(bool bl);
    void runCommand();

private:
    Ui::MainWindow* ui;
    DataECSV dt;
    QString dir;
    NeuroConstructor<NTypeValue> construct;
    NeuroComitet<NTypeValue> comitet;
    PipeFile pf;
    bool stopapp;
    std::thread* tcommand;
    bool tcommand_work;
    SmartTimer timer;
};

#endif // MAINWINDOW_H
