#include "nlayersoftsign.h"

template <typename NType>
NLayerSoftsign<NType>::NLayerSoftsign()
{
    this->type = NLayerType::NFuncSoftsign;
    this->koef = 1;
}

template <typename NType>
NLayerSoftsign<NType>::~NLayerSoftsign()
{
}

template <typename NType>
NType NLayerSoftsign<NType>::activation(NType& x)
{
    if(x >= 0)
    {
        return x/(1/this->koef + x);
    }
    else
    {
        return x/(1/this->koef - x);
    }
}

template <typename NType>
NType NLayerSoftsign<NType>::derivative(NType& y)
{
    if(y >= 0)
    {
        NType val = y - 1;
        return this->koef * val * val;
    }
    else
    {
        NType val = y + 1;
        return this->koef * val * val;
    }
}
