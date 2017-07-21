#ifndef NLAYERSOFTSIGN_H
#define NLAYERSOFTSIGN_H

#include "inlayer.h"

template <typename NType>
class NLayerSoftsign: public INLayer<NType>
{
public:
    NLayerSoftsign();
    virtual ~NLayerSoftsign();
public:
    virtual NType activation(NType& x);
    virtual NType derivative(NType& y);
};

#endif // NLAYERSOFTSIGN_H
