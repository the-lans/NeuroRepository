#ifndef NOPTIMIZERADAM_H
#define NOPTIMIZERADAM_H

template <typename NType> class TrainBP;
#include "inoptimizer.h"

template <typename NType>
class NOptimizerAdam: public INOptimizer<NType>
{
public:
    NOptimizerAdam();
    NOptimizerAdam(TrainBP<NType>* train);
    NOptimizerAdam(TrainBP<NType>* train, NArray<NType>& param);
    virtual ~NOptimizerAdam();

protected:
    NType beta1;
    NType beta2;
    NType epsilon;
    NType sbeta1; //= 1 - beta1
    NType sbeta2; //= 1 - beta2
    NType koefLR; //= Корень(1 - beta2)/(1 - beta1)
    INeuroNetStruct<NType> moment1; //Initialize 1st moment struct
    INeuroNetStruct<NType> moment2; //Initialize 2nd moment struct
    NMatrix<NType> changeW;
    NArray<NType> changeB;

public:
    virtual void init();
    virtual void init(NArray<NType>& param);
    //virtual void deinit();
    virtual void runWeight(NMatrix<NType>& change, int k);
    virtual void runBias(NArray<NType>& change, int k);
};

template <typename NType>
NOptimizerAdam<NType>::NOptimizerAdam():
    INOptimizer<NType>()
{
    this->init();
}

template <typename NType>
NOptimizerAdam<NType>::NOptimizerAdam(TrainBP<NType>* train):
    INOptimizer<NType>(train)
{
    this->init();
}

template <typename NType>
NOptimizerAdam<NType>::NOptimizerAdam(TrainBP<NType>* train, NArray<NType>& param):
    INOptimizer<NType>(train)
{
    this->init(param);
}

template <typename NType>
NOptimizerAdam<NType>::~NOptimizerAdam()
{
}

template <typename NType>
void NOptimizerAdam<NType>::init()
{
    NArray<NType> param;
    param.push(0.9); //beta1
    param.push(0.999); //beta2
    param.push(0.00000001); //epsilon
    this->init(param);
}

template <typename NType>
void NOptimizerAdam<NType>::init(NArray<NType>& param)
{
    this->beta1 = param[0];
    this->beta2 = param[1];
    this->epsilon = param[2];
    this->sbeta1 = 1 - this->beta1;
    this->sbeta2 = 1 - this->beta2;
    this->koefLR = NFUNC_SQRT(this->sbeta2)/this->sbeta1;

    this->moment1.init(this->train->net->lenNeurons, this->train->net->typeLay);
    this->moment2.init(this->train->net->lenNeurons, this->train->net->typeLay);
}

template <typename NType>
void NOptimizerAdam<NType>::runWeight(NMatrix<NType>& change, int k)
{
    NMatrix<NType>& W1 = this->moment1.lay[k]->weigth;
    NMatrix<NType>& W2 = this->moment2.lay[k]->weigth;

    //1st moment struct
    this->changeW.copyValue(change);
    this->changeW.valmul(this->sbeta1);
    W1.valmul(this->beta1);
    W1.sum(this->changeW);

    //2st moment struct
    this->changeW.matmul(change, change);
    this->changeW.valmul(this->sbeta2);
    W2.valmul(this->beta2);
    W2.sum(this->changeW);

    //Adam
    change.funcCalc(W2, nfunc_sqrt<NType>);
    change.valsum(this->epsilon);
    change.matdiv(W1, change);
    change.valmul(this->koefLR);
}

template <typename NType>
void NOptimizerAdam<NType>::runBias(NArray<NType>& change, int k)
{
    NArray<NType>& B1 = this->moment1.lay[k]->bias;
    NArray<NType>& B2 = this->moment2.lay[k]->bias;

    //1st moment struct
    this->changeB.copyValue(change);
    this->changeB.valmul(this->sbeta1);
    B1.valmul(this->beta1);
    B1.sum(this->changeB);

    //2st moment struct
    this->changeB.mul(change, change);
    this->changeB.valmul(this->sbeta2);
    B2.valmul(this->beta2);
    B2.sum(this->changeB);

    //Adam
    change.funcCalc(B2, nfunc_sqrt<NType>);
    change.valsum(this->epsilon);
    change.div(B1, change);
    change.valmul(this->koefLR);
}

#endif // NOPTIMIZERADAM_H
