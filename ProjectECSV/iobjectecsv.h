#ifndef IOBJECTECSV_H
#define IOBJECTECSV_H

#include "dataecsv.h"

class IObjectECSV
{
public:
    virtual void saveECSV(DataECSV& dt, string& parent) = 0; //Сохранение объекта в структуру
    virtual void loadECSV(DataECSV& dt, string& parent) = 0; //Загрузка объекта из структуры
};

#endif // IOBJECTECSV_H
