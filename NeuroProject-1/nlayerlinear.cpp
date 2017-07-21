#include "nlayerlinear.h"

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
