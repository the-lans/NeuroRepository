#ifndef INLAYER_H
#define INLAYER_H

#include "narray.h"
#include "nmatrix.h"
#include <iostream>
#include <ctime>
#include <math.h>

enum class NLayerType {NFuncNone, NFuncTanh, NFuncSoftsign, NFuncArctg, NFuncLinear};
enum class NLayerDerivat {NDerivatSum, NDerivatOut};

template <typename NType>
class INLayer
{
public:
    INLayer();
    INLayer(INLayer<NType>& obj);
    INLayer<NType>& operator=(INLayer<NType>& obj);
    virtual ~INLayer();
public:
    NMatrix<NType> weigth;
    NArray<NType> bias;
    NArray<NType> output;
    NArray<NType> sum;
protected:
    NType koef;
    NLayerType type;
    NType energyRegularization;
    NLayerDerivat typeDerivat;
public:
    void setKoef(NType koef);
    NType getKoef();
    NLayerType getType();
    NType getEnergyRegularization();
    NLayerDerivat getTypeDerivat();
public:
    virtual void init(NType value);
    virtual void init(int lenRow, int lenColumn, NType value);
    virtual void ginit(NType value);
    virtual void ginit(int lenRow, int lenColumn, NType value);
    virtual NType activation(NType& x) = 0;
    virtual NType derivative(NType& y) = 0;
    virtual NArray<NType>* run(NArray<NType>* X);
    virtual NType funcRegularization();
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

#endif // INLAYER_H
