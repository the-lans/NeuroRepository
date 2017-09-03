#ifndef TEMPCLASS_H
#define TEMPCLASS_H

#include "iobjectecsv.h"
#include "subclass.h"
#include <vector>

enum class EnumColor {ColorNone, ColorRed, ColorBlue};

class TempClass: public IObjectECSV
{
public:
    TempClass();
    double val;
    vector<EnumColor> color;
    vector<SubClass> sub;
public:
    virtual void saveECSV(DataECSV& dt, string& parent); //Сохранение объекта в структуру
    virtual void loadECSV(DataECSV& dt, string& parent); //Загрузка объекта из структуры
};

#endif // TEMPCLASS_H
