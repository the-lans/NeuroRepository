#ifndef NLAYERTANH_H
#define NLAYERTANH_H

#include "inlayer.h"
#include <math.h>

template <typename NType>
class NLayerTanh: public INLayer<NType>
{
public:
    NLayerTanh();
    virtual ~NLayerTanh();
public:
    virtual NType activation(NType& x);
    virtual NType derivative(NType& y);
};


template <typename NType>
NLayerTanh<NType>::NLayerTanh()
{
    this->type = NLayerType::NFuncTanh;
    this->koef = 2;
}

template <typename NType>
NLayerTanh<NType>::~NLayerTanh()
{
}

template <typename NType>
NType NLayerTanh<NType>::activation(NType& x)
{
    return tanh(this->koef * x);
}

template <typename NType>
NType NLayerTanh<NType>::derivative(NType& y)
{
    return (this->koef * (1 - y*y));
}

#endif // NLAYERTANH_H
