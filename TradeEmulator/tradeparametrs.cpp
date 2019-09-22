#include "tradeparametrs.h"

TradeParametrs::TradeParametrs()
{
    this->period = 1;
    this->timeframe_min = 1;
    this->lenInBar = 0;
    this->wind = 0;
    this->vStopLoss = 0;
    this->vTakeProfit = 0;
    this->stepExmTime = 0;
}

void TradeParametrs::saveECSV(DataECSV& dt, string& parent)
{
    string str_val;

    dt.addGroup(parent, "");
    str_val = to_string(this->period); dt.addElement(parent, "period", str_val, typeid(int).name());
    str_val = to_string(this->timeframe_min); dt.addElement(parent, "timeframe_min", str_val, typeid(int).name());
    str_val = to_string(this->lenInBar); dt.addElement(parent, "lenInBar", str_val, typeid(int).name());
    str_val = to_string(this->wind); dt.addElement(parent, "wind", str_val, typeid(int).name());
    str_val = to_string(this->vStopLoss); dt.addElement(parent, "vStopLoss", str_val, typeid(float).name());
    str_val = to_string(this->vTakeProfit); dt.addElement(parent, "vTakeProfit", str_val, typeid(float).name());
    str_val = to_string(this->stepExmTime); dt.addElement(parent, "stepExmTime", str_val, typeid(int).name());
    str_val = to_string(this->countIteration); dt.addElement(parent, "countIteration", str_val, typeid(int).name());

    dt.addString(parent, "name_inarch", this->name_inarch);
    dt.addString(parent, "name_input", this->name_input);
    dt.addString(parent, "name_outdata", this->name_outdata);
    dt.addString(parent, "name_outtotal", this->name_outtotal);
}

void TradeParametrs::loadECSV(DataECSV& dt, string& parent)
{
    NMatrix<float> mtrx_input;
    StructECSV* iter;
    string str_val;

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        to_matrix_value(mtrx_input, iter->mtrx);
    }
    else
    {
        //Поля класса
        size_t ind = dt.getShift();
        iter = dt.modules[ind];
        if(iter->cmpPath(parent, true)) {ind++;}

        while(dt.isNextBlock(ind, parent))
        {
            iter = dt.modules[ind];

                 if(iter->getFieldValue(parent, "period", str_val)) {to_value(this->period, str_val);}
            else if(iter->getFieldValue(parent, "timeframe_min", str_val)) {to_value(this->timeframe_min, str_val);}
            else if(iter->getFieldValue(parent, "lenInBar", str_val)) {to_value(this->lenInBar, str_val);}
            else if(iter->getFieldValue(parent, "wind", str_val)) {to_value(this->wind, str_val);}
            else if(iter->getFieldValue(parent, "vStopLoss", str_val)) {to_value(this->vStopLoss, str_val);}
            else if(iter->getFieldValue(parent, "vTakeProfit", str_val)) {to_value(this->vTakeProfit, str_val);}
            else if(iter->getFieldValue(parent, "stepExmTime", str_val)) {to_value(this->stepExmTime, str_val);}
            else if(iter->getFieldValue(parent, "countIteration", str_val)) {to_value(this->countIteration, str_val);}

            else if(iter->getFieldValue(parent, "name_inarch", str_val)) {this->name_inarch = str_val;}
            else if(iter->getFieldValue(parent, "name_input", str_val)) {this->name_input = str_val;}
            else if(iter->getFieldValue(parent, "name_outdata", str_val)) {this->name_outdata = str_val;}
            else if(iter->getFieldValue(parent, "name_outtotal", str_val)) {this->name_outtotal = str_val;}

            ind++;
        }

        dt.setShift(ind);
    }
}
