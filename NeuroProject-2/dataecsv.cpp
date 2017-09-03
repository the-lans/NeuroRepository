#include "dataecsv.h"

DataECSV::DataECSV()
{
    this->shift = 0;
    this->delim_data = ";";
    this->array_br = false;
}

void DataECSV::setShift(size_t ind)
{
    this->shift = ind;
}

size_t DataECSV::getShift()
{
    return this->shift;
}

void DataECSV::setDelimData(string& delim)
{
    this->delim_data = delim;
}

string DataECSV::getDelimData()
{
    return this->delim_data;
}

void DataECSV::setArrayBR(bool bl)
{
    this->array_br = bl;
}

void DataECSV::beginShift()
{
    this->shift = 0;
}

void DataECSV::endShift()
{
    this->shift = this->modules.size();
}

bool DataECSV::isSpec(string& str)
{
    return (str.length() > 0 && str[0] == '$');
}

string& DataECSV::getSpec(string& str)
{
    str = str.substr(1, str.length()-1);
    return str;
}

bool DataECSV::isDataPath(StructECSV* block)
{
    return (block->mtrx.getLenRow() == 1 && !this->array_br)
            || (block->mtrx.getLenRow() == 1 && block->mtrx.getLenColumn() == 1);
}

vector<string>& DataECSV::splitPath(vector<string>& dest, string& str)
{
    dest.clear();
    split(dest, str, ".");
    if(dest[0] == "") {dest.erase(dest.begin());}
    return dest;
}

StructECSV* DataECSV::addDataMatrix(StructECSV* block, string& str)
{
    if(block == nullptr)
    {
        block = new StructECSV;
    }
    block->addDataMatrix(str, this->delim_data);
    return block;
}

void DataECSV::clear()
{
    StructECSV* block;
    while(!this->modules.empty())
    {
        block = this->modules.back();
        delete block;
        this->modules.pop_back();
    }
    this->beginShift();
}

bool DataECSV::isOneMatrix()
{
    return this->modules.size() == 1 && this->modules[0]->path.size() == 0;
}

bool DataECSV::read(string name)
{
    const char* c_delim_data = this->delim_data.c_str();
    this->clear();
    ifstream in_file(name); //Открываем файл для считывания информации 

    if(in_file.is_open())
    {
        string str, curs, class_curs;
        //int r1, r2;
        vector<string> spec, value_size;
        StructECSV* block = nullptr;

        while(getline(in_file, str))
        {
            spec.clear();
            value_size.clear();
            delcomment(str);
            trimstr(str);

            if(this->isSpec(str)) //Специальная строка
            {
                split(spec, str, "=");
                if(spec.size() > 2) {throw "Many signs \"=\"";}

                //Идентификатор
                class_curs = spec[0];
                this->getSpec(class_curs);
                trimstr(class_curs);
                curs = class_curs;
                str_to_lower(curs);

                //Параметры
                if(curs == "size")
                {
                    split(value_size, spec[1], c_delim_data);
                    if(value_size.size() > 2 || value_size.size() == 0) {throw "Wrong size!";}
                    //r1 = value_size[0];
                    //r2 = (value_size.size() == 2, value_size[1], 1);
                    //block->mtrx.init(0, r1);
                }
                else if(curs == "type")
                {
                    split(block->type, spec[1], c_delim_data);
                    str_to_element(block->type);
                }
                else if(curs == "comment") {} //Комментарий
                else if(curs == "class" && spec.size() == 2) //Путь к данным
                {
                    //Новый блок
                    if(block != nullptr) {this->modules.push_back(block);}
                    block = new StructECSV;

                    class_curs = spec[1];
                    trimstr(class_curs);
                    this->splitPath(block->path, class_curs);
                }
                else if(spec.size() == 1 || spec.size() == 2) //Путь к данным
                {
                    //Новый блок
                    if(block != nullptr) {this->modules.push_back(block);}
                    block = new StructECSV;

                    this->splitPath(block->path, class_curs);
                    if(spec.size() == 2) //Данные
                    {
                        block = this->addDataMatrix(block, spec[1]);
                    }
                }
                else
                {
                    throw "Wrong identy!";
                }
            }
            else if(str != "") //Данные
            {
                block = this->addDataMatrix(block, str);
            }
        }

        if(block != nullptr) {this->modules.push_back(block);}
        in_file.close(); // Закрываем файл
        return true;
    }
    return false;
}

bool DataECSV::write(string name)
{
    string delim_tmp = this->delim_data + " ";
    ofstream out_file(name); //Открываем файл для записи

    if(out_file.is_open())
    {
        StructECSV* block;
        string str;
        bool blData;

        out_file.write("", 0); //Очистка файла
        for(size_t index = 0; index < this->modules.size(); index++)
        {
            block = this->modules[index];
            blData = false;

            //Путь к данным
            if(block->path.size() >= 1)
            {
                str = "$" + block->getPath();
                if(this->isDataPath(block)) //Данные
                {
                    block->getData(str, delim_tmp);
                    blData = true;
                    out_file << str;
                }
                else
                {
                    out_file << str << endl;
                }
            }

            //Тип данных
            if(block->path.size() > 0 && block->type.size() > 0)
            {
                block->getType(str, delim_tmp);
                str = "$Type = " + str;
                out_file << str << endl;
            }

            //Размер данных
            if(block->path.size() > 0 && block->mtrx.getLenRow() > 1)
            {
                str = "$Size = ";
                str += to_string(block->mtrx.getLenColumn());
                str.append(delim_tmp);
                str += to_string(block->mtrx.getLenRow());
                out_file << str << endl;
            }
            else if(block->path.size() > 0 && block->mtrx.getLenColumn() > 1)
            {
                str = "$Size = ";
                str += to_string(block->mtrx.getLenColumn());
                out_file << str << endl;
            }

            //Данные
            if(!blData)
            {
                str = "";
                block->getData(str, delim_tmp);
                blData = true;
                out_file << str;
            }

            if(index < this->modules.size()-1) {out_file << endl;}
        }

        out_file.close(); // Закрываем файл
        return true;
    }
    return false;
}

