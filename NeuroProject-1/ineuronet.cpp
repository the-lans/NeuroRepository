#include "ineuronet.h"

template <typename NType>
INeuroNet<NType>::INeuroNet()
{
    this->valueWeight = 0;
    this->outrun = nullptr;
    this->testset = 0;
    this->beginset = 0;
    this->endset = 0;
    this->energyRegularization = 0;
}

template <typename NType>
INeuroNet<NType>::INeuroNet(INeuroNet<NType>& obj):
    lay(obj.lay.getLength()), exam(obj.exam.getLength()), outpostrun(obj.outpostrun)
{
    INLayer<NType>** pLay = obj.lay.getData();
    INLayer<NType>* layer;
    for(int i = 0; i < obj.lay.getLength(); i++)
    {
        layer = this->newLayer(pLay[i]->getType());
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    NExample<NType>* exm = nullptr;
    for(int i = 0; i < obj.exam.getLength(); i++)
    {
        exm = new NExample<NType>(obj.exam.get(i));
        this->exam.push(exm);
    }

    this->valueWeight = obj.getValueWeight();
    this->outrun = &(layer->output);
    this->testset = obj.getTestset();
    this->beginset = obj.getBeginset();
    this->endset = obj.getEndset();
    this->energyRegularization = obj.getEnergyRegularization();
}

template <typename NType>
INeuroNet<NType>& INeuroNet<NType>::operator=(const INeuroNet<NType>& obj)
{
    this->lay.clear();
    this->exam.clear();
    this->lay.resize(obj.lay.getLength());
    this->exam.resize(obj.exam.getLength());
    this->outpostrun = obj.outpostrun;

    INLayer<NType>** pLay = obj.lay.getData();
    INLayer<NType>* layer;
    for(int i = 0; i < obj.lay.getLength(); i++)
    {
        layer = this->newLayer(pLay[i]->getType());
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    NExample<NType>* exm = nullptr;
    for(int i = 0; i < obj.exam.getLength(); i++)
    {
        exm = new NExample<NType>(obj.exam.get(i));
        this->exam.push(exm);
    }

    this->valueWeight = obj.getValueWeight();
    this->outrun = &(layer->output);
    this->testset = obj.getTestset();
    this->beginset = obj.getBeginset();
    this->endset = obj.getEndset();
    this->energyRegularization = obj.getEnergyRegularization();

    return *this;
}

template <typename NType>
INeuroNet<NType>::INeuroNet(int size):
    lay(size)
{
    this->valueWeight = 0;
    this->outrun = nullptr;
    this->testset = 0;
    this->beginset = 0;
    this->endset = 0;
    this->energyRegularization = 0;
}

template <typename NType>
INeuroNet<NType>::~INeuroNet()
{
    this->emtyLayer();
    this->emptyExamples();
}


template <typename NType>
void INeuroNet<NType>::setValueWeight(NType value)
{
    this->valueWeight = value;
}

template <typename NType>
NType INeuroNet<NType>::getValueWeight()
{
    return this->valueWeight;
}

template <typename NType>
void INeuroNet<NType>::setTestset(int testset)
{
    this->testset = testset;
}

template <typename NType>
int INeuroNet<NType>::getTestset()
{
    return this->testset;
}

template <typename NType>
void INeuroNet<NType>::setBeginset(int val)
{
    this->beginset = val;
}

template <typename NType>
void INeuroNet<NType>::setEndset(int val)
{
    this->endset = val;
}

template <typename NType>
int INeuroNet<NType>::getBeginset()
{
    return this->beginset;
}

template <typename NType>
int INeuroNet<NType>::getEndset()
{
    return this->endset;
}

template <typename NType>
void INeuroNet<NType>::setOutRun(NArray<NType>* val)
{
    this->outrun = val;
}

template <typename NType>
NArray<NType>* INeuroNet<NType>::getOutrun()
{
    return this->outrun;
}

template <typename NType>
NType INeuroNet<NType>::getEnergyAver()
{
    return this->energyAver;
}

template <typename NType>
NType INeuroNet<NType>::getEnergyMax()
{
    return this->energyMax;
}

template <typename NType>
int INeuroNet<NType>::getLenIn()
{
    return this->lay.get(0)->weigth.getLenRow();
}

template <typename NType>
int INeuroNet<NType>::getLenOut()
{
    int len = this->lay.getLength();
    return this->lay.get(len-1)->weigth.getLenColumn();
}

template <typename NType>
NType INeuroNet<NType>::getEnergySum()
{
    return this->energySum;
}

template <typename NType>
NType INeuroNet<NType>::getEnergyRegularization()
{
    return this->energyRegularization;
}


template <typename NType>
void INeuroNet<NType>::copyNet(INeuroNet<NType>* obj)
{
    this->emtyLayer();
    //this->lay.init(obj->lay.getLength(), nullptr);
    this->outpostrun = obj->outpostrun;

    INLayer<NType>** pLay = obj->lay.getData();
    INLayer<NType>* layer;
    for(int i = 0; i < obj->lay.getLength(); i++)
    {
        layer = this->newLayer(pLay[i]->getType());
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    this->valueWeight = obj->getValueWeight();
    this->outrun = &(layer->output);
    this->testset = obj->getTestset();
    this->beginset = obj->getBeginset();
    this->endset = obj->getEndset();
    this->energyRegularization = obj->getEnergyRegularization();
}

template <typename NType>
NArray<NType>* INeuroNet<NType>::run(NArray<NType>* X)
{
    INLayer<NType>** pLay = this->lay.getData();
    NArray<NType>* input = X;
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay[i]->run(input);
        input = &(pLay[i]->output);
    }
    this->outrun = input;

    return this->outrun;
}

template <typename NType>
void INeuroNet<NType>::runExamples(NSetType st)
{
    for(int i = 0; i < this->exam.getLength(); i++)
    {
        this->runExample(i);
    }

    this->runEnergy(st);
}

template <typename NType>
void INeuroNet<NType>::runExample(int pos)
{
    NExample<NType>* exm = this->exam.get(pos);
    NArray<NType>* input = &(exm->input);
    this->run(input);

    exm->outrun.clear();
    NType* pOutrun = this->outrun->getData();
    for(int i = 0; i < this->outrun->getLength(); i++)
    {
        exm->outrun.push(pOutrun[i]);
    }

    exm->setEnergy(this->funcEnergy(exm));
}

template <typename NType>
INLayer<NType>* INeuroNet<NType>::newLayer(NLayerType layType)
{
    INLayer<NType>* layer;
    if(layType == NLayerType::NFuncTanh)
    {
        layer = new NLayerTanh<NType>();
    }
    else if(layType == NLayerType::NFuncSoftsign)
    {
        layer = new NLayerSoftsign<NType>();
    }
    else if(layType == NLayerType::NFuncArctg)
    {
        layer = new NLayerArctg<NType>();
    }
    else if(layType == NLayerType::NFuncLinear)
    {
        layer = new NLayerLinear<NType>();
    }
    return layer;
}

template <typename NType>
void INeuroNet<NType>::init()
{
    if(this->endset == 0) {this->endset = this->exam.getLength();}

    INLayer<NType>** pLay = this->lay.getData();
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay[i]->init(this->valueWeight);
    }
}

template <typename NType>
void INeuroNet<NType>::init(NArray<int>& num, NArray<NLayerType>& layType)
{
    this->emtyLayer();
    if(this->endset == 0) {this->endset = this->exam.getLength();}

    int lenLayer = num.getLength()-1;
    this->lay.resize(lenLayer);
    INLayer<NType>* layer;
    for(int i = 0; i < lenLayer; i++)
    {
        layer = this->newLayer(layType.get(i));
        layer->init(num.get(i), num.get(i+1), this->valueWeight);
        this->lay.push(layer);
    }
}

template <typename NType>
void INeuroNet<NType>::emtyLayer()
{
    INLayer<NType>* layer;
    int len = this->lay.getLength();
    for(int i = 0; i < len; i++)
    {
        layer = this->lay.pop();
        delete layer;
    }
}

template <typename NType>
void INeuroNet<NType>::emptyExamples()
{
    NExample<NType>* exm;
    for(int i = 0; i < this->exam.getLength(); i++)
    {
        exm = this->exam.pop();
        delete exm;
    }
}

template <typename NType>
void INeuroNet<NType>::ginit()
{
    if(this->endset == 0) {this->endset = this->exam.getLength();}

    srand(time(0));
    INLayer<NType>** pLay = this->lay.getData();
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay[i]->ginit(this->valueWeight);
    }
}

