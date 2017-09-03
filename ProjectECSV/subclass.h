#ifndef SUBCLASS_H
#define SUBCLASS_H

#include "iobjectecsv.h"
#include <vector>

class SubClass: public IObjectECSV
{
public:
    SubClass();
    int val_a;
    double val_b;
    vector<int> vec;
public:
    virtual void saveECSV(DataECSV& dt, string& parent); //Сохранение объекта в структуру
    virtual void loadECSV(DataECSV& dt, string& parent); //Загрузка объекта из структуры
};

#endif // SUBCLASS_H
