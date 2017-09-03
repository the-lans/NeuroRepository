#ifndef TRAINBP_H
#define TRAINBP_H

#include "ineuronet.h"
#include "iobjectecsv.h"

template <typename NType>
class TrainBP: public IObjectECSV
{
public:
    TrainBP();
    ~TrainBP();
public:
    INeuroNet<NType>* net; //Обучаемая нейронная сеть
    INeuroNet<NType>* changeNet; //Копия нейросети для изменений весов
    NArray< NArray<NType>* > errorNet; //Ошибки слоёв
    NArray<NType> energyAver; //Обучение: средняя ошибка обучения
    NArray<NType> energyMax; //Обучение: максимальная ошибка обучения
protected:
    NType speedLearning; //Обучение: скорость обучения
    NType speedRegularization; //Обучение: скорость регуляризации
    NType inertia; //Обучение: инерция
    int maxEpoches; //Обучение: максимальное количество эпох
    NType minError; //Обучение: минимальная ошибка
    NType minErrorChange; //Обучение: минимальное изменение энергии
    int epoches; //Обучение: Количество эпох
    int stabEnergy; //Обучение: количество стабилизаций энергии
    NType changeEnergy; //Обучение: изменение энергии
    int stepReg; //Обучение: шаг сохранения энергии
private:
    NArray<NType> backOut; //Производная целевой функции или вектор, рассчитанный по дельта-правилу
    NMatrix<NType> changeWeight1, changeWeight2;
    NArray<NType> changeBias1, changeBias2;
    NType koefTrain; //Коэффициент обучения, агрегирующий другие коэффициенты
    NType koefRegular; //Коэффициент регуляризации
    INLayer<NType>** pLay; //net.lay
    NExample<NType>** pExam; //net.examples
    NArray<NType>** pMas; //errorNet
    INLayer<NType>** pChangeLay; //changeNet.lay
    NType* pOutrun; //exm.outrun
    int lenLay; //net.lay - Количество слоёв
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
    virtual void init(); //Инициализация
    virtual void deinit(); //Деинициализация
    virtual void train(); //Процесс обучения
    virtual void calculateError(NExample<NType>* exm); //Расчёт ошибок обучения
    virtual void change(NExample<NType>* exm); //Расчёт изменений весов
    virtual void update(); //Принятие изменений
public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
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
    this->stabEnergy = 0;
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
    int len = this->errorNet.getLength();
    for(int i = 0; i < len; i++)
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

    int beginset = this->net->examples->getBeginset();
    int testset = this->net->examples->getTestset();
    this->koefTrain = this->speedLearning * (1 - this->inertia);
    this->koefRegular = - (this->speedLearning * this->speedRegularization) / (testset - beginset);
    this->pLay = this->net->lay.getData();
    this->pExam = this->net->examples->getData();
    this->pMas = this->errorNet.getData();
    this->pChangeLay = this->changeNet->lay.getData();
    this->lenLay = this->net->lay.getLength();

    this->net->runExamples(NSetType::NSetTrain);
    prevEnrg = this->net->examples->getEnergyAver();
    curEnrg = this->net->examples->getEnergyAver();
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
            this->net->runEnergy(NSetType::NSetTrain);
            curEnrg = this->net->examples->getEnergyAver();
            this->changeEnergy = prevEnrg - curEnrg;
            if(this->changeEnergy < 0) {this->changeEnergy = -this->changeEnergy;}
            prevEnrg = curEnrg;

            if((this->stepReg != 0)&&(this->epoches % this->stepReg == 0))
            {
                this->energyAver.push(curEnrg);
                this->energyMax.push(this->net->examples->getEnergyMax());
            }
        }

        this->stabEnergy++;
        this->net->runExamples(NSetType::NSetTrain);
        curEnrg = this->net->examples->getEnergyAver();
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

template <typename NType>
void TrainBP<NType>::saveECSV(DataECSV& dt, string& parent)
{
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;

    str_val = to_string(this->speedLearning); dt.addElement(parent, "speedLearning", str_val, typeid(NType).name());
    str_val = to_string(this->speedRegularization); dt.addElement(parent, "speedRegularization", str_val, typeid(NType).name());
    str_val = to_string(this->inertia); dt.addElement(parent, "inertia", str_val, typeid(NType).name());
    str_val = to_string(this->maxEpoches); dt.addElement(parent, "maxEpoches", str_val, typeid(int).name());
    str_val = to_string(this->minError); dt.addElement(parent, "minError", str_val, typeid(NType).name());
    str_val = to_string(this->minErrorChange); dt.addElement(parent, "minErrorChange", str_val, typeid(NType).name());
    str_val = to_string(this->epoches); dt.addElement(parent, "epoches", str_val, typeid(int).name());
    str_val = to_string(this->stabEnergy); dt.addElement(parent, "stabEnergy", str_val, typeid(int).name());
    str_val = to_string(this->changeEnergy); dt.addElement(parent, "changeEnergy", str_val, typeid(NType).name());
    str_val = to_string(this->stepReg); dt.addElement(parent, "stepReg", str_val, typeid(int).name());

    to_array_string(str_vec, this->energyAver); dt.addElement(parent, "energyAver", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyMax); dt.addElement(parent, "energyMax", str_vec, typeid(NType).name());
}

template <typename NType>
void TrainBP<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    //unsigned int enm;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;
    bool* isLoad = newbool(12, false);

    TrainBP<NType>::deinit();

    if(dt.isOneMatrix())
    {
        iter = dt.modules[0];
    }
    else
    {
        size_t ind = dt.getShift();
        while(!booland(isLoad, 12) && ind < dt.modules.size())
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "speedLearning", str_val)) {to_value(this->speedLearning, str_val); isLoad[0] = true;}
            if(iter->getFieldValue(parent, "speedRegularization", str_val)) {to_value(this->speedRegularization, str_val); isLoad[1] = true;}
            if(iter->getFieldValue(parent, "inertia", str_val)) {to_value(this->inertia, str_val); isLoad[2] = true;}
            if(iter->getFieldValue(parent, "maxEpoches", str_val)) {to_value(this->maxEpoches, str_val); isLoad[3] = true;}
            if(iter->getFieldValue(parent, "minError", str_val)) {to_value(minError, str_val); isLoad[4] = true;}
            if(iter->getFieldValue(parent, "minErrorChange", str_val)) {to_value(this->minErrorChange, str_val); isLoad[5] = true;}
            if(iter->getFieldValue(parent, "epoches", str_val)) {to_value(this->epoches, str_val); isLoad[6] = true;}
            if(iter->getFieldValue(parent, "stabEnergy", str_val)) {to_value(this->stabEnergy, str_val); isLoad[7] = true;}
            if(iter->getFieldValue(parent, "changeEnergy", str_val)) {to_value(this->changeEnergy, str_val); isLoad[8] = true;}
            if(iter->getFieldValue(parent, "stepReg", str_val)) {to_value(this->stepReg, str_val); isLoad[9] = true;}

            if(iter->getFieldValue(parent, "energyAver", str_vec)) {to_array_value(this->energyAver, str_vec); isLoad[10] = true;}
            if(iter->getFieldValue(parent, "energyMax", str_vec)) {to_array_value(this->energyMax, str_vec); isLoad[11] = true;}

            ind++;
        }
    }
}

#endif // TRAINBP_H
