#ifndef NEXAMPLE_H
#define NEXAMPLE_H

#include "narray.h"
#include "iobjectecsv.h"
#include "dataecsv.h"

enum NSetType {NSetNone, NSetTrain, NSetTest};

template <typename NType>
class NExample: public IObjectECSV
{
public:
    NExample();
    NExample(NExample<NType>& obj);
    virtual ~NExample();
public:
    NArray<NType> input; //Входной вектор
    NArray<NType> output; //Выходной эталонный вектор
    NArray<NType> outrun; //Выходной расчётный вектор
protected:
    NType energy; //Энергия примера - разница между расчётным и эталонным вектором
public:
    void setEnergy(NType energy);
    NType getEnergy();
public:
    void deinit(); //Деинициализация
    void init(int lenIn, int lenOut, NType value); //Инициализация массивов значением
public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NExample<NType>::NExample()
{
    this->energy = 0;
}

template <typename NType>
NExample<NType>::NExample(NExample<NType>& obj):
    input(obj.input), output(obj.output), outrun(obj.outrun)
{
    this->energy = obj.getEnergy();
}

template <typename NType>
NExample<NType>::~NExample()
{
    this->deinit();
}

template <typename NType>
void NExample<NType>::setEnergy(NType energy)
{
    this->energy = energy;
}

template <typename NType>
NType NExample<NType>::getEnergy()
{
    return this->energy;
}

template <typename NType>
void NExample<NType>::deinit()
{
    this->input.clear();
    this->output.clear();
    this->outrun.clear();
}

template <typename NType>
void NExample<NType>::init(int lenIn, int lenOut, NType value)
{
    this->input.init(lenIn, value);
    this->output.init(lenOut, value);
    this->outrun.clear();
}

template <typename NType>
void NExample<NType>::saveECSV(DataECSV& dt, string& parent)
{
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;

    to_array_string(str_vec, this->input); dt.addElement(parent, "input", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->output); dt.addElement(parent, "output", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->outrun); dt.addElement(parent, "outrun", str_vec, typeid(NType).name());
    str_val = to_string(this->energy); dt.addElement(parent, "energy", str_val, typeid(NType).name());
}

template <typename NType>
void NExample<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    //unsigned int enm;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;
    bool* isLoad = newbool(4, false);

    this->deinit();

    if(dt.isOneMatrix())
    {
        iter = dt.modules[0];
        iter->getMatrixVector(str_vec); to_array_value(this->input, str_vec);
    }
    else
    {
        size_t ind = dt.getShift();
        while(!booland(isLoad, 4) && ind < dt.modules.size())
        {
            iter = dt.modules[ind];
            if(iter->getFieldValue(parent, "input", str_vec)) {to_array_value(this->input, str_vec); isLoad[0] = true;}
            if(iter->getFieldValue(parent, "output", str_vec)) {to_array_value(this->output, str_vec); isLoad[1] = true;}
            if(iter->getFieldValue(parent, "outrun", str_vec)) {to_array_value(this->outrun, str_vec); isLoad[2] = true;}
            if(iter->getFieldValue(parent, "energy", str_val)) {to_value(this->energy, str_val); isLoad[3] = true;}
            ind++;
        }
        dt.setShift(ind);
    }
}

#endif // NEXAMPLE_H
