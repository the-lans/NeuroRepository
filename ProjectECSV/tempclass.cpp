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
    string field = parent + ".sub";

    toUInt<EnumColor>(enm_arr, this->color);

    dt.addGroup(parent, "");
    str_val = to_string(this->val); dt.addElement(parent, "val", str_val, typeid(double).name());
    to_array_string(str_vec, enm_arr); dt.addElement(parent, "color", str_vec, typeid(int).name());

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
    string field = parent + ".sub";

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
        size_t ind = dt.getShift();
        iter = dt.modules[ind];
        if(iter->cmpPath(parent, true)) {ind++;}

        //Поля класса
        while(dt.isNextBlock(ind, parent))
        {
            iter = dt.modules[ind];
            if(iter->getFieldValue(parent, "color", str_vec)) {to_array_value(enm_arr, str_vec); convertUInt(this->color, enm_arr); ind++;}
            else if(iter->getFieldValue(parent, "val", str_val)) {to_value(this->val, str_val); ind++;}
            else if(iter->cmpPath(field, true)) //Субполя класса
            {
                dt.setShift(ind);
                sb = new SubClass();
                sb->loadECSV(dt, field);
                this->sub.push_back(*sb);
                ind = dt.getShift();
            }
            else
            {
                ind++;
            }
        }

        dt.setShift(ind);
    }
}
