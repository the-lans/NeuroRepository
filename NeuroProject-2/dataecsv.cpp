#include "dataecsv.h"

DataECSV::DataECSV()
{
    this->fieldsNotSave = nullptr;
    this->shift = 0;
    this->delim_data = ";";
    this->array_br = true;
}

void DataECSV::setShift(size_t ind)
{
    this->shift = ind;
}

size_t DataECSV::getShift()
{
    return this->shift;
}

size_t DataECSV::getIndexSize()
{
    return this->indexSize;
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

bool DataECSV::getGroup(string& str)
{
    if(str[0] == '[' && str[str.length()-1] == ']')
    {
        str = str.substr(1, str.length()-2);
        return true;
    }
    return false;
}

bool DataECSV::isDataPath(StructECSV* block)
{
    return (block->mtrx.getLenRow() == 1 && !this->array_br)
            || (block->mtrx.getLenRow() == 1 && block->mtrx.getLenColumn() == 1);
}

bool DataECSV::isNextBlock(size_t ind, string& parent)
{
    return ind < this->modules.size() && this->modules[ind]->isFieldObj(parent);
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
    this->clearIndex();
    this->clearData();
}

void DataECSV::clearData()
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

void DataECSV::clearIndex()
{
    this->indexModule.clear();
    this->indexString.clear();
}

bool DataECSV::isOneMatrix()
{
    return this->modules.size() == 1 && this->modules[0]->path.size() == 0;
}

void DataECSV::seek_back(fstream& sm, long skback)
{
    sm.seekg(skback, ios_base::beg);
}

void DataECSV::shiftIndexString(fstream& index_file)
{
    index_file.seekp(0, ios_base::end);
    long fsize = index_file.tellp();

    for(size_t i = 0; i < this->indexString.size(); i++)
    {
        this->indexString[i] += fsize;
    }
}

bool DataECSV::read(string name)
{
    this->clear();
    fstream in_file; //Открываем файл для считывания информации
    in_file.open(name, ios_base::in | ios_base::binary);

    if(in_file.is_open())
    {
        in_file.seekg (0, ios_base::end);
        long fsize = in_file.tellg();
        in_file.seekg (0, ios_base::beg);

        this->readHead(in_file); //Шапка файла
        this->readBody(in_file, fsize); //Чтение
        in_file.close(); // Закрываем файл
        return true;
    }
    return false;
}

bool DataECSV::write(string name)
{
    fstream out_file; //Открываем файл для записи
    out_file.open(name, ios_base::out | ios_base::binary);

    if(out_file.is_open())
    {
        this->writeHead(out_file); //Шапка файла
        this->writeBody(out_file); //Запись
        out_file.close(); // Закрываем файл
        return true;
    }
    return false;
}

void DataECSV::readBody(fstream& in_file, long seekstop)
{
    const char* c_delim_data = this->delim_data.c_str();
    string str, curs, class_curs;
    //int r1, r2;
    vector<string> spec, value_size;
    StructECSV* block = nullptr;
    long seek = in_file.tellg();

    while(getline(in_file, str) && seek < seekstop)
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
                block->group = this->getGroup(class_curs);
                block->splitPath(class_curs);
            }
            else if(spec.size() == 1 || spec.size() == 2) //Путь к данным
            {
                //Новый блок
                if(block != nullptr) {this->modules.push_back(block);}
                block = new StructECSV;

                block->group = this->getGroup(class_curs);
                block->splitPath(class_curs);
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

        seek = in_file.tellg();
    }

    if(block != nullptr) {this->modules.push_back(block);}
}

