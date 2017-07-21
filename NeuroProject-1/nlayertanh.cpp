#include "nlayertanh.h"
#include <math.h>

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
    NType val = exp(this->koef * x);
    return (val - 1)/(val + 1);
}

template <typename NType>
NType NLayerTanh<NType>::derivative(NType& y)
{
    return (0.5 * this->koef * (1 - y*y));
}
