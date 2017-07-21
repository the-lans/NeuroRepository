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

#endif // DATACSVAPROX_H
