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


template <typename NType>
NExample<NType>::NExample()
{
    this->energy = 0;
}

template <typename NType>
NExample<NType>::NExample(NExample<NType>& obj):
    input(obj.input), output(obj.output), outrun(obj.outrun)
{
    this->energy = obj.getEnergy();
}

template <typename NType>
NExample<NType>::~NExample()
{
}

template <typename NType>
void NExample<NType>::init(int lenIn, int lenOut, NType value)
{
    this->input.init(lenIn, value);
    this->output.init(lenOut, value);
}

template <typename NType>
void NExample<NType>::setEnergy(NType energy)
{
    this->energy = energy;
}

template <typename NType>
NType NExample<NType>::getEnergy()
{
    return this->energy;
}

#endif // NEXAMPLE_H
