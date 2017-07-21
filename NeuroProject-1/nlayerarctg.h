#ifndef NLAYERARCTG_H
#define NLAYERARCTG_H

#include "inlayer.h"

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

#endif // NLAYERARCTG_H
