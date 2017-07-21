#include "nlayerarctg.h"
#include <math.h>

template <typename NType>
NLayerArctg<NType>::NLayerArctg()
{
    this->type = NLayerType::NFuncArctg;
    this->koef = 1;
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
