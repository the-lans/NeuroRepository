#include "structecsv.h"


StructECSV::StructECSV()
{
    this->group = false;
}

TypeDataECSV StructECSV::getTypeECSV(int ind)
{
    int len = this->typeECSV.size();
    if(len > ind)
    {
        return this->typeECSV[ind];
    }
    else if(len >= 1)
    {
        return this->typeECSV[len-1];
    }
    else
    {
        return TypeDataECSV::Element;
    }
}

string& StructECSV::getData(string& data, const char* delim)
{
    if(data.length() > 0) {data.append(" = ");}
    string str, elem;
    string* vec;
    size_t len_delim = strlen(delim);

    for(int jnd = 0; jnd < this->mtrx.getLenRow(); jnd++)
    {
        vec = this->mtrx.at(jnd);
        //concat(str, vec, this->mtrx.getLenColumn(), delim);

        str = "";
        for(int ind = 0; ind < this->mtrx.getLenColumn(); ind++)
        {
            shield(elem, vec[ind]);
            if(this->getTypeECSV(ind) == TypeDataECSV::Element)
            {
                str.append(elem);
            }
            else
            {
                str.append("\"");
                str.append(elem);
                str.append("\"");
            }
            str.append(delim);
        }
        if(this->mtrx.getLenColumn() > 0) {str.erase(str.length()-len_delim, len_delim);}

        data.append(str);
        data.append("\n");
    }

    return data;
}

string& StructECSV::getData(string& data, string& delim)
{
    const char* c_delim = delim.c_str();
    return this->getData(data, c_delim);
}

string& StructECSV::getType(string& str, const char* delim)
{
    concat(str, this->type, delim);
    return str;
}

string& StructECSV::getType(string& str, string& delim)
{
    const char* c_delim = delim.c_str();
    return this->getType(str, c_delim);
}

string StructECSV::getPath()
{
    string str = "";
    concat(str, this->path, ".");
    str = "." + str;
    return str;
}

bool StructECSV::cmpPath(string& str, bool acc)
{
    vector<string> subpath;
    split(subpath, str, ".");
    if(subpath.size() > 0 && subpath[0] == "") {subpath.erase(subpath.begin());}
    return this->cmpPath(subpath, acc);
}

bool StructECSV::cmpPath(vector<string>& subpath, bool acc)
{
    bool bl = (!acc || this->path.size() == subpath.size());
    for(size_t i = 0; bl == true && i < subpath.size(); i++)
    {
        if(subpath[i] != this->path[i]) {bl = false;}
    }
    return bl;
}

vector<string>& StructECSV::splitPath(string& str)
{
    this->path.clear();
    split(this->path, str, ".");
    if(this->path[0] == "") {this->path.erase(this->path.begin());}
    return this->path;
}

bool StructECSV::isFieldObj(string& parent)
{
    bool bl = true;
    if(parent != "")
    {
        vector<string> path_parent;
        split(path_parent, parent, ".");
        if(path_parent.size() > 0 && path_parent[0] == "") {path_parent.erase(path_parent.begin());}
        bl = (this->path.size() > path_parent.size());
        if(bl) {bl = this->cmpPath(path_parent, false);}
    }
    return bl;
}

void StructECSV::addDataMatrix(string& str, const char* delim)
{
    vector<string> data;
    split_no_slash(data, str, delim);
    this->typeECSV = str_to_element(data);
    freeshield(data);
    this->mtrx.pushRow(data);
}

void StructECSV::addDataMatrix(string& str, string& delim)
{
    const char* c_delim = delim.c_str();
    this->addDataMatrix(str, c_delim);
}

void StructECSV::clear()
{
    this->path.clear();
    this->type.clear();
    this->mtrx.clear();
    this->typeECSV.clear();
}

string& StructECSV::getMatrixValue(string& value)
{
    value = this->mtrx.get(0, 0);
    return value;
}

string StructECSV::getMatrixValue()
{
    return this->mtrx.get(0, 0);
}

vector<string>& StructECSV::getMatrixVector(vector<string>& value)
{
    //this->mtrx.popRow(value);
    this->mtrx.getRow(0, value);
    return value;
}

NArray<string>& StructECSV::getMatrixVector(NArray<string>& value)
{
    //this->mtrx.popRow(value);
    this->mtrx.getRow(0, value);
    return value;
}

bool StructECSV::isField(string& parent, const string& field)
{
    if(parent[0] != '.') {parent = "" + parent;}
    return this->getPath() == (parent + "." + field);
}

bool StructECSV::getFieldValue(string& parent, const string& field, NMatrix<string>& value)
{
    bool bl = this->isField(parent, field);
    if(bl) {value = this->mtrx;}
    return bl;
}

bool StructECSV::getFieldValue(string& parent, const string& field, vector<string>& value)
{
    bool bl = this->isField(parent, field);
    if(bl) {this->getMatrixVector(value);}
    return bl;
}

bool StructECSV::getFieldValue(string& parent, const string& field, NArray<string>& value)
{
    bool bl = this->isField(parent, field);
    if(bl) {this->getMatrixVector(value);}
    return bl;
}

bool StructECSV::getFieldValue(string& parent, const string& field, string& value)
{
    bool bl = this->isField(parent, field);
    if(bl) {this->getMatrixValue(value);}
    return bl;
}
