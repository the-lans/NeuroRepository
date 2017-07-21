#ifndef NLAYERTANH_H
#define NLAYERTANH_H

#include "inlayer.h"

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

#endif // NLAYERTANH_H
