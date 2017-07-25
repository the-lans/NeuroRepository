#ifndef DATACSVAPROX_H
#define DATACSVAPROX_H

#include "idatacsv.h"

template <typename NType>
class DataCSVAprox: public IDataCSV<NType>
{
public:
    DataCSVAprox();
public:
    virtual void calculate();
};


template <typename NType>
DataCSVAprox<NType>::DataCSVAprox()
 :IDataCSV<NType>()
{
}

template <typename NType>
void DataCSVAprox<NType>::calculate()
{
    this->mtrx.init(this->data.length(), 2, 0);
    NType val = 0;
    int ind, jnd;

    ind = 0;
    for(const QStringList& curStr: this->data){
        jnd = 0;
        for(const QString& item: curStr){
            if (this->type_value == NTypeEnum::NTypeFloat) {val = item.toFloat();}
            else if(this->type_value == NTypeEnum::NTypeDouble) {val = item.toDouble();}
            else if(this->type_value == NTypeEnum::NTypeInt) {val = item.toInt();}
            this->mtrx.set(val, ind, jnd);
            jnd++;
        }
        ind++;
    }
}

#endif // DATACSVAPROX_H
