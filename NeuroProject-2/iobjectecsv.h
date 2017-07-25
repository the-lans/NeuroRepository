#ifndef IOBJECTECSV_H
#define IOBJECTECSV_H

#include "dataecsv.h"

class IObjectECSV
{
public:
    virtual void saveECSV(DataECSV& dt) = 0;
    virtual void loadECSV(DataECSV& dt) = 0;
};

#endif // IOBJECTECSV_H