void DataECSV::writeBody(fstream& out_file)
{
    string delim_tmp = this->delim_data + " ";
    StructECSV* block;
    string str;
    bool blData;

    out_file.seekp(0, ios_base::end);
    long fsize = out_file.tellp();
    if(fsize > 0) {out_file << endl;}

    for(size_t index = 0; index < this->modules.size(); index++)
    {
        block = this->modules[index];
        blData = false;

        //Путь к данным
        if(block->path.size() >= 1)
        {
            if(block->group) {str = "$[" + block->getPath() + "]";}
            else {str = "$" + block->getPath();}

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
}

bool DataECSV::readHead(string name)
{
    fstream in_file; //Открываем файл для считывания информации
    in_file.open(name, ios_base::in | ios_base::binary);

    if(in_file.is_open())
    {
        this->readHead(in_file);
        in_file.close(); // Закрываем файл
        return true;
    }
    return false;
}

void DataECSV::readHead(fstream& in_file)
{
    const char* c_delim_data = this->delim_data.c_str();
    string str = "", curs, class_curs;
    vector<string> spec, vec_str;
    long skback = 0;
    bool blExit = true;

    in_file.seekg(0, ios_base::beg);

    while(blExit && getline(in_file, str))
    {
        spec.clear();
        vec_str.clear();
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
            if(curs == "indexmodule")
            {
                split(vec_str, spec[1], c_delim_data);
                to_array_value(this->indexModule, vec_str);
            }
            else if(curs == "indexstring")
            {
                split(vec_str, spec[1], c_delim_data);
                to_array_value(this->indexString, vec_str);
            }
            else if(curs == "comment") {} //Комментарий
            else {blExit = false;}
        }
        else if(str != "") {blExit = false;} //Данные

        if(blExit) {skback = in_file.tellg();}
    }

    if(in_file.eof())
    {
        in_file.seekg(0, ios_base::end);
        skback = in_file.tellg();
    }
    else
    {
        this->seek_back(in_file, skback);
    }
    this->indexSize = skback;
}

bool DataECSV::writeHead(string name)
{
    fstream out_file; //Открываем файл для записи
    out_file.open(name, ios_base::out | ios_base::trunc | ios_base::binary);

    if(out_file.is_open())
    {
        this->writeHead(out_file);
        out_file.close(); // Закрываем файл
        return true;
    }
    return false;
}

void DataECSV::writeHead(fstream& out_file)
{
    string delim_tmp = this->delim_data + " ";
    vector<string> vec_str;
    string str;
    bool isHead = false;

    //Очистка файла
    //out_file.close();
    //out_file.open(out_file, ios_base::out | ios_base::trunc | ios_base::binary);

    if(this->indexModule.size() > 0)
    {
        to_array_string(vec_str, this->indexModule);
        concat(str, vec_str, delim_tmp.c_str());
        str = "$IndexModule = " + str;
        out_file << str << endl;
        isHead = true;
    }

    if(this->indexString.size() > 0)
    {
        to_array_string(vec_str, this->indexString);
        concat(str, vec_str, delim_tmp.c_str());
        str = "$IndexString = " + str;
        out_file << str << endl;
        isHead = true;
    }

    if(isHead) {out_file << endl;}

    out_file.seekp(0, ios_base::end);
    this->indexSize = out_file.tellp();
}

bool DataECSV::readObj(string name, size_t num)
{
    fstream in_file; //Открываем файл для считывания информации
    in_file.open(name, ios_base::in | ios_base::binary);

    if(in_file.is_open())
    {
        //this->readHead(in_file);
        this->readObj(in_file, num);
        in_file.close(); // Закрываем файл
        return true;
    }
    return false;
}

void DataECSV::readObj(fstream& in_file, size_t num)
{
    this->seekr(in_file, num);
    this->readBody(in_file, this->indexString[num]);
}

bool DataECSV::appendObj(string name)
{
    fstream out_file; //Открываем файл для записи
    out_file.open(name, ios_base::out | ios_base::app | ios_base::binary);

    if(out_file.is_open())
    {
        this->appendObj(out_file); //Запись
        out_file.close(); // Закрываем файл
        return true;
    }
    return false;
}

void DataECSV::appendObj(fstream& out_file)
{
    out_file.seekp(0, ios_base::end);
    this->seekw(out_file, false);
    this->writeBody(out_file);
}

void DataECSV::seekr(fstream& in_file, size_t ind)
{
    if(ind == 0) {in_file.seekg(0, ios_base::beg);}
    else {in_file.seekg(this->indexString[ind-1]);}
}

void DataECSV::seekw(fstream& out_file, bool seq)
{
    if(seq || this->indexModule.size() == 0)
    {
        this->indexModule.push_back(this->modules.size());
    }
    else
    {
        this->indexModule.push_back(*(this->indexModule.rbegin()) + this->modules.size());
    }

    long seek = out_file.tellp();
    this->indexString.push_back(seek);
}

bool DataECSV::unionHeadBody(string index_name, string body_name)
{
    fstream index_file, body_file;
    index_file.open(index_name, ios_base::out | ios_base::app | ios_base::binary);
    body_file.open(body_name, ios_base::in | ios_base::binary);

    if(index_file.is_open() && body_file.is_open())
    {
        this->unionHeadBody(index_file, body_file);
        index_file.close();
        body_file.close();
        return true;
    }
    return false;
}

void DataECSV::unionHeadBody(fstream& index_file, fstream& body_file)
{
    int length = 512;
    char* buffer = new char[length];
    index_file.seekp(0, ios_base::end);
    body_file.seekg(0, ios_base::beg);

    while(!body_file.eof())
    {
        body_file.read(buffer, length);
        index_file.write(buffer, body_file.gcount());
    }
    index_file.flush();

    delete[] buffer;
}


StructECSV* DataECSV::addElement(string& parent, const string& field, string& value, string& type)
{
    StructECSV* block = nullptr;

    if(resolvedfield(this->fieldsNotSave, parent, field))
    {
        block = new StructECSV;
        if(parent != "") {block->splitPath(parent);}
        if(field != "") {block->path.push_back(field);}
        if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::Element);

        block->mtrx.init(1, 1);
        block->mtrx.set(value, 0, 0);
        this->modules.push_back(block);
    }

    return block;
}

