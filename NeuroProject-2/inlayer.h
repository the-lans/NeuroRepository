#ifndef INLAYER_H
#define INLAYER_H

#include "narray.h"
#include "nmatrix.h"
#include "iobjectecsv.h"
#include <iostream>
#include <ctime>
#include <math.h>

enum class NLayerType {NFuncNone, NFuncTanh, NFuncSoftsign, NFuncArctg, NFuncLinear};
enum class NLayerDerivat {NDerivatSum, NDerivatOut};

template <typename NType>
class INLayer: public IObjectECSV
{
public:
    INLayer();
    INLayer(INLayer<NType>& obj);
    INLayer<NType>& operator=(INLayer<NType>& obj);
    virtual ~INLayer();
public:
    NMatrix<NType> weigth; //Матрица весов
    NArray<NType> bias; //Вектор смещений
    NArray<NType> output; //Расчётный выход слоя
    NArray<NType> sum; //Промежуточные значения сумм
protected:
    NType koef; //Коэффициент функции активации
    NLayerType type; //Тип слоя
    NType energyRegularization; //Энергия регуляризации слоя
    NLayerDerivat typeDerivat; //Тип аргумента производной
public:
    void setKoef(NType koef);
    NType getKoef();
    NLayerType getType();
    NType getEnergyRegularization();
    NLayerDerivat getTypeDerivat();
public:
    virtual void deinit(); //Деинициализация
    virtual void init(NType value); //Инициализация значением
    virtual void init(int lenRow, int lenColumn, NType value); //Инициализация значением
    virtual void ginit(NType value); //Инициализация Нгуен-Видроу
    virtual void ginit(int lenRow, int lenColumn, NType value); //Инициализация Нгуен-Видроу
    virtual NType activation(NType& x) = 0; //Функция активации нейрона
    virtual NType derivative(NType& y) = 0; //Производная функции активации
    virtual NArray<NType>* run(NArray<NType>* X); //Функционирование слоя
    virtual NType funcRegularization(); //Функция регуляризации слоя
public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
INLayer<NType>::INLayer()
{
    this->type = NLayerType::NFuncNone;
    this->koef = 1;
    this->energyRegularization = 0;
    this->typeDerivat = NLayerDerivat::NDerivatOut;
}

template <typename NType>
INLayer<NType>::INLayer(INLayer<NType>& obj):
    weigth(obj.weigth), bias(obj.bias), output(obj.output), sum(obj.sum)
{
    this->type = obj.getType();
    this->koef = obj.getKoef();
    this->energyRegularization = obj.getEnergyRegularization();
    this->typeDerivat = obj.getTypeDerivat();
}

template <typename NType>
INLayer<NType>& INLayer<NType>::operator=(INLayer<NType>& obj)
{
    this->weigth = obj.weigth;
    this->bias = obj.bias;
    this->output = obj.output;
    this->sum = obj.sum;

    this->type = obj.getType();
    this->koef = obj.getKoef();
    this->energyRegularization = obj.getEnergyRegularization();
    this->typeDerivat = obj.getTypeDerivat();

    return *this;
}

template <typename NType>
INLayer<NType>::~INLayer()
{
    this->deinit();
}

template <typename NType>
void INLayer<NType>::setKoef(NType koef)
{
    this->koef = koef;
}

template <typename NType>
NType INLayer<NType>::getKoef()
{
    return this->koef;
}

template <typename NType>
NLayerType INLayer<NType>::getType()
{
    return this->type;
}

template <typename NType>
NType INLayer<NType>::getEnergyRegularization()
{
    return this->energyRegularization;
}

template <typename NType>
NLayerDerivat INLayer<NType>::getTypeDerivat()
{
    return this->typeDerivat;
}

template <typename NType>
void INLayer<NType>::deinit()
{
    this->weigth.clear();
    this->bias.clear();
    this->output.clear();
    this->sum.clear();
}

template <typename NType>
void INLayer<NType>::init(NType value)
{
    int lenRow = this->weigth.getLenRow();
    int lenColumn = this->weigth.getLenColumn();
    this->init(lenRow, lenColumn, value);
}

template <typename NType>
void INLayer<NType>::init(int lenRow, int lenColumn, NType value)
{
    this->weigth.init(lenRow, lenColumn, value);
    this->bias.init(lenColumn, value);
    this->output.init(lenColumn, value);
    this->sum.init(lenColumn, value);
}

template <typename NType>
void INLayer<NType>::ginit(NType value)
{
    int lenRow = this->weigth.getLenRow();
    int lenColumn = this->weigth.getLenColumn();
    this->ginit(lenRow, lenColumn, value);
}

template <typename NType>
void INLayer<NType>::ginit(int lenRow, int lenColumn, NType value)
{
    int i, j;
    this->init(lenRow, lenColumn, 0);
    NType beta = 0.7 * pow(lenColumn, 1/lenRow);

    NType ran, S;
    for(j = 0; j < lenColumn; j++)
    {
        S = 0;
        for(i = 0; i < lenRow; i++)
        {
            ran = 2 * value * ((NType)rand()/(NType)RAND_MAX) - value;
            this->weigth.set(ran, i, j);
            S += ran * ran;
        }

        S = sqrt(S);
        for(i = 0; i < lenRow; i++)
        {
            ran = this->weigth[i][j];
            ran = beta * ran / S;
            this->weigth.set(ran, i, j);
        }
        ran = 2 * beta * ((NType)rand()/(NType)RAND_MAX) - beta;
        this->bias.set(ran, j);
    }
}

template <typename NType>
NArray<NType>* INLayer<NType>::run(NArray<NType>* X)
{
    this->sum.mul(*X, this->weigth, true);
    this->sum.sum(this->bias);

    NType* pOut = this->output.getData();
    NType* pSum = this->sum.getData();
    for(int i = 0; i < this->output.getLength(); i++)
    {
        pOut[i] = this->activation(pSum[i]);
    }

    return &(this->output);
}

template <typename NType>
NType INLayer<NType>::funcRegularization()
{
    int i, j;
    this->energyRegularization = 0;
    NType* pData = this->weigth.getData();
    int lenRow = this->weigth.getLenRow();
    int lenColumn = this->weigth.getLenColumn();

    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            this->energyRegularization += pData[i*lenColumn+j] * pData[i*lenColumn+j];
        }
    }

    this->energyRegularization *= 0.5;
    return this->energyRegularization;
}

