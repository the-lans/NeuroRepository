#ifndef NAPROX_H
#define NAPROX_H

#include "ineuronet.h"

template <typename NType>
class NAprox: public INeuroNet<NType>
{
public:
    NAprox();
    NAprox(NAprox<NType>& obj);
    NAprox(int size);
public:
    virtual void prerun(NMatrix<NType>& tab);
    virtual NArray<NType>* postrun();
};

#endif // NAPROX_H