StructECSV* DataECSV::addElement(string& parent, const string& field, vector<string>& value, string& type)
{
    StructECSV* block = nullptr;

    if(resolvedfield(this->fieldsNotSave, parent, field))
    {
        block = new StructECSV;
        if(parent != "") {block->splitPath(parent);}
        if(field != "") {block->path.push_back(field);}
        if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::Element);

        block->mtrx.pushRow(value);
        this->modules.push_back(block);
    }

    return block;
}

StructECSV* DataECSV::addElement(string& parent, const string& field, NArray<string>& value, string& type)
{
    StructECSV* block = nullptr;

    if(resolvedfield(this->fieldsNotSave, parent, field))
    {
        block = new StructECSV;
        if(parent != "") {block->splitPath(parent);}
        if(field != "") {block->path.push_back(field);}
        if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::Element);

        block->mtrx.pushRow(value);
        this->modules.push_back(block);
    }

    return block;
}

StructECSV* DataECSV::addElement(string& parent, const string& field, NMatrix<string>& value, string& type)
{
    StructECSV* block = nullptr;

    if(resolvedfield(this->fieldsNotSave, parent, field))
    {
        block = new StructECSV;
        if(parent != "") {block->splitPath(parent);}
        if(field != "") {block->path.push_back(field);}
        if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::Element);

        block->mtrx = value;
        this->modules.push_back(block);
    }

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
    StructECSV* block = nullptr;

    if(resolvedfield(this->fieldsNotSave, parent, field))
    {
        block = new StructECSV;
        if(parent != "") {block->splitPath(parent);}
        if(field != "") {block->path.push_back(field);}
        //if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::String);

        block->mtrx.init(1, 1);
        block->mtrx.set(value, 0, 0);
        this->modules.push_back(block);
    }

    return block;
}

StructECSV* DataECSV::addString(string& parent, const string& field, vector<string>& value)
{
    StructECSV* block = nullptr;

    if(resolvedfield(this->fieldsNotSave, parent, field))
    {
        block = new StructECSV;
        if(parent != "") {block->splitPath(parent);}
        if(field != "") {block->path.push_back(field);}
        //if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::String);

        block->mtrx.pushRow(value);
        this->modules.push_back(block);
    }

    return block;
}

StructECSV* DataECSV::addString(string& parent, const string& field, NArray<string>& value)
{
    StructECSV* block = nullptr;

    if(resolvedfield(this->fieldsNotSave, parent, field))
    {
        block = new StructECSV;
        if(parent != "") {block->splitPath(parent);}
        if(field != "") {block->path.push_back(field);}
        //if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::String);

        block->mtrx.pushRow(value);
        this->modules.push_back(block);
    }

    return block;
}

StructECSV* DataECSV::addString(string& parent, const string& field, NMatrix<string>& value)
{
    StructECSV* block = nullptr;

    if(resolvedfield(this->fieldsNotSave, parent, field))
    {
        block = new StructECSV;
        if(parent != "") {block->splitPath(parent);}
        if(field != "") {block->path.push_back(field);}
        //if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::String);

        block->mtrx = value;
        this->modules.push_back(block);
    }

    return block;
}

StructECSV* DataECSV::addGroup(string& parent, string& type)
{
    StructECSV* block = nullptr;
    if(parent != "")
    {
        block = new StructECSV;

        block->splitPath(parent);
        if(type != "") {block->type.push_back(type);}
        block->typeECSV.push_back(TypeDataECSV::Element);
        block->group = true;

        this->modules.push_back(block);
    }

    return block;
}

StructECSV* DataECSV::addGroup(string& parent, const char* type)
{
    string str_type(type);
    return this->addGroup(parent, str_type);
}
