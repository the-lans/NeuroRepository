#ifndef NEXAMPLE_H
#define NEXAMPLE_H

#include "narray.h"

enum NSetType {NSetNone, NSetTrain, NSetTest};

template <typename NType>
class NExample
{
public:
    NExample();
    NExample(NExample<NType>& obj);
    ~NExample();
public:
    NArray<NType> input;
    NArray<NType> output;
    NArray<NType> outrun;
protected:
    NType energy;
public:
    void init(int lenIn, int lenOut, NType value);
    void setEnergy(NType energy);
    NType getEnergy();
};

#endif // NEXAMPLE_H
