#include "tempclass.h"

TempClass::TempClass()
{
    this->val = 0;
}

void TempClass::saveECSV(DataECSV& dt, string& parent)
{
    vector<unsigned int> enm_arr;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    string field;

    toUInt<EnumColor>(enm_arr, this->color);

    str_val = to_string(this->val); dt.addElement(parent, "val", str_val, typeid(double).name());
    to_array_string(str_vec, enm_arr); dt.addElement(parent, "color", str_vec, typeid(int).name());

    field = parent + ".sub";
    for(size_t i = 0; i < this->sub.size(); i++)
    {
        this->sub[i].saveECSV(dt, field);
    }
}

void TempClass::loadECSV(DataECSV& dt, string& parent)
{
    SubClass* sb;
    vector<unsigned int> enm_arr;
    StructECSV* iter;
    //unsigned int enm;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    vector<string> subpath;
    string field;
    bool* isLoad = newbool(2, false);

    //Выполнение операций перед загрузкой
    this->color.clear();
    this->sub.clear();

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        field = parent + ".sub";
        sb = new SubClass();
        sb->loadECSV(dt, field);
        this->sub.push_back(*sb);
    }
    else
    {
        //Поля класса
        size_t ind = dt.getShift();
        while(!booland(isLoad, 2) && ind < dt.modules.size())
        {
            iter = dt.modules[ind];
            if(iter->getFieldValue(parent, "color", str_vec)) {to_array_value(enm_arr, str_vec); convertUInt(this->color, enm_arr); isLoad[0] = true;}
            if(iter->getFieldValue(parent, "val", str_val)) {to_value(this->val, str_val); isLoad[1] = true;}
            ind++;
        }

        //Субполя класса
        if(ind >= dt.modules.size()) {ind = dt.getShift();}
        dt.setShift(ind);

        field = parent + ".sub";
        dt.splitPath(subpath, field);
        int ind_sub = 0;

        while(ind < dt.modules.size())
        {
            iter = dt.modules[ind];
            if(iter->cmpPath(subpath))
            {
                sb = new SubClass();
                sb->loadECSV(dt, field);
                this->sub.push_back(*sb);
                ind_sub++;
                ind = dt.getShift();
            }
            else
            {
                ind++; dt.setShift(ind);
            }
        }
    }
}
