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

#endif // DATACSVTRADE_H
