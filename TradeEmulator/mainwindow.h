#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <QMainWindow>
#include <QString>
#include <QtGui>
#include <QFileDialog>

#include "dataecsv.h"
#include "pipefile.h"
#include "neuroproject.h"
#include "tradeparametrs.h"
#include "nmatrixecsv.h"
#include "smarttimer.h"

typedef float NTypeValue;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_dir_clicked();
    void on_pushButton_run_clicked();
    void on_pushButton_stop_clicked();

private:
    void activForm(bool bl);
    void runCommand();
    void updateData(int num);

private:
    Ui::MainWindow *ui;
    DataECSV dt;
    QString dir;
    PipeFile pf;
    bool stopapp;
    std::thread* tcommand;
    bool tcommand_work;
    string name_param;
    TradeParametrs tparam;
    NMatrixECSV<std::string> mtrx_bars;
    NMatrixECSV<std::string> mtrx_addbars;
    SmartTimer timer;
};

#endif // MAINWINDOW_H
