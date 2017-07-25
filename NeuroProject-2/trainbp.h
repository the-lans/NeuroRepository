#ifndef TRAINBP_H
#define TRAINBP_H

#include "ineuronet.h"

template <typename NType>
class TrainBP
{
public:
    TrainBP();
    ~TrainBP();
public:
    INeuroNet<NType>* net;
    INeuroNet<NType>* changeNet;
    NArray< NArray<NType>* > errorNet;
    NArray<NType> backOut;
    NArray<NType> energyAver;
    NArray<NType> energyMax;
protected:
    NType speedLearning;
    NType speedRegularization;
    NType inertia;
    int maxEpoches;
    NType minError;
    NType minErrorChange;
    int epoches;
    int stabEnergy;
    NType changeEnergy;
    int stepReg;
private:
    NMatrix<NType> changeWeight1, changeWeight2;
    NArray<NType> changeBias1, changeBias2;
    NType koefTrain;
    NType koefRegular;
    INLayer<NType>** pLay; //net.lay
    NExample<NType>** pExam; //net.exam
    NArray<NType>** pMas; //errorNet
    INLayer<NType>** pChangeLay; //changeNet.lay
    NType* pOutrun; //exm.outrun
    int lenLay; //net.lay
public:
    void setSpeedLearning(NType param);
    void setSpeedRegularization(NType param);
    void setInertia(NType param);
    void setMaxEpoches(NType param);
    void setMinError(NType param);
    void setMinErrorChange(NType param);
    void setStepReg(int param);
    NType getSpeedLearning();
    NType getSpeedRegularization();
    NType getInertia();
    int getEpoches();
    int getStabEnergy();
    NType getChangeEnergy();
    int getStepReg();
public:
    virtual void init();
    virtual void deinit();
    virtual void train();
    virtual void calculateError(NExample<NType>* exm);
    virtual void change(NExample<NType>* exm);
    virtual void update();
};


template <typename NType>
TrainBP<NType>::TrainBP()
{
    this->net = nullptr;
    this->changeNet = nullptr;
    this->speedLearning = 1;
    this->speedRegularization = 0;
    this->inertia = 0;
    this->maxEpoches = 0;
    this->minError = 0;
    this->minErrorChange = 0;
    this->epoches = 0;
    this->changeEnergy = 0;
    this->stepReg = 0;
}

template <typename NType>
TrainBP<NType>::~TrainBP()
{
    this->deinit();
}


template <typename NType>
void TrainBP<NType>::setSpeedLearning(NType param)
{
    this->speedLearning = param;
}

template <typename NType>
void TrainBP<NType>::setSpeedRegularization(NType param)
{
    this->speedRegularization = param;
}

template <typename NType>
void TrainBP<NType>::setInertia(NType param)
{
    this->inertia = param;
}

template <typename NType>
void TrainBP<NType>::setMaxEpoches(NType param)
{
    this->maxEpoches = param;
}

template <typename NType>
void TrainBP<NType>::setMinError(NType param)
{
    this->minError = param;
}

template <typename NType>
void TrainBP<NType>::setMinErrorChange(NType param)
{
    this->minErrorChange = param;
}

template <typename NType>
void TrainBP<NType>::setStepReg(int param)
{
    this->stepReg = param;
}

template <typename NType>
NType TrainBP<NType>::getSpeedLearning()
{
     return this->speedLearning;
}

template <typename NType>
NType TrainBP<NType>::getSpeedRegularization()
{
    return this->speedRegularization;
}

template <typename NType>
NType TrainBP<NType>::getInertia()
{
    return this->inertia;
}

template <typename NType>
int TrainBP<NType>::getEpoches()
{
    return this->epoches;
}

template <typename NType>
int TrainBP<NType>::getStabEnergy()
{
    return this->stabEnergy;
}

template <typename NType>
NType TrainBP<NType>::getChangeEnergy()
{
    return this->changeEnergy;
}

template <typename NType>
int TrainBP<NType>::getStepReg()
{
    return this->stepReg;
}


template <typename NType>
void TrainBP<NType>::init()
{
    this->deinit();
    this->changeEnergy = 0;
    this->epoches = 0;
    this->stabEnergy = 0;
    this->energyAver.clear();
    this->energyMax.clear();

    if(this->changeNet == nullptr)
    {
        this->changeNet = new INeuroNet<NType>;
    }
    this->changeNet->copyNet(this->net);
    this->changeNet->setValueWeight(0);
    this->changeNet->init();

    NArray<NType>* mas;
    INLayer<NType>* layer;
    for(int i = 0; i < this->net->lay.getLength(); i++)
    {
        layer = this->net->lay[i];
        mas = new NArray<NType>();
        mas->init(layer->weigth.getLenColumn(), 0);
        this->errorNet.push(mas);
    }
}

template <typename NType>
void TrainBP<NType>::deinit()
{
    if(this->changeNet != nullptr)
    {
        this->changeNet->emtyLayer();
        delete this->changeNet;
        this->changeNet = nullptr;
    }

    NArray<NType>* mas;
    for(int i = 0; i < this->errorNet.getLength(); i++)
    {
        mas = this->errorNet.pop();
        delete mas;
    }
}