template <typename NType>
void INeuroNet<NType>::ginit(NArray<int>& num, NArray<NLayerType>& layType)
{
    this->emtyLayer();
    if(this->endset == 0) {this->endset = this->exam.getLength();}

    srand(time(0));
    int lenLayer = num.getLength()-1;
    this->lay.resize(lenLayer);
    INLayer<NType>* layer;
    for(int i = 0; i < lenLayer; i++)
    {
        layer = this->newLayer(layType.get(i));
        layer->ginit(num.get(i), num.get(i+1), this->valueWeight);
        this->lay.push(layer);
    }
}

template <typename NType>
void INeuroNet<NType>::runEnergy(NSetType st)
{
    int i;
    int begExm = this->beginset;
    int endExm = this->endset;
    if(st == NSetType::NSetTrain)
    {
        begExm = this->beginset;
        endExm = this->testset - 1;
    }
    else if(st == NSetType::NSetTest)
    {
        begExm = this->testset;
        endExm = this->endset - 1;
    }
    int lenExm = endExm - begExm + 1;

    NExample<NType>** pData = this->exam.getData();
    NType enrgSum = 0;
    NType enrgMax;

    for(i = begExm; i <= endExm; i++)
    {
        enrgSum += pData[i]->getEnergy();
    }
    this->energySum = enrgSum;
    this->energyAver = (lenExm == 0 ? 0 : enrgSum / lenExm);

    this->energyMax = (lenExm == 0 ? 0 : pData[begExm]->getEnergy());
    for(i = begExm+1; i <= endExm; i++)
    {
        enrgMax = pData[i]->getEnergy();
        if(this->energyMax < enrgMax)
        {
            this->energyMax = enrgMax;
        }
    }
}

