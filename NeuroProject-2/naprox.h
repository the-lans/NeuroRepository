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


template <typename NType>
NAprox<NType>::NAprox():
    INeuroNet<NType>()
{
}

template <typename NType>
NAprox<NType>::NAprox(NAprox<NType>& obj):
    INeuroNet<NType>(obj)
{
}

template <typename NType>
NAprox<NType>::NAprox(int size):
    INeuroNet<NType>(size)
{
}

template <typename NType>
void NAprox<NType>::prerun(NMatrix<NType>& tab)
{
    int ind;
    NType value;
    this->emptyExamples();

    for(ind = 0; ind < tab.getLenRow(); ind++)
    {
        NExample<NType>* pExam = new NExample<NType>();

        value = tab[ind][0];
        pExam->input.push(value);

        value = tab[ind][1];
        pExam->output.push(value);

        this->exam.push(pExam);
    }

    this->beginset = 0;
    this->testset = this->exam.getLength();
    this->endset = this->exam.getLength();
}

template <typename NType>
NArray<NType>* NAprox<NType>::postrun()
{
    this->outpostrun.init(0, 0);
    NType value = this->outrun->get(0);
    this->outpostrun.push(value);
    return &(this->outpostrun);
}

#endif // NAPROX_H
