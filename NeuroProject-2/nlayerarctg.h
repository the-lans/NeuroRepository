#ifndef NLAYERARCTG_H
#define NLAYERARCTG_H

#include "inlayer.h"
#include <math.h>

template <typename NType>
class NLayerArctg: public INLayer<NType>
{
public:
    NLayerArctg();
    virtual ~NLayerArctg();
public:
    virtual NType activation(NType& x);
    virtual NType derivative(NType& y);
};


template <typename NType>
NLayerArctg<NType>::NLayerArctg()
{
    this->type = NLayerType::NFuncArctg;
    this->koef = 1;
    this->typeDerivat = NLayerDerivat::NDerivatSum;
}

template <typename NType>
NLayerArctg<NType>::~NLayerArctg()
{
}

template <typename NType>
NType NLayerArctg<NType>::activation(NType& x)
{
    return atan(this->koef * x);
}

template <typename NType>
NType NLayerArctg<NType>::derivative(NType& y)
{
    NType val = this->koef * y;
    return this->koef / (val * val + 1);
}

#endif // NLAYERARCTG_H