template <typename NType>
void TrainBP<NType>::train()
{
    int pos;
    NExample<NType>* exm;
    NType prevEnrg, curEnrg;

    int beginset = this->net->getBeginset();
    int testset = this->net->getTestset();
    this->koefTrain = this->speedLearning * (1 - this->inertia);
    this->koefRegular = - (this->speedLearning * this->speedRegularization) / (testset - beginset);
    this->pLay = this->net->lay.getData();
    this->pExam = this->net->exam.getData();
    this->pMas = this->errorNet.getData();
    this->pChangeLay = this->changeNet->lay.getData();
    this->lenLay = this->net->lay.getLength();

    this->net->runExamples(NSetType::NSetTrain);
    //this->net->runEnergy(NSetType::NSetTrain);
    prevEnrg = this->net->getEnergyAver();
    curEnrg = this->net->getEnergyAver();
    this->changeEnergy = prevEnrg;

    while(this->epoches < this->maxEpoches
          && curEnrg > this->minError
          && this->changeEnergy >= this->minErrorChange)
    {
        while(this->epoches < this->maxEpoches
            && curEnrg > this->minError
            && this->changeEnergy >= this->minErrorChange)
        {
            for(pos = beginset; pos < testset; pos++)
            {
                exm = this->pExam[pos];
                this->net->runExample(pos);
                calculateError(exm);
                change(exm);
                update();
            }
            this->epoches++;
            //this->net->runExamples(NSetType::NSetTrain);
            this->net->runEnergy(NSetType::NSetTrain);
            curEnrg = this->net->getEnergyAver();
            this->changeEnergy = prevEnrg - curEnrg;
            if(this->changeEnergy < 0) {this->changeEnergy = -this->changeEnergy;}
            prevEnrg = curEnrg;

            if((this->stepReg != 0)&&(this->epoches % this->stepReg == 0))
            {
                this->energyAver.push(curEnrg);
                this->energyMax.push(this->net->getEnergyMax());
            }
        }

        this->stabEnergy++;
        this->net->runExamples(NSetType::NSetTrain);
        curEnrg = this->net->getEnergyAver();
        this->changeEnergy = prevEnrg - curEnrg;
        if(this->changeEnergy < 0) {this->changeEnergy = -this->changeEnergy;}
        prevEnrg = curEnrg;
    }
}

template <typename NType>
void TrainBP<NType>::calculateError(NExample<NType>* exm)
{
    int k, j;

    // Выходной слой
    NArray<NType>& er = this->net->derivEnergy(exm);
    k = lenLay-1;

    if(pLay[k]->getTypeDerivat() == NLayerDerivat::NDerivatOut)
    {
        pOutrun = exm->outrun.getData();
    }
    else
    {
        pOutrun = pLay[k]->sum.getData();
    }

    for(j = 0; j < er.getLength(); j++)
    {
        pMas[k]->set(pLay[k]->derivative(pOutrun[j]), j);
    }
    pMas[k]->mul(er);

    // Скрытый слой
    for(k = lenLay-2; k >= 0; k--)
    {
        if(pLay[k]->getTypeDerivat() == NLayerDerivat::NDerivatOut)
        {
            pOutrun = pLay[k]->output.getData();
        }
        else
        {
            pOutrun = pLay[k]->sum.getData();
        }

        for(j = 0; j < pLay[k]->weigth.getLenColumn(); j++)
        {
            pMas[k]->set(pLay[k]->derivative(pOutrun[j]), j);
        }
        this->backOut.mul(*pMas[k+1], pLay[k+1]->weigth, false);
        pMas[k]->mul(this->backOut);
    }
}

template <typename NType>
void TrainBP<NType>::change(NExample<NType>* exm)
{
    // Входной слой weight
    this->changeWeight1.mul(*pMas[0], exm->input);
    this->changeWeight1.valmul(koefTrain);
    this->changeWeight2 = pChangeLay[0]->weigth;
    this->changeWeight2.valmul(this->inertia);
    this->changeWeight2.sum(this->changeWeight1);

    this->changeWeight1 = pLay[0]->weigth;
    this->changeWeight1.valmul(koefRegular);
    pChangeLay[0]->weigth.sum(this->changeWeight1, this->changeWeight2);

    // Скрытый слой weight
    for(int k = 1; k < this->net->lay.getLength(); k++)
    {
        this->changeWeight1.mul(*pMas[k], pLay[k-1]->output);
        this->changeWeight1.valmul(koefTrain);
        this->changeWeight2 = pChangeLay[k]->weigth;
        this->changeWeight2.valmul(this->inertia);
        this->changeWeight2.sum(this->changeWeight1);

        this->changeWeight1 = pLay[k]->weigth;
        this->changeWeight1.valmul(koefRegular);
        pChangeLay[k]->weigth.sum(this->changeWeight1, this->changeWeight2);
    }

    // Cлой bias
    for(int k = 0; k < this->net->lay.getLength(); k++)
    {
        this->changeBias1 = *pMas[k];
        this->changeBias1.valmul(koefTrain);
        this->changeBias2 = pChangeLay[k]->bias;
        this->changeBias2.valmul(this->inertia);
        this->changeBias2.sum(this->changeBias1);

        this->changeBias1 = pLay[k]->bias;
        this->changeBias1.valmul(koefRegular);
        pChangeLay[k]->bias.sum(this->changeBias1, this->changeBias2);
    }
}

template <typename NType>
void TrainBP<NType>::update()
{
    for(int k = 0; k < this->net->lay.getLength(); k++)
    {
        pLay[k]->weigth.sum(pChangeLay[k]->weigth);
        pLay[k]->bias.sum(pChangeLay[k]->bias);
    }
}

#endif // TRAINBP_H
