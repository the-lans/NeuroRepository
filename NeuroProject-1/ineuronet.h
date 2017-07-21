#ifndef INEURONET_H
#define INEURONET_H

#include "inlayer.h"
#include "nlayertanh.h"
#include "nlayersoftsign.h"
#include "nlayerarctg.h"
#include "nlayerlinear.h"
#include "nexample.h"
#include <iostream>
#include <ctime>

template <typename NType>
class INeuroNet
{
public:
    INeuroNet();
    INeuroNet(INeuroNet<NType>& obj);
    INeuroNet<NType>& operator=(const INeuroNet<NType>& obj);
    INeuroNet(int size);
    virtual ~INeuroNet();
public:
    NArray<INLayer<NType>*> lay;
    NArray<NExample<NType>*> exam;
    NArray<NType> outpostrun;
protected:
    NType valueWeight;
    NArray<NType>* outrun;
    int beginset;
    int testset;
    int endset;
    NType energyAver;
    NType energyMax;
    NArray<NType> derivEnrg;
    NType energySum;
    NType energyRegularization;
public:
    void setValueWeight(NType value);
    NType getValueWeight();
    void setOutRun(NArray<NType>* val);
    NArray<NType>* getOutrun();
    void setTestset(int testset);
    int getTestset();
    void setBeginset(int val);
    void setEndset(int val);
    int getBeginset();
    int getEndset();
    NType getEnergyAver();
    NType getEnergyMax();
    int getLenIn();
    int getLenOut();
    NType getEnergySum();
    NType getEnergyRegularization();
public:
    void copyNet(INeuroNet<NType>* obj);
    virtual NArray<NType>* run(NArray<NType>* X);
    void runExamples(NSetType st);
    void runExample(int pos);
    INLayer<NType>* newLayer(NLayerType layType);
    virtual void init();
    virtual void init(NArray<int>& num, NArray<NLayerType>& layType);
    virtual void emtyLayer();
    virtual void emptyExamples();
    virtual void ginit();
    virtual void ginit(NArray<int>& num, NArray<NLayerType>& layType);
    virtual void runEnergy(NSetType st);
    virtual NType funcEnergy(NExample<NType>* exm);
    virtual NType funcRegularization();
    virtual NArray<NType>& derivEnergy(NExample<NType>* exm);
public:
    virtual void prerun(NMatrix<NType>& tab);
    virtual NArray<NType>* postrun();
};

#endif // INEURONET_H
