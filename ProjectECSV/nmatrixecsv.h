#ifndef NMATRIXECSV_H
#define NMATRIXECSV_H

#include "nmatrix.h"
#include "iobjectecsv.h"

template <typename NType>
class NMatrixECSV: public NMatrix<NType>, public IObjectECSV
{
public:
    NMatrixECSV();
public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NMatrixECSV<NType>::NMatrixECSV():
    NMatrix<NType>()
{
}

template <typename NType>
void NMatrixECSV<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<string> str_mtrx;
    to_matrix_string(str_mtrx, *this); dt.addElement(parent, "", str_mtrx, typeid(NType).name());
}

template <typename NType>
void NMatrixECSV<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    NMatrix<string> str_mtrx;

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        to_matrix_value(*this, iter->mtrx);
    }
    else if(dt.modules.size() > 0)
    {
        //Поля класса
        size_t ind = dt.getShift();
        iter = dt.modules[ind];
        to_matrix_value(*this, iter->mtrx);
        ind++; dt.setShift(ind);
    }
}

#endif // NMATRIXECSV_H
