#ifndef DATACSVTRADE_H
#define DATACSVTRADE_H

#include "idatacsv.h"

template <typename NType>
class DataCSVTrade: public IDataCSV<NType>
{
public:
    DataCSVTrade();
public:
    virtual void calculate();
};


template <typename NType>
DataCSVTrade<NType>::DataCSVTrade()
 :IDataCSV<NType>()
{
}

template <typename NType>
void DataCSVTrade<NType>::calculate()
{
    this->mtrx.init(this->data.length(), 5, 0);
    NType val = 0;
    int ind, jnd;

    ind = 0;
    for(const QStringList& curStr: this->data){
        jnd = 0;
        for(const QString& item: curStr){
            if(jnd < 2) {jnd++; continue;}
            if (this->type_value == NTypeEnum::NTypeFloat) {val = item.toFloat();}
            else if(this->type_value == NTypeEnum::NTypeDouble) {val = item.toDouble();}
            else if(this->type_value == NTypeEnum::NTypeInt) {val = item.toInt();}
            this->mtrx.set(val, ind, jnd-2);
            jnd++;
        }
        ind++;
    }
}

#endif // DATACSVTRADE_H
