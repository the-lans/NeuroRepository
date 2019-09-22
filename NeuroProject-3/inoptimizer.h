#ifndef INOPTIMIZER_H
#define INOPTIMIZER_H

#include "defsmart.h"
#include "neuroproject.h"
#include "ineuronetstruct.h"
#include "nexample.h"

template <typename NType> class TrainBP;
//#include "trainbp.h"

template <typename NType>
class INOptimizer
{
public:
    INOptimizer();
    INOptimizer(TrainBP<NType>* train);
    virtual ~INOptimizer();

protected:
    TrainBP<NType>* train; //Обучение
    NOptimizerType optType; //Тип алгоритма оптимизации

public:
    void setTrain(TrainBP<NType>* train);
    TrainBP<NType>* getTrain();
    void setType(NOptimizerType optType);
    NOptimizerType getType();

public:
    virtual void init();
    virtual void init(NArray<NType>& param);
    virtual void deinit();
    virtual void runWeight(NMatrix<NType>& change, int k);
    virtual void runBias(NArray<NType>& change, int k);
};

template <typename NType>
INOptimizer<NType>::INOptimizer()
{
    this->train = nullptr;
    this->optType = NOptimizerType::NOptimizerNone;
    this->init();
}

template <typename NType>
INOptimizer<NType>::INOptimizer(TrainBP<NType>* train)
{
    this->train = train;
    this->optType = NOptimizerType::NOptimizerNone;
    this->init();
}

template <typename NType>
INOptimizer<NType>::~INOptimizer()
{
    this->deinit();
}

template <typename NType>
void INOptimizer<NType>::setTrain(TrainBP<NType>* train)
{
    this->train = train;
}

template <typename NType>
TrainBP<NType>* INOptimizer<NType>::getTrain()
{
    return this->train;
}

template <typename NType>
void INOptimizer<NType>::setType(NOptimizerType optType)
{
    this->optType = optType;
}

template <typename NType>
NOptimizerType INOptimizer<NType>::getType()
{
    return this->optType;
}

template <typename NType>
void INOptimizer<NType>::init()
{
}

template <typename NType>
void INOptimizer<NType>::init(NArray<NType>& /*param*/)
{
}

template <typename NType>
void INOptimizer<NType>::deinit()
{
}

template <typename NType>
void INOptimizer<NType>::runWeight(NMatrix<NType>& /*change*/, int /*k*/)
{
}

template <typename NType>
void INOptimizer<NType>::runBias(NArray<NType>& /*change*/, int /*k*/)
{
}

#endif // INOPTIMIZER_H
