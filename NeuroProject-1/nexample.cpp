#include "nexample.h"

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