template <typename NType>
NType INeuroNet<NType>::funcEnergy(NExample<NType>* exm)
{
    NType enrg = 0;
    NType* pOutput = exm->output.getData();
    NType* pOutrun = exm->outrun.getData();
    for(int i = 0; i < exm->output.getLength(); i++)
    {
        enrg += (pOutput[i] - pOutrun[i]) * (pOutput[i] - pOutrun[i]);
    }
    enrg = enrg / 2;
    exm->setEnergy(enrg);
    return enrg;
}

template <typename NType>
NType INeuroNet<NType>::funcRegularization()
{
    this->energyRegularization = 0;
    INLayer<NType>* currentLay;
    for(int k = 0; k < this->lay.getLength(); k++)
    {
        currentLay = this->lay.get(k);
        this->energyRegularization += currentLay->funcRegularization();
    }

    return this->energyRegularization;
}

template <typename NType>
NArray<NType>& INeuroNet<NType>::derivEnergy(NExample<NType>* exm)
{
    NType enrg = 0;
    NType* pOutput = exm->output.getData();
    NType* pOutrun = exm->outrun.getData();
    this->derivEnrg.clear();
    for(int i = 0; i < exm->output.getLength(); i++)
    {
        enrg = pOutput[i] - pOutrun[i];
        this->derivEnrg.push(enrg);
    }
    return this->derivEnrg;
}

template <typename NType>
void INeuroNet<NType>::prerun(NMatrix<NType>& tab)
{
    int ind, jnd;
    NType value;
    this->emptyExamples();

    for(ind = 0; ind < tab.getLenRow(); ind++)
    {
        NExample<NType>* pExam = new NExample<NType>();
        for(jnd = 0; jnd < tab.getLenColumn(); jnd++)
        {
            value = tab.get(ind, jnd);
            pExam->input.push(value);
        }
        this->exam.push(pExam);
    }

    this->beginset = 0;
    this->testset = this->exam.getLength();
    this->endset = this->exam.getLength();
}

template <typename NType>
NArray<NType>* INeuroNet<NType>::postrun()
{
    this->outpostrun.clear();
    NType value;
    for(int ind = 0; ind < this->outrun->getLength(); ind++)
    {
        value = this->outrun->get(ind);
        this->outpostrun.push(value);
    }
    return &(this->outpostrun);
}
