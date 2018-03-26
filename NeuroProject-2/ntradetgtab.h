#ifndef NTRADETGTAB_H
#define NTRADETGTAB_H

#include "narray.h"
#include "nmatrix.h"
#include "iobjectecsv.h"
#include "neuroproject.h"
#include "ntrade.h"
#include <string>

using namespace std;

template <typename NType>
struct NTradeTgTab: public IObjectECSV
{
    NArray<string> date;
    NArray<string> time;
    NArray<NType> open_real;
    NArray<NType> high_real;
    NArray<NType> low_real;
    NArray<NType> close_real;
    NArray<NType> volume_real;
    NArray<NType> high_calc;
    NArray<NType> low_calc;
    NArray<NType> energy;
    NArray<NSetType> typeExample;

    int getLenRow();
    int getLenColumn();
    void init(int len);
    void deinit();
    void convertMatrix(NMatrix<string>& mtrx);
    NMatrix<string>& toMatrix(NMatrix<string>& mtrx);
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
int NTradeTgTab<NType>::getLenRow()
{
    return this->close_real.getLength();
}

template <typename NType>
int NTradeTgTab<NType>::getLenColumn()
{
    if(this->high_calc.empty() || this->low_calc.empty()) {return 7;}
    else if(this->typeExample.empty()) {return 10;}
    else {return 11;}
}

template <typename NType>
void NTradeTgTab<NType>::init(int len)
{
    this->date.init(len, "");
    this->time.init(len, "");
    this->open_real.init(len, 0);
    this->high_real.init(len, 0);
    this->low_real.init(len, 0);
    this->close_real.init(len, 0);
    this->volume_real.init(len, 0);
    this->high_calc.init(len, 0);
    this->low_calc.init(len, 0);
    this->energy.init(len, 0);
    this->typeExample.init(len, NSetType::NSetNone);
}

template <typename NType>
void NTradeTgTab<NType>::deinit()
{
    this->date.clear();
    this->time.clear();
    this->open_real.clear();
    this->high_real.clear();
    this->low_real.clear();
    this->close_real.clear();
    this->volume_real.clear();
    this->high_calc.clear();
    this->low_calc.clear();
    this->energy.clear();
    this->typeExample.clear();
}

template <typename NType>
void NTradeTgTab<NType>::convertMatrix(NMatrix<string>& mtrx)
{
    vector<string> str_vec;

    mtrx.getColumn(0, this->date);
    mtrx.getColumn(1, this->time);
    mtrx.getColumn(2, str_vec); to_array_value(this->open_real, str_vec);
    mtrx.getColumn(3, str_vec); to_array_value(this->high_real, str_vec);
    mtrx.getColumn(4, str_vec); to_array_value(this->low_real, str_vec);
    mtrx.getColumn(5, str_vec); to_array_value(this->close_real, str_vec);
    mtrx.getColumn(6, str_vec); to_array_value(this->volume_real, str_vec);

    if(mtrx.getLenColumn() >= 10)
    {
        mtrx.getColumn(7, str_vec); to_array_value(this->high_calc, str_vec);
        mtrx.getColumn(8, str_vec); to_array_value(this->low_calc, str_vec);
        mtrx.getColumn(9, str_vec); to_array_value(this->energy, str_vec);
    }
    if(mtrx.getLenColumn() >= 11)
    {
        mtrx.getColumn(10, str_vec); to_array_value(this->typeExample, str_vec);
    }
}

template <typename NType>
NMatrix<string>& NTradeTgTab<NType>::toMatrix(NMatrix<string>& mtrx)
{
    vector<string> str_vec;
    mtrx.clear();

    if(this->date.empty() || this->time.empty())
    {
        mtrx.init(this->getLenRow(), 2, "");
    }
    else
    {
        mtrx.pushColumn(this->date);
        mtrx.pushColumn(this->time);
    }
    to_array_string(str_vec, this->open_real); mtrx.pushColumn(str_vec);
    to_array_string(str_vec, this->high_real); mtrx.pushColumn(str_vec);
    to_array_string(str_vec, this->low_real); mtrx.pushColumn(str_vec);
    to_array_string(str_vec, this->close_real); mtrx.pushColumn(str_vec);
    to_array_string(str_vec, this->volume_real); mtrx.pushColumn(str_vec);
    if (!this->high_calc.empty() && !this->low_calc.empty())
    {
        to_array_string(str_vec, this->high_calc); mtrx.pushColumn(str_vec);
        to_array_string(str_vec, this->low_calc); mtrx.pushColumn(str_vec);
        to_array_string(str_vec, this->energy); mtrx.pushColumn(str_vec);
    }
    if(!this->typeExample.empty())
    {
        to_array_string(str_vec, this->typeExample); mtrx.pushColumn(str_vec);
    }

    return mtrx;
}

template <typename NType>
void NTradeTgTab<NType>::saveECSV(DataECSV& dt, string& parent)
{
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    //string str_val;
    //string field;

    dt.addGroup(parent, "");
    dt.addString(parent, "date", this->date);
    dt.addString(parent, "time", this->time);
    to_array_string(str_vec, this->open_real); dt.addElement(parent, "open_real", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->high_real); dt.addElement(parent, "high_real", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->low_real); dt.addElement(parent, "low_real", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->close_real); dt.addElement(parent, "close_real", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->volume_real); dt.addElement(parent, "volume_real", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->high_calc); dt.addElement(parent, "high_calc", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->low_calc); dt.addElement(parent, "low_calc", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energy); dt.addElement(parent, "energy", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->typeExample); dt.addElement(parent, "typeExample", str_vec, typeid(NType).name());
}

template <typename NType>
void NTradeTgTab<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    //string str_val;

    this->deinit();

    if(dt.isOneMatrix())
    {
        iter = dt.modules[0];
        this->convertMatrix(iter->mtrx);
    }
    else
    {
        size_t ind = dt.getShift();
        iter = dt.modules[ind];
        if(iter->cmpPath(parent, true)) {ind++;}

        while(dt.isNextBlock(ind, parent))
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "date", this->date)) {;}
            else if(iter->getFieldValue(parent, "time", this->time)) {;}
            else if(iter->getFieldValue(parent, "open_real", str_vec)) {to_array_value(this->open_real, str_vec);}
            else if(iter->getFieldValue(parent, "high_real", str_vec)) {to_array_value(this->high_real, str_vec);}
            else if(iter->getFieldValue(parent, "low_real", str_vec)) {to_array_value(this->low_real, str_vec);}
            else if(iter->getFieldValue(parent, "close_real", str_vec)) {to_array_value(this->close_real, str_vec);}
            else if(iter->getFieldValue(parent, "volume_real", str_vec)) {to_array_value(this->volume_real, str_vec);}
            else if(iter->getFieldValue(parent, "high_calc", str_vec)) {to_array_value(this->high_calc, str_vec);}
            else if(iter->getFieldValue(parent, "low_calc", str_vec)) {to_array_value(this->low_calc, str_vec);}
            else if(iter->getFieldValue(parent, "energy", str_vec)) {to_array_value(this->energy, str_vec);}
            else if(iter->getFieldValue(parent, "typeExample", str_vec)) {to_array_value(this->typeExample, str_vec);}

            ind++;
        }

        dt.setShift(ind);
    }
}

#endif // NTRADETGTAB_H
