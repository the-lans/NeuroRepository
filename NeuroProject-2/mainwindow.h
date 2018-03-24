#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtGui>
#include <QFileDialog>

#include "neuroconstructor.h"
#include "ntradetg.h"

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

private:
    void init_files();
    void run(bool isTrain);

private:
    Ui::MainWindow* ui;
    DataECSV dt;
    QString dir;
    NeuroConstructor<NTypeValue> construct;
};

#endif // MAINWINDOW_H
