#ifndef NLAYERLINEAR_H
#define NLAYERLINEAR_H

#include "inlayer.h"

template <typename NType>
class NLayerLinear: public INLayer<NType>
{
public:
    NLayerLinear();
    virtual ~NLayerLinear();
public:
    virtual NType activation(NType& x);
    virtual NType derivative(NType& y);
};


template <typename NType>
NLayerLinear<NType>::NLayerLinear()
{
    this->type = NLayerType::NFuncLinear;
    this->koef = 1;
}

template <typename NType>
NLayerLinear<NType>::~NLayerLinear()
{
}

template <typename NType>
NType NLayerLinear<NType>::activation(NType& x)
{
    return this->koef * x;
}

template <typename NType>
NType NLayerLinear<NType>::derivative(NType& y)
{
    return this->koef;
}

#endif // NLAYERLINEAR_H