template <typename NType>
void INLayer<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;

    to_matrix_string(str_mtrx, this->weigth); dt.addElement(parent, "weigth", str_mtrx, typeid(NType).name());

    to_array_string(str_vec, this->bias); dt.addElement(parent, "bias", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->output); dt.addElement(parent, "output", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->sum); dt.addElement(parent, "sum", str_vec, typeid(NType).name());

    str_val = to_string(this->koef); dt.addElement(parent, "koef", str_val, typeid(NType).name());
    str_val = to_string(this->energyRegularization); dt.addElement(parent, "energyRegularization", str_val, typeid(NType).name());
    str_val = to_string((unsigned int)this->type); dt.addElement(parent, "type", str_val, typeid(int).name());
    str_val = to_string((unsigned int)this->typeDerivat); dt.addElement(parent, "typeDerivat", str_val, typeid(int).name());
}

template <typename NType>
void INLayer<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    unsigned int enm;
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;
    bool* isLoad = newbool(8, false);

    INLayer<NType>::deinit();

    if(dt.isOneMatrix())
    {
        iter = dt.modules[0];
        to_matrix_value(this->weigth, iter->mtrx);
    }
    else
    {
        size_t ind = dt.getShift();
        while(!booland(isLoad, 8) && ind < dt.modules.size())
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "weigth", str_mtrx)) {to_matrix_value(this->weigth, str_mtrx); isLoad[0] = true;}

            if(iter->getFieldValue(parent, "bias", str_vec)) {to_array_value(this->bias, str_vec); isLoad[1] = true;}
            if(iter->getFieldValue(parent, "output", str_vec)) {to_array_value(this->output, str_vec); isLoad[2] = true;}
            if(iter->getFieldValue(parent, "sum", str_vec)) {to_array_value(this->sum, str_vec); isLoad[3] = true;}

            if(iter->getFieldValue(parent, "koef", str_val)) {to_value(this->koef, str_val); isLoad[4] = true;}
            if(iter->getFieldValue(parent, "energyRegularization", str_val)) {to_value(this->energyRegularization, str_val); isLoad[5] = true;}
            if(iter->getFieldValue(parent, "type", str_val)) {to_value(enm, str_val); this->type = (NLayerType)enm; isLoad[6] = true;}
            if(iter->getFieldValue(parent, "typeDerivat", str_val)) {to_value(enm, str_val); this->typeDerivat = (NLayerDerivat)enm; isLoad[7] = true;}

            ind++;
        }
        dt.setShift(ind);
    }
}

#endif // INLAYER_H
