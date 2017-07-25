#include "nlayertanh.h"
#include <math.h>

template <typename NType>
NLayerTanh<NType>::NLayerTanh()
{
    this->type = NLayerType::NFuncTanh;
    this->typeDerivat = NLayerDerivat::NDerivatOut;
    this->koef = 1;
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
