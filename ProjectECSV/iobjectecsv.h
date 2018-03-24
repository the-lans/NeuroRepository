#ifndef IOBJECTECSV_H
#define IOBJECTECSV_H

#include "dataecsv.h"

class IObjectECSV
{
public:
    vector<string> fieldsNotSave; //Список запрещённых полей для сохранения

public:
    void pushFieldNotSave(string& parent, const string& field); //Добавление запрещённого поля
    virtual void saveECSV(DataECSV& dt, string& parent) = 0; //Сохранение объекта в структуру
    virtual void loadECSV(DataECSV& dt, string& parent) = 0; //Загрузка объекта из структуры
};

#endif // IOBJECTECSV_H