StructECSV* DataECSV::addElement(string& parent, const string& field, string& value, string& type)
{
    StructECSV* block = new StructECSV;

    if(parent != "") {this->splitPath(block->path, parent);}
    if(field != "") {block->path.push_back(field);}
    if(type != "") {block->type.push_back(type);}
    block->typeECSV.push_back(TypeDataECSV::Element);

    block->mtrx.init(1, 1);
    block->mtrx.set(value, 0, 0);
    this->modules.push_back(block);
    return block;
}

StructECSV* DataECSV::addElement(string& parent, const string& field, vector<string>& value, string& type)
{
    StructECSV* block = new StructECSV;

    if(parent != "") {this->splitPath(block->path, parent);}
    if(field != "") {block->path.push_back(field);}
    if(type != "") {block->type.push_back(type);}
    block->typeECSV.push_back(TypeDataECSV::Element);

    block->mtrx.pushRow(value);
    this->modules.push_back(block);
    return block;
}

StructECSV* DataECSV::addElement(string& parent, const string& field, NArray<string>& value, string& type)
{
    StructECSV* block = new StructECSV;

    if(parent != "") {this->splitPath(block->path, parent);}
    if(field != "") {block->path.push_back(field);}
    if(type != "") {block->type.push_back(type);}
    block->typeECSV.push_back(TypeDataECSV::Element);

    block->mtrx.pushRow(value);
    this->modules.push_back(block);
    return block;
}

StructECSV* DataECSV::addElement(string& parent, const string& field, NMatrix<string>& value, string& type)
{
    StructECSV* block = new StructECSV;

    if(parent != "") {this->splitPath(block->path, parent);}
    if(field != "") {block->path.push_back(field);}
    if(type != "") {block->type.push_back(type);}
    block->typeECSV.push_back(TypeDataECSV::Element);

    block->mtrx = value;
    this->modules.push_back(block);
    return block;
}

StructECSV* DataECSV::addElement(string& parent, const string& field, string& value, const char* type)
{
    string str_type(type);
    return this->addElement(parent, field, value, str_type);
}

StructECSV* DataECSV::addElement(string& parent, const string& field, vector<string>& value, const char* type)
{
    string str_type(type);
    return this->addElement(parent, field, value, str_type);
}

StructECSV* DataECSV::addElement(string& parent, const string& field, NArray<string>& value, const char* type)
{
    string str_type(type);
    return this->addElement(parent, field, value, str_type);
}

StructECSV* DataECSV::addElement(string& parent, const string& field, NMatrix<string>& value, const char* type)
{
    string str_type(type);
    return this->addElement(parent, field, value, str_type);
}

StructECSV* DataECSV::addString(string& parent, const string& field, string& value)
{
    StructECSV* block = new StructECSV;

    if(parent != "") {this->splitPath(block->path, parent);}
    if(field != "") {block->path.push_back(field);}
    //if(type != "") {block->type.push_back(type);}
    block->typeECSV.push_back(TypeDataECSV::String);

    block->mtrx.init(1, 1);
    block->mtrx.set(value, 0, 0);
    this->modules.push_back(block);
    return block;
}

StructECSV* DataECSV::addString(string& parent, const string& field, vector<string>& value)
{
    StructECSV* block = new StructECSV;

    if(parent != "") {this->splitPath(block->path, parent);}
    if(field != "") {block->path.push_back(field);}
    //if(type != "") {block->type.push_back(type);}
    block->typeECSV.push_back(TypeDataECSV::String);

    block->mtrx.pushRow(value);
    this->modules.push_back(block);
    return block;
}

StructECSV* DataECSV::addString(string& parent, const string& field, NArray<string>& value)
{
    StructECSV* block = new StructECSV;

    if(parent != "") {this->splitPath(block->path, parent);}
    if(field != "") {block->path.push_back(field);}
    //if(type != "") {block->type.push_back(type);}
    block->typeECSV.push_back(TypeDataECSV::String);

    block->mtrx.pushRow(value);
    this->modules.push_back(block);
    return block;
}

StructECSV* DataECSV::addString(string& parent, const string& field, NMatrix<string>& value)
{
    StructECSV* block = new StructECSV;

    if(parent != "") {this->splitPath(block->path, parent);}
    if(field != "") {block->path.push_back(field);}
    //if(type != "") {block->type.push_back(type);}
    block->typeECSV.push_back(TypeDataECSV::String);

    block->mtrx = value;
    this->modules.push_back(block);
    return block;
}
