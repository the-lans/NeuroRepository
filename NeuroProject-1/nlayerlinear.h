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

#endif // NLAYERLINEAR_H
