#include "tradeemulator.h"

TradeEmulator::TradeEmulator(): pf("command")
{
    dir = "D:/Ilya/Prog/NeuroProject-3/comitet_class";
}

void TradeEmulator::runCommand()
{
    bool blLoop, isOneStep = true;
    NCommandNet command;
    QString comstr;
    int indParallel;

    this->pf.setPath(this->dir);
    if(!this->pf.exists()) {this->pf.create();}
    this->pf.open();
    comstr = QString::fromStdString(to_string(NCommandNet::NComBeg));
    if(!this->pf.writeFormat(comstr)) {ui->textBrowser->append("Error write pipe begin!\n"); this->stopapp = true;}

    while(!this->stopapp)
    {
        blLoop = this->pf.waitForLoop(10);
        if(!blLoop) {continue;}

        if(this->pf.read())
        {
            to_value(command, this->pf.command.toStdString());
            if(command == NCommandNet::NComRunTrain)
            {
                //Загрузка данных для обучения
                this->comitet.load_zad(this->dir.toStdString());
                this->comitet.zad->prerun();
                if(this->comitet.getBlShiftEnd())
                {
                    this->comitet.zad->setEndset(this->comitet.zad->getValidset());
                    this->comitet.zad->portionend();
                }
                this->comitet.update_examples();
                if(isOneStep) {this->comitet.init_train(); isOneStep = false;}

                //Обучение
                if(this->pf.param.length() > 0)
                {
                    indParallel = this->comitet.thread_train(this->pf.param[0].toStdString());
                    this->comitet.waitProccess(indParallel);
                }
                else
                {
                    this->comitet.thread_train();
                    this->comitet.waitManager();
                }

                //Сохранение данных
                this->comitet.nc_save(this->dir.toStdString(), true); //Сохраняем текущие данные комитета
                comstr = QString::fromStdString(to_string(NCommandNet::NComEndTrain));
                if(!this->pf.writeFormat(comstr)) {ui->textBrowser->append("Error write pipe: NComEndTrain!\n"); this->stopapp = true;}
            }

            else if(command == NCommandNet::NComRunFunc)
            {
                //Загрузка данных для функционирования
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
                if(this->pf.param.length() > 0) {this->comitet.runFuncName(this->pf.param[0].toStdString(), NSetType::NSetValidate);}
                else {this->comitet.runFunc(NSetType::NSetValidate, true);}

                //Сохраняем выходные данные комитета
                this->comitet.outdata_save(this->dir.toStdString());
                comstr = QString::fromStdString(to_string(NCommandNet::NComUpdateOut));
                if(!this->pf.writeFormat(comstr)) {ui->textBrowser->append("Error write pipe: NComUpdateOut!\n"); this->stopapp = true;}
            }

            else if(command == NCommandNet::NComFinish)
            {
                this->stopapp = true;
            }
        }
        else
        {
            ui->textBrowser->append("Error read!\n");
        }
    }

    comstr = QString::fromStdString(to_string(NCommandNet::NComEnd));
    if(!this->pf.writeFormat(comstr)) {ui->textBrowser->append("Error write pipe end!\n");}
    this->pf.close();

    this->tcommand_work = false;
    //this->activForm(true);
}
