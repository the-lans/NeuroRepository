#ifndef INEURONET_H
#define INEURONET_H

#include "inlayer.h"
#include "nlayertanh.h"
#include "nlayersoftsign.h"
#include "nlayerarctg.h"
#include "nlayerlinear.h"
#include "nexamples.h"
#include "iobjectecsv.h"
#include <iostream>
#include <ctime>

template <typename NType>
class INeuroNet: public IObjectECSV
{
public:
    INeuroNet();
    INeuroNet(INeuroNet<NType>& obj);
    INeuroNet<NType>& operator=(const INeuroNet<NType>& obj);
    INeuroNet(int size);
    virtual ~INeuroNet();
public:
    NExamples<NType>* examples; //Примеры
    NArray<INLayer<NType>*> lay; //Структура нейросети: слои
    NArray<NLayerType> typeLay; //Структура нейросети: Типы слоёв
    NArray<int> lenNeurons; //Структура нейросети: Количество нейронов
protected:
    NType valueWeight; //Структура нейросети: параметр инициализации весов
    NArray<NType> derivEnrg; //Производная целевой функции
    NType energyRegularization; //Энергия регуляризации
public:
    void setValueWeight(NType value);
    NType getValueWeight();
    int getLenIn();
    int getLenOut();
    NType getEnergyRegularization();
public:
    void copyNet(INeuroNet<NType>* obj); //Копирование структуры нейросети
    virtual NArray<NType>* run(NArray<NType>* X); //Функционирование нейросети
    virtual void runExample(int pos); //Выполнение примера
    virtual void runExamples(NSetType st); //Выполнение примеров
    INLayer<NType>* newLayer(NLayerType layType); //Добавление нового слоя
    virtual void deinit(); //Деинициализация
    virtual void init(); //Инициализация
    virtual void init(NArray<int>& num, NArray<NLayerType>& layType); //Инициализация
    virtual void init_struct(); //Инициализация структуры
    void emtyLayer(); //Очистить слои
    virtual void ginit(); //Инициализация Нгуен-Видроу
    virtual void ginit(NArray<int>& num, NArray<NLayerType>& layType); //Инициализация Нгуен-Видроу
    virtual void ginit_struct(); //Инициализация структуры
    virtual void runEnergy(NSetType st); //Расчёт энергии примеров
    virtual NType funcEnergy(NExample<NType>* exm); //Целевая функция
    virtual NType funcRegularization(); //Функция регуляризации
    virtual NArray<NType>& derivEnergy(NExample<NType>* exm); //Производная целевой функции
    void updateTypeLay(); //Обновить типы слоёв
public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
INeuroNet<NType>::INeuroNet()
{
    this->valueWeight = 0;
    this->examples = nullptr;
    this->energyRegularization = 0;
}

template <typename NType>
INeuroNet<NType>::INeuroNet(INeuroNet<NType>& obj):
    lay(obj.lay.getLength())
{
    //Копирование слоёв
    INLayer<NType>** pLay = obj.lay.getData();
    INLayer<NType>* layer;
    for(int i = 0; i < obj.lay.getLength(); i++)
    {
        layer = this->newLayer(pLay[i]->getType());
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    //Копирование примеров
    this->examples = obj.examples;
    this->typeLay = obj.typeLay;
    this->lenNeurons = obj.lenNeurons;

    //Копирование других полей
    this->valueWeight = obj.getValueWeight();
    //this->examples->outrun = &(layer->output);
    this->energyRegularization = obj.getEnergyRegularization();
}

template <typename NType>
INeuroNet<NType>& INeuroNet<NType>::operator=(const INeuroNet<NType>& obj)
{
    //Копирование слоёв
    this->emtyLayer();
    this->lay.resize(obj.lay.getLength());

    INLayer<NType>** pLay = obj.lay.getData();
    INLayer<NType>* layer;
    for(int i = 0; i < obj.lay.getLength(); i++)
    {
        layer = this->newLayer(pLay[i]->getType());
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    //Копирование примеров
    this->examples = obj.examples;
    this->typeLay = obj.typeLay;
    this->lenNeurons = obj.lenNeurons;

    //Копирование других полей
    this->valueWeight = obj.getValueWeight();
    //this->examples->outrun = &(layer->output);
    this->energyRegularization = obj.getEnergyRegularization();

    return *this;
}

template <typename NType>
INeuroNet<NType>::INeuroNet(int size):
    lay(size)
{
    this->valueWeight = 0;
    this->examples = nullptr;
    this->energyRegularization = 0;
}

template <typename NType>
INeuroNet<NType>::~INeuroNet()
{
    this->deinit();
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
int INeuroNet<NType>::getLenIn()
{
    return this->lay[0]->weigth.getLenRow();
}

template <typename NType>
int INeuroNet<NType>::getLenOut()
{
    int len = this->lay.getLength();
    return this->lay[len-1]->weigth.getLenColumn();
}

template <typename NType>
NType INeuroNet<NType>::getEnergyRegularization()
{
    return this->energyRegularization;
}


template <typename NType>
void INeuroNet<NType>::copyNet(INeuroNet<NType>* obj)
{
    //Копирование слоёв
    this->emtyLayer();
    this->lay.resize(obj->lay.getLength());

    INLayer<NType>** pLay = obj->lay.getData();
    INLayer<NType>* layer;
    for(int i = 0; i < obj->lay.getLength(); i++)
    {
        layer = this->newLayer(pLay[i]->getType());
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    //Копирование других полей
    this->valueWeight = obj->getValueWeight();
    //this->examples->outrun = &(layer->output);
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
    if(this->examples != nullptr) {this->examples->outrun = input;}

    return input;
}

template <typename NType>
void INeuroNet<NType>::runExample(int pos)
{
    //Функционирование нейросети
    NExample<NType>* exm = this->examples->get(pos);
    NArray<NType>* input = &(exm->input);
    this->run(input);

    //Копирование выходного вектора
    exm->outrun.clear();
    NType* pOutrun = this->examples->outrun->getData();
    for(int i = 0; i < this->examples->outrun->getLength(); i++)
    {
        exm->outrun.push(pOutrun[i]);
    }

    //Расчёт целевой функции
    exm->setEnergy(this->funcEnergy(exm));
}

template <typename NType>
void INeuroNet<NType>::runExamples(NSetType st)
{
    for(int i = 0; i < this->examples->getLength(); i++)
    {
        this->runExample(i);
    }
    this->runEnergy(st);
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
void INeuroNet<NType>::deinit()
{
    this->emtyLayer();
    this->lenNeurons.clear();
    this->typeLay.clear();
    this->derivEnrg.clear();
}

template <typename NType>
void INeuroNet<NType>::init()
{
    if(this->examples != nullptr && this->examples->getEndset() == 0) {this->examples->setEndset(this->examples->getLength());}

    INLayer<NType>** pLay = this->lay.getData();
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay[i]->init(this->valueWeight);
    }
}

template <typename NType>
void INeuroNet<NType>::init(NArray<int>& num, NArray<NLayerType>& layType)
{
    this->lenNeurons = num;
    this->typeLay = layType;
    if(this->examples != nullptr && this->examples->getEndset() == 0) {this->examples->setEndset(this->examples->getLength());}
    this->init_struct();
}

template <typename NType>
void INeuroNet<NType>::init_struct()
{
    this->emtyLayer();

    int lenLayer = this->lenNeurons.getLength()-1;
    this->lay.resize(lenLayer);
    INLayer<NType>* layer;

    for(int i = 0; i < lenLayer; i++)
    {
        layer = this->newLayer(this->typeLay[i]);
        layer->init(this->lenNeurons[i], this->lenNeurons[i+1], this->valueWeight);
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
void INeuroNet<NType>::ginit()
{
    if(this->examples != nullptr && this->examples->getEndset() == 0) {this->examples->setEndset(this->examples->getLength());}

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
    this->lenNeurons = num;
    this->typeLay = layType;
    if(this->examples != nullptr && this->examples->getEndset() == 0) {this->examples->setEndset(this->examples->getLength());}
    this->ginit_struct();
}

template <typename NType>
void INeuroNet<NType>::ginit_struct()
{
    this->emtyLayer();

    srand(time(0));
    int lenLayer = this->lenNeurons.getLength()-1;
    this->lay.resize(lenLayer);
    INLayer<NType>* layer;

    for(int i = 0; i < lenLayer; i++)
    {
        layer = this->newLayer(this->typeLay[i]);
        layer->ginit(this->lenNeurons[i], this->lenNeurons[i+1], this->valueWeight);
        this->lay.push(layer);
    }
}

template <typename NType>
void INeuroNet<NType>::runEnergy(NSetType st)
{
    int i;
    int begExm = this->examples->getBeginset();
    int endExm = this->examples->getEndset();
    if(st == NSetType::NSetTrain)
    {
        begExm = this->examples->getBeginset();
        endExm = this->examples->getTestset() - 1;
    }
    else if(st == NSetType::NSetTest)
    {
        begExm = this->examples->getTestset();
        endExm = this->examples->getEndset() - 1;
    }
    int lenExm = endExm - begExm + 1;

    NExample<NType>** pData = this->examples->getData();
    NType enrgSum = 0;
    NType enrgMax;

    for(i = begExm; i <= endExm; i++)
    {
        enrgSum += pData[i]->getEnergy();
    }
    this->examples->setEnergySum(enrgSum);
    this->examples->setEnergyAver(lenExm == 0 ? 0 : enrgSum / lenExm);

    this->examples->setEnergyMax(lenExm == 0 ? 0 : pData[begExm]->getEnergy());
    for(i = begExm+1; i <= endExm; i++)
    {
        enrgMax = pData[i]->getEnergy();
        if(this->examples->getEnergyMax() < enrgMax)
        {
            this->examples->setEnergyMax(enrgMax);
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
    for(int k = 0; k < this->lay.getLength(); k++)
    {
        this->energyRegularization += this->lay[k]->funcRegularization();
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
void INeuroNet<NType>::updateTypeLay()
{
    this->typeLay.clear();
    INLayer<NType>* pLay;
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay = this->lay[i];
        this->typeLay.push(pLay->getType());
    }
}

template <typename NType>
void INeuroNet<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NArray<unsigned int> enm_arr;
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    string field;

    this->updateTypeLay();
    this->typeLay.toUInt(enm_arr);

    str_val = to_string(this->valueWeight); dt.addElement(parent, "valueWeight", str_val, typeid(NType).name());
    str_val = to_string(this->energyRegularization); dt.addElement(parent, "energyRegularization", str_val, typeid(NType).name());
    to_array_string(str_vec, enm_arr); dt.addElement(parent, "typeLay", str_vec, typeid(int).name());
    to_array_string(str_vec, this->lenNeurons); dt.addElement(parent, "lenNeurons", str_vec, typeid(int).name());

    field = parent + ".lay";
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        this->lay[i]->saveECSV(dt, field);
    }
}

template <typename NType>
void INeuroNet<NType>::loadECSV(DataECSV& dt, string& parent)
{
    NArray<unsigned int> enm_arr;
    INLayer<NType>* pLay;
    StructECSV* iter;
    //unsigned int enm;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    string field;
    vector<string> subpath;
    bool* isLoad = newbool(4, false);

    //Выполнение операций перед загрузкой
    INeuroNet<NType>::deinit();

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        pLay = this->newLayer(NLayerType::NFuncLinear);
        to_matrix_value(pLay->weigth, iter->mtrx);
    }
    else
    {
        //Поля класса
        size_t ind = dt.getShift();
        while(!booland(isLoad, 4) && ind < dt.modules.size())
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "valueWeight", str_val)) {to_value(this->valueWeight, str_val); isLoad[0] = true;}
            if(iter->getFieldValue(parent, "energyRegularization", str_val)) {to_value(this->energyRegularization, str_val); isLoad[1] = true;}
            if(iter->getFieldValue(parent, "typeLay", str_vec)) {to_array_value(enm_arr, str_vec); this->typeLay.convertUInt(enm_arr); isLoad[2] = true;}
            if(iter->getFieldValue(parent, "lenNeurons", str_vec)) {to_array_value(this->lenNeurons, str_vec); isLoad[3] = true;}

            ind++;
        }

        //Субполя класса
        if(ind >= dt.modules.size()) {ind = dt.getShift();}
        dt.setShift(ind);

        field = parent + ".lay";
        dt.splitPath(subpath, field);
        int ind_lay = 0;

        while(ind < dt.modules.size())
        {
            iter = dt.modules[ind];
            if(iter->cmpPath(subpath))
            {
                pLay = this->newLayer((NLayerType)this->typeLay[ind_lay]);
                pLay->loadECSV(dt, field);
                this->lay.push(pLay);
                ind_lay++;
                ind = dt.getShift();
            }
            else
            {
                ind++; dt.setShift(ind);
            }
        }
    }
}

#endif // INEURONET_H
