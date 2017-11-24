#include "subclass.h"

SubClass::SubClass()
{
    this->val_a = 0;
    this->val_b = 1;
}

void SubClass::saveECSV(DataECSV& dt, string& parent)
{
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;

    dt.addGroup(parent, "");
    to_array_string(str_vec, this->vec); dt.addElement(parent, "vec", str_vec, typeid(int).name());
    str_val = to_string(this->val_a); dt.addElement(parent, "val_a", str_val, typeid(int).name());
    str_val = to_string(this->val_b); dt.addElement(parent, "val_b", str_val, typeid(double).name());
}

void SubClass::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    //unsigned int enm;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;

    //Выполнение операций перед загрузкой
    this->vec.clear();

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        to_matrix_value(this->vec, iter->mtrx);
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
            if(iter->getFieldValue(parent, "vec", str_vec)) {to_array_value(this->vec, str_vec);}
            else if(iter->getFieldValue(parent, "val_a", str_val)) {to_value(this->val_a, str_val);}
            else if(iter->getFieldValue(parent, "val_b", str_val)) {to_value(this->val_b, str_val);}
            ind++;
        }

        dt.setShift(ind);
    }
}
