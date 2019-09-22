#ifndef INEURONET_H
#define INEURONET_H

#include "inlayer.h"
#include "nexamples.h"
//#include "iobjectecsv.h"
//#include <iostream>
//#include <ctime>
#include "ineuronetstruct.h"

template <typename NType>
class INeuroNet: public IObjectECSV
{
public:
    INeuroNet();
    INeuroNet(int size);
    INeuroNet(INeuroNet<NType>& obj);
    INeuroNet<NType>& operator=(const INeuroNet<NType>& obj);
    virtual ~INeuroNet();

public:
    NExamples<NType>* examples; //Примеры
    NExamples<NType>* tmp_examples; //Временное хранилище данных для параллельного обучения
    NArray<INLayer<NType>*> lay; //Структура нейросети: слои
    NArray<NLayerType> typeLay; //Структура нейросети: Типы слоёв
    NArray<int> lenNeurons; //Структура нейросети: Количество нейронов
    NArray<NType> koefLay; //Структура нейросети: Коэффициенты слоя
    NArray<NType> shot; //Снимок нейросети
    NArray<bool> lock; //Маска для блокировки нейронов от изменения
    bool blLock; //Выключатель блокировки нейронов для нейросети в целом
    NArray<NType> dropout; //Параметр выключения нейронов dropout

protected:
    NType valueWeight; //Структура нейросети: параметр инициализации весов
    NArray<NType> derivEnrg; //Производная целевой функции
    NArray<NType> derivLay; //Производная функции слоя
    NType energyRegularization; //Энергия регуляризации
    int shiftout; //Структура нейросети: cмещение выходного вектора примеров для нейросети
    NInitType typeInit; //Структура нейросети: тип инициализации
    bool extrn; //Внешние данные
    NTargetFunc typeTarget; //Вид целевой функции
    NError erCheck; //Отслеживание ошибок
    NStatusNet status; //Статус нейросети

public:
    void (*activationLay)(NArray<NType>&); //Функция активации последнего слоя
    void (*derivActivationLay)(NArray<NType>&, NArray<NType>&, NArray<NType>&); //Производная функции активации последнего слоя
    NType (*target)(NType*, NType*, int); //Целевая функция
    NType (*targetAver)(const NType&, int);
    NType (*targetMax)(const NType&);
    void (*derivTarget)(NArray<NType>&, NType*, NType*, int); //Производная целевой функции

public:
    void setValueWeight(NType value);
    NType getValueWeight();
    int getLenIn();
    int getLenOut();
    NType getEnergyRegularization();
    void setShiftout(int shift);
    int getShiftout();
    void setTypeInit(NInitType param);
    NInitType getTypeInit();
    void setStatus(NStatusNet param);
    NStatusNet getStatus();
    void setExtern(bool param);
    bool getExtern();
    void setTypeTarget(NTargetFunc param);
    NTargetFunc getTypeTarget();
    NError getCheck();

public:
    void copyNet(INeuroNet<NType>* obj); //Копирование структуры нейросети
    void init_slay(); //Задаём множители
    void initKoefDropout(); //Инициализация коэффициента dropout
    virtual NArray<NType>* run(NArray<NType>* X); //Функционирование нейросети
    virtual NArray<NType>* run_dropout(NArray<NType>* X);
    virtual void runExample(NExample<NType>* tmp_exm); //Выполнение примера
    virtual void runExampleDropout(NExample<NType>* tmp_exm);
    virtual void runExamples(NSetType st); //Выполнение примеров
    static INLayer<NType>* newLayer(NLayerType layType); //Добавление нового слоя
    virtual void deinit(); //Деинициализация
    void init(); //Инициализация
    void init(NArray<int>& num, NArray<NLayerType>& layType); //Инициализация
    void init_struct(); //Инициализация структуры
    void update_struct(); //Обновление структуры нейросети
    void emtyLayer(); //Очистить слои
    void ginit(); //Инициализация Нгуен-Видроу
    void ginit(NArray<int>& num, NArray<NLayerType>& layType); //Инициализация Нгуен-Видроу
    void ginit_struct(); //Инициализация структуры
    virtual void runEnergy(NSetType st); //Расчёт энергии примеров
    NType funcEnergy(NExample<NType>* tmp_exm); //Целевая функция
    NArray<NType>& derivEnergy(NExample<NType>* tmp_exm); //Производная целевой функции
    void funcActivLay(NArray<NType>& out); //Функция активации слоя
    NArray<NType>* derivActivLay(NExample<NType>* tmp_exm); //Производная функции активации слоя
    virtual NType funcRegularization(); //Функция регуляризации
    void updateTypeLay(); //Обновить типы слоёв
    void updateLenNeurons(); //Обновить структуру нейросети
    void updateKoefLay(); //Обновить коэффициенты слоёв
    void updateDropout(); //Обновить dropout
    void updateFuncNeurons(); //Обновление функций
    int getLenNeurons(); //Общее количество нейронов
    int getLenNeuronsCalc(); //Общее расчётное количество нейронов
    int getLenWeight(); //Общее количество связей
    int getLenWeightCalc(); //Общее расчётное количество связей
    void makeShot(); //Сделать снимок нейросети
    void restorShot(); //Восстановить нейросеть из снимка
    bool isShotData(); //Определяет единство структуры данных и снимка нейросети
    void updateData(); //Обновить указатели на данные
    void deleteData(); //Удаление альтернативной структуры данных
    void createData(); //Создание альтернативной структуры данных
    bool check(); //Проверить на ошибки
    int getIndexOutNumClass(); //Индекс для вектора outputNumClass
    NType maxOut(); //Максимальный выход нейросети
    int maxArg();
    int maxArg(NArray<NType>& vecout);
    NType maxOut(NArray<NType>& vecout);
    int actNet(NArray<NType>& outrun); //Действие нейросети при онлайн обучении

public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
INeuroNet<NType>::INeuroNet()
{
    this->valueWeight = 0;
    this->examples = nullptr;
    this->tmp_examples = nullptr;
    this->energyRegularization = 0;
    this->shiftout = 0;
    this->typeInit = NInitType::NInitNone;
    this->blLock = false;
    this->extrn = true;
    this->typeTarget = NTargetFunc::NTargetNone;
    this->activationLay = nullptr;
    this->derivActivationLay = nullptr;
    this->target = nullptr;
    this->targetAver = nullptr;
    this->targetMax = nullptr;
    this->derivTarget = nullptr;
    this->erCheck = NError::NErrorNone;
    this->status = NStatusNet::NStatusCreate;
}

template <typename NType>
INeuroNet<NType>::INeuroNet(int size):
    lay(size)
{
    this->valueWeight = 0;
    this->examples = nullptr;
    this->tmp_examples = nullptr;
    this->energyRegularization = 0;
    this->shiftout = 0;
    this->typeInit = NInitType::NInitNone;
    this->blLock = false;
    this->extrn = true;
    this->typeTarget = NTargetFunc::NTargetNone;
    this->activationLay = nullptr;
    this->derivActivationLay = nullptr;
    this->target = nullptr;
    this->targetAver = nullptr;
    this->targetMax = nullptr;
    this->derivTarget = nullptr;
    this->erCheck = NError::NErrorNone;
    this->status = NStatusNet::NStatusCreate;
}

template <typename NType>
INeuroNet<NType>::INeuroNet(INeuroNet<NType>& obj):
    lay(obj.lay.getLength())
{
    this->blLock = obj.blLock;
    this->lock = obj.lock;
    this->shot = obj.shot;
    this->extrn = obj.getExtern();
    this->activationLay = obj.activationLay;
    this->derivActivationLay = obj.derivActivationLay;
    this->target = obj.target;
    this->targetAver = obj.targetAver;
    this->targetMax = obj.targetMax;
    this->derivTarget = obj.derivTarget;

    //Копирование слоёв
    INLayer<NType>** pLay = obj.lay.getData();
    INLayer<NType>* layer;
    for(int i = 0; i < obj.lay.getLength(); i++)
    {
        layer = INeuroNet::newLayer(pLay[i]->getType());
        //layer->setExtern(pLay[i]->getExtern());
        layer->setExtern(this->extrn);
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    //Копирование других полей
    this->valueWeight = obj.getValueWeight();
    //tmp_examples->outrun = &(layer->output);
    this->energyRegularization = obj.getEnergyRegularization();
    this->shiftout = obj.getShiftout();
    this->typeInit = obj.getTypeInit();
    this->status = obj.getStatus();

    this->updateData();
    this->derivEnrg.init(this->getLenOut(), 0);
    this->derivLay.init(this->getLenOut(), 0);

    //Копирование примеров
    this->examples = obj.examples;
    this->tmp_examples = obj.tmp_examples;
    this->typeLay = obj.typeLay;
    this->lenNeurons = obj.lenNeurons;
    this->koefLay = obj.koefLay;
    this->dropout = obj.dropout;
}

template <typename NType>
INeuroNet<NType>& INeuroNet<NType>::operator=(const INeuroNet<NType>& obj)
{
    this->copyNet(&obj);
    this->examples = obj.examples; //Копирование примеров
    this->tmp_examples = obj.tmp_examples;
    return *this;
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
    return this->lay.endElement()->weigth.getLenColumn();
}

template <typename NType>
NType INeuroNet<NType>::getEnergyRegularization()
{
    return this->energyRegularization;
}

template <typename NType>
void INeuroNet<NType>::setShiftout(int shift)
{
    this->shiftout = shift;
}

template <typename NType>
int INeuroNet<NType>::getShiftout()
{
    return this->shiftout;
}

template <typename NType>
void INeuroNet<NType>::setTypeInit(NInitType param)
{
    this->typeInit = param;
}

template <typename NType>
NInitType INeuroNet<NType>::getTypeInit()
{
    return this->typeInit;
}

template <typename NType>
void INeuroNet<NType>::setStatus(NStatusNet param)
{
    this->status = param;
}

template <typename NType>
NStatusNet INeuroNet<NType>::getStatus()
{
    return this->status;
}

template <typename NType>
void INeuroNet<NType>::setExtern(bool param)
{
    this->extrn = param;
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        this->lay[i]->setExtern(param);
    }
}

template <typename NType>
bool INeuroNet<NType>::getExtern()
{
    return this->extrn;
}

template <typename NType>
void INeuroNet<NType>::setTypeTarget(NTargetFunc param)
{
    this->typeTarget = param;
}

template <typename NType>
NTargetFunc INeuroNet<NType>::getTypeTarget()
{
    return this->typeTarget;
}

template <typename NType>
NError INeuroNet<NType>::getCheck()
{
    return this->erCheck;
}


template <typename NType>
void INeuroNet<NType>::copyNet(INeuroNet<NType>* obj)
{
    //Копирование слоёв
    this->emtyLayer();
    this->lay.resize(obj->lay.getLength());

    this->blLock = obj->blLock;
    this->lock = obj->lock;
    this->shot = obj->shot;
    //this->extrn = obj->extrn;
    this->activationLay = obj->activationLay;
    this->derivActivationLay = obj->derivActivationLay;
    this->target = obj->target;
    this->targetAver = obj->targetAver;
    this->targetMax = obj->targetMax;
    this->derivTarget = obj->derivTarget;

    INLayer<NType>** pLay = obj->lay.getData();
    INLayer<NType>* layer;
    for(int i = 0; i < obj->lay.getLength(); i++)
    {
        layer = INeuroNet::newLayer(pLay[i]->getType());
        //layer->setExtern(pLay[i]->getExtern());
        layer->setExtern(this->extrn);
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    //Копирование других полей
    this->valueWeight = obj->getValueWeight();
    //tmp_examples->outrun = &(layer->output);
    this->energyRegularization = obj->getEnergyRegularization();
    this->shiftout = obj->getShiftout();
    this->typeInit = obj->getTypeInit();
    this->status = obj->getStatus();
    this->typeLay = obj->typeLay;
    this->lenNeurons = obj->lenNeurons;
    this->koefLay = obj->koefLay;
    this->dropout = obj->dropout;

    this->updateData();
    this->derivEnrg.init(this->getLenOut(), 0);
    this->derivLay.init(this->getLenOut(), 0);
}

template <typename NType>
void INeuroNet<NType>::init_slay()
{
    INLayer<NType>** pLay = this->lay.getData();
    for(int ind = 0; ind < this->lay.getLength(); ind++)
    {
        if(pLay[ind]->getDropout() > 0) {pLay[ind]->init_slay();}
    }
    //cout << "slay = " << pLay[0]->slay.sumElements() << "\n";
}

template <typename NType>
void INeuroNet<NType>::initKoefDropout()
{
    INLayer<NType>** pLay = this->lay.getData();
    for(int ind = 0; ind < this->lay.getLength(); ind++)
    {
        pLay[ind]->initKoefDropout();
    }
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

    this->funcActivLay(*input);
    if(tmp_examples != nullptr) {tmp_examples->outrun = input;}

    return input;
}

template <typename NType>
NArray<NType>* INeuroNet<NType>::run_dropout(NArray<NType>* X)
{
    INLayer<NType>** pLay = this->lay.getData();
    NArray<NType>* input = X;

    for(int i = 0; i < this->lay.getLength(); i++)
    {
        if(pLay[i]->getDropout() > 0) {pLay[i]->run_dropout(input);}
        else {pLay[i]->run(input);}
        input = &(pLay[i]->output);
    }

    this->funcActivLay(*input);
    if(tmp_examples != nullptr) {tmp_examples->outrun = input;}

    return input;
}

template <typename NType>
void INeuroNet<NType>::runExample(NExample<NType>* tmp_exm)
{
    //Функционирование нейросети
    this->run(&(tmp_exm->original->input));

    //Копирование выходного вектора
    tmp_exm->outrun.memCopy(tmp_examples->outrun->getData(), shiftout, tmp_examples->outrun->getLength());

    //Расчёт целевой функции
    this->funcEnergy(tmp_exm);
    //tmp_exm->setEnergy(this->funcEnergy(tmp_exm));
}

template <typename NType>
void INeuroNet<NType>::runExampleDropout(NExample<NType>* tmp_exm)
{
    //Функционирование нейросети
    this->run_dropout(&(tmp_exm->original->input));

    //Копирование выходного вектора
    tmp_exm->outrun.memCopy(tmp_examples->outrun->getData(), shiftout, tmp_examples->outrun->getLength());

    //Расчёт целевой функции
    this->funcEnergy(tmp_exm);
    //tmp_exm->setEnergy(this->funcEnergy(tmp_exm));
}

template <typename NType>
void INeuroNet<NType>::runExamples(NSetType st)
{
    NExample<NType>* tmp_exm;
    int begExm = tmp_examples->getBeginset();
    int endExm = tmp_examples->getEndset();
    bool isOnline = tmp_examples->getIsOnline();
    int action;

    if(st == NSetType::NSetTrain)
    {
        if(!isOnline) {begExm = tmp_examples->getBeginset();}
        endExm = tmp_examples->getTestset();
    }
    else if(st == NSetType::NSetTest)
    {
        if(!isOnline) {begExm = tmp_examples->getTestset();}
        endExm = tmp_examples->getValidset();
    }
    else if(st == NSetType::NSetValidate)
    {
        if(!isOnline) {begExm = tmp_examples->getValidset();}
        endExm = tmp_examples->getEndset();
    }

    if(isOnline) {tmp_examples->initState();}
    for(int pos = begExm; pos < endExm-1; pos++)
    {
        tmp_exm = tmp_examples->get(pos);
        this->runExample(tmp_exm);
        if(isOnline)
        {
            action = this->actNet(tmp_exm->outrun);
            tmp_examples->nextState(action, pos);
        }
    }

    if(endExm > begExm)
    {
        tmp_exm = tmp_examples->get(endExm-1);
        this->runExample(tmp_exm);
    }
    if(isOnline) {tmp_examples->deinitState();}

    this->runEnergy(st);
}

template <typename NType>
INLayer<NType>* INeuroNet<NType>::newLayer(NLayerType layType)
{
    INLayer<NType>* layer = new INLayer<NType>();
    layer->setType(layType);

         if(layType == NLayerType::NFuncTanh)       {layer->setTypeDerivat(NLayerDerivat::NDerivatOut); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncSoftsign)   {layer->setTypeDerivat(NLayerDerivat::NDerivatOut); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncArctg)      {layer->setTypeDerivat(NLayerDerivat::NDerivatSum); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncLinear)     {layer->setTypeDerivat(NLayerDerivat::NDerivatOut); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncStep)       {layer->setTypeDerivat(NLayerDerivat::NDerivatSum); layer->setKoef(0);}
    else if(layType == NLayerType::NFuncSoftStep)   {layer->setTypeDerivat(NLayerDerivat::NDerivatOut); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncReLU)       {layer->setTypeDerivat(NLayerDerivat::NDerivatSum); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncPReLU)      {layer->setTypeDerivat(NLayerDerivat::NDerivatSum); layer->setKoef(0.01);}
    else if(layType == NLayerType::NFuncELU)        {layer->setTypeDerivat(NLayerDerivat::NDerivatOut); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncSoftPlus)   {layer->setTypeDerivat(NLayerDerivat::NDerivatSum); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncSin)        {layer->setTypeDerivat(NLayerDerivat::NDerivatSum); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncSinc)       {layer->setTypeDerivat(NLayerDerivat::NDerivatNone); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncGaussian)   {layer->setTypeDerivat(NLayerDerivat::NDerivatSum); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncLinearInt)  {layer->setTypeDerivat(NLayerDerivat::NDerivatOut); layer->setKoef(1);}
    else if(layType == NLayerType::NFuncSoftMax)    {layer->setTypeDerivat(NLayerDerivat::NDerivatOut); layer->setKoef(1);}

    layer->updateFuncNeurons();

    return layer;
}

template <typename NType>
void INeuroNet<NType>::deinit()
{
    if(tmp_examples != examples && tmp_examples != nullptr)
    {
        tmp_examples->deinit();
        tmp_examples = examples;
    }
    this->emtyLayer();
    this->typeLay.clear();
    this->lenNeurons.clear();
    this->koefLay.clear();
    this->derivEnrg.clear();
    this->derivLay.clear();
    this->lock.clear();
    this->dropout.clear();
}

template <typename NType>
void INeuroNet<NType>::init()
{
    this->energyRegularization = 0;
    if(tmp_examples != nullptr && tmp_examples->getEndset() == 0) {tmp_examples->setEndset(tmp_examples->getLength());}
    this->updateData();

    int pos = 0;
    INLayer<NType>** pLay = this->lay.getData();
    bool* pLock = this->lock.getData();
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        if(this->blLock) {pLay[i]->lock = pLock + pos;} else {pLay[i]->lock = nullptr;}
        if(i >= this->koefLay.getLength()) {this->koefLay.push(pLay[i]->getKoef());}
        if(i >= this->dropout.getLength()) {this->dropout.push(pLay[i]->getDropout());}
        pLay[i]->init(this->valueWeight);
        pos += pLay[i]->getLenNeurons();
    }

    this->derivEnrg.init(this->getLenOut(), 0);
    this->derivLay.init(this->getLenOut(), 0);
    //this->lock.init(this->getLenNeurons(), false);
    this->updateFuncNeurons();
    this->status = NStatusNet::NStatusInit;
}

template <typename NType>
void INeuroNet<NType>::init(NArray<int>& num, NArray<NLayerType>& layType)
{
    this->typeLay = layType;
    this->lenNeurons = num;
    if(tmp_examples != nullptr && tmp_examples->getEndset() == 0) {tmp_examples->setEndset(tmp_examples->getLength());}
    this->init_struct();
}

template <typename NType>
void INeuroNet<NType>::init_struct()
{
    int pos = 0;
    INLayer<NType>* layer;
    this->emtyLayer();
    this->shot.init(this->getLenWeightCalc(), 0);

    //Создание слоёв
    for(int i = 0; i < this->lenNeurons.getLength()-1; i++)
    {
        layer = INeuroNet::newLayer(this->typeLay[i]);
        layer->setExtern(this->extrn);
        this->lay.push(layer);
    }

    //Инициализация слоёв
    NType* pShot = this->shot.getData();
    for(int i = 0; i < this->lenNeurons.getLength()-1; i++)
    {
        layer = this->lay[i];
        if(i >= this->koefLay.getLength()) {this->koefLay.push(layer->getKoef());}
        if(i >= this->dropout.getLength()) {this->dropout.push(layer->getDropout());}
        layer->shotlay = (layer->getExtern() ? pShot + pos : nullptr);
        layer->setKoef(this->koefLay[i]);
        layer->setDropout(this->dropout[i]);
        layer->init(this->lenNeurons[i], this->lenNeurons[i+1], this->valueWeight);
        pos += (layer->getExtern() ? layer->getSizeData() : 0);
    }

    this->derivEnrg.init(this->getLenOut(), 0);
    this->derivLay.init(this->getLenOut(), 0);
    this->updateFuncNeurons();
    this->status = NStatusNet::NStatusInit;
}

template <typename NType>
void INeuroNet<NType>::update_struct()
{
    int pos = 0;
    INLayer<NType>* layer;

    //Инициализация слоёв
    NType* pShot = this->shot.getData();
    for(int i = 0; i < this->lenNeurons.getLength()-1; i++)
    {
        layer = this->lay[i];
        if(i >= this->koefLay.getLength()) {this->koefLay.push(layer->getKoef());}
        if(i >= this->dropout.getLength()) {this->dropout.push(layer->getDropout());}
        layer->shotlay = (layer->getExtern() ? pShot + pos : nullptr);
        layer->setKoef(this->koefLay[i]);
        layer->setDropout(this->dropout[i]);
        //layer->init(this->lenNeurons[i], this->lenNeurons[i+1], this->valueWeight);
        layer->update_struct(this->lenNeurons[i], this->lenNeurons[i+1]);
        pos += (layer->getExtern() ? layer->getSizeData() : 0);
    }

    this->derivEnrg.init(this->getLenOut(), 0);
    this->derivLay.init(this->getLenOut(), 0);
    this->updateFuncNeurons();
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
    this->shot.clear();
    this->status = NStatusNet::NStatusCreate;
}

template <typename NType>
void INeuroNet<NType>::ginit()
{
    if(tmp_examples != nullptr && tmp_examples->getEndset() == 0) {tmp_examples->setEndset(tmp_examples->getLength());}
    this->updateData();

    //srand(time(0));
    int pos = 0;
    INLayer<NType>** pLay = this->lay.getData();
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        if(this->blLock) {pLay[i]->lock = this->lock.getData() + pos;}
        else {pLay[i]->lock = nullptr;}
        if(i >= this->koefLay.getLength()) {this->koefLay.push(pLay[i]->getKoef());}
        if(i >= this->dropout.getLength()) {this->dropout.push(pLay[i]->getDropout());}
        pLay[i]->ginit(this->valueWeight);
        pos += pLay[i]->getLenNeurons();
    }
    this->derivEnrg.init(this->getLenOut(), 0);
    this->derivLay.init(this->getLenOut(), 0);
    this->updateFuncNeurons();
    this->status = NStatusNet::NStatusInit;
}

template <typename NType>
void INeuroNet<NType>::ginit(NArray<int>& num, NArray<NLayerType>& layType)
{
    this->typeLay = layType;
    this->lenNeurons = num;
    if(tmp_examples != nullptr && tmp_examples->getEndset() == 0) {tmp_examples->setEndset(tmp_examples->getLength());}
    this->ginit_struct();
}

template <typename NType>
void INeuroNet<NType>::ginit_struct()
{
    int i, pos = 0;
    this->emtyLayer();

    //srand(time(0));
    int lenLayer = this->lenNeurons.getLength()-1;
    this->lay.clear();
    this->shot.init(this->getLenWeightCalc());
    INLayer<NType>* layer;

    //Создание слоёв
    for(i = 0; i < lenLayer; i++)
    {
        layer = INeuroNet::newLayer(this->typeLay[i]);
        layer->setExtern(this->extrn);
        this->lay.push(layer);
    }

    //Инициализация слоёв
    NType* pShot = this->shot.getData();
    for(i = 0; i < lenLayer; i++)
    {
        layer = this->lay[i];
        if(i >= this->koefLay.getLength()) {this->koefLay.push(layer->getKoef());}
        if(i >= this->dropout.getLength()) {this->dropout.push(layer->getDropout());}
        //pos += layer->updateData(pShot, pos);
        layer->shotlay = (layer->getExtern() ? pShot + pos : nullptr);
        layer->setKoef(this->koefLay[i]);
        layer->setDropout(this->dropout[i]);
        layer->ginit(this->lenNeurons[i], this->lenNeurons[i+1], this->valueWeight);
        pos += (layer->getExtern() ? layer->getSizeData() : 0);
    }
    this->derivEnrg.init(this->getLenOut(), 0);
    this->derivLay.init(this->getLenOut(), 0);
    this->updateFuncNeurons();
    this->status = NStatusNet::NStatusInit;
}

template <typename NType>
void INeuroNet<NType>::runEnergy(NSetType st)
{
    int i;
    int begExm = tmp_examples->getBeginset();
    int endExm = tmp_examples->getEndset();
    bool isOnline = tmp_examples->getIsOnline();

    if(st == NSetType::NSetTrain)
    {
        if(!isOnline) {begExm = tmp_examples->getBeginset();}
        endExm = tmp_examples->getTestset();
    }
    else if(st == NSetType::NSetTest)
    {
        if(!isOnline) {begExm = tmp_examples->getTestset();}
        endExm = tmp_examples->getValidset();
    }
    else if(st == NSetType::NSetValidate)
    {
        if(!isOnline) {begExm = tmp_examples->getValidset();}
        endExm = tmp_examples->getEndset();
    }
    int lenExm = endExm - begExm;

    NExample<NType>** pData = tmp_examples->getData();
    NType enrgSum = 0;
    NType enrgMax;

    for(i = begExm; i < endExm; i++)
    {
        enrgSum += pData[i]->getEnergy();
    }
    tmp_examples->setEnergySum(enrgSum);

    if(this->targetAver == nullptr)
    {
        tmp_examples->setEnergyAver(lenExm == 0 ? 0 : enrgSum / lenExm);
    }
    else
    {
        tmp_examples->setEnergyAver(this->targetAver(enrgSum, lenExm));
    }

    enrgMax = (lenExm == 0 ? 0 : pData[begExm]->getEnergy());
    for(i = begExm+1; i < endExm; i++)
    {
        if(enrgMax < pData[i]->getEnergy())
        {
            enrgMax = pData[i]->getEnergy();
        }
    }

    if(this->targetMax == nullptr)
    {
        tmp_examples->setEnergyMax(enrgMax);
    }
    else
    {
        tmp_examples->setEnergyMax(this->targetMax(enrgMax));
    }
}

template <typename NType>
NType INeuroNet<NType>::funcEnergy(NExample<NType>* tmp_exm)
{
    NType* pOutput = tmp_exm->original->output.getData() + shiftout;
    NType* pOutrun = tmp_exm->outrun.getData() + shiftout;

    NType enrg = this->target(pOutput, pOutrun, getLenOut());
    tmp_exm->setEnergy(enrg);

    return enrg;
}

template <typename NType>
NArray<NType>& INeuroNet<NType>::derivEnergy(NExample<NType>* tmp_exm)
{
    NType* pOutput = tmp_exm->original->output.getData() + shiftout;
    NType* pOutrun = tmp_exm->outrun.getData() + shiftout;

    derivTarget(derivEnrg, pOutput, pOutrun, getLenOut());

    return derivEnrg;
}

template <typename NType>
void INeuroNet<NType>::funcActivLay(NArray<NType>& out)
{
    if(activationLay != nullptr) {activationLay(out);}
}

template <typename NType>
NArray<NType>* INeuroNet<NType>::derivActivLay(NExample<NType>* tmp_exm)
{
    if(derivActivationLay != nullptr)
    {
        derivActivationLay(derivLay, tmp_exm->original->output, tmp_exm->outrun);
        return &derivLay;
    }
    return nullptr;
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
void INeuroNet<NType>::updateTypeLay()
{
    if(this->lay.getLength() > 0)
    {
        this->typeLay.clear();
        INLayer<NType>* pLay;
        for(int i = 0; i < this->lay.getLength(); i++)
        {
            pLay = this->lay[i];
            this->typeLay.push(pLay->getType());
        }
    }
}

template <typename NType>
void INeuroNet<NType>::updateLenNeurons()
{
    if(this->lay.getLength() > 0)
    {
        this->lenNeurons.clear();
        INLayer<NType>* pLay;
        this->lenNeurons.push(this->getLenIn());
        for(int i = 0; i < this->lay.getLength(); i++)
        {
            pLay = this->lay[i];
            this->lenNeurons.push(pLay->getLenNeurons());
        }
    }
}

template <typename NType>
void INeuroNet<NType>::updateKoefLay()
{
    if(this->lay.getLength() > 0)
    {
        this->koefLay.clear();
        INLayer<NType>* pLay;
        for(int i = 0; i < this->lay.getLength(); i++)
        {
            pLay = this->lay[i];
            this->koefLay.push(pLay->getKoef());
        }
    }
}

template <typename NType>
void INeuroNet<NType>::updateDropout()
{
    if(this->lay.getLength() > 0)
    {
        this->dropout.clear();
        INLayer<NType>* pLay;
        for(int i = 0; i < this->lay.getLength(); i++)
        {
            pLay = this->lay[i];
            this->dropout.push(pLay->getKoef());
        }
    }
}

template <typename NType>
void INeuroNet<NType>::updateFuncNeurons()
{
    NLayerType lt = this->typeLay.endElement();

    if(lt == NLayerType::NFuncSoftMax)
    {
        this->activationLay = funcLaySoftMax;
        if(this->typeTarget == NTargetFunc::NTargetCross) {this->derivActivationLay = derivLaySoftMax;}
        else if(this->typeTarget == NTargetFunc::NTargetMSE) {this->derivActivationLay = nullptr;}
        else {this->derivActivationLay = nullptr;}
    }
    else
    {
        this->activationLay = nullptr;
    }

    if(this->typeTarget == NTargetFunc::NTargetMSE)
    {
        this->target = targetMSE;
        this->targetAver = targetMSEAver;
        this->targetMax = targetMSEMax;
        this->derivTarget = derivTargetMSE;
    }
    else if(this->typeTarget == NTargetFunc::NTargetCross)
    {
        this->target = targetCross;
        this->targetAver = nullptr;
        this->targetMax = nullptr;
        this->derivTarget = derivTargetCross;
    }
    else
    {
        this->target = nullptr;
        this->targetAver = nullptr;
        this->targetMax = nullptr;
        this->derivTarget = nullptr;
    }
}

template <typename NType>
int INeuroNet<NType>::getLenNeurons()
{
    INLayer<NType>* pLay;
    int num = 0;
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay = this->lay[i];
        num += pLay->getLenNeurons();
    }
    return num;
}

template <typename NType>
int INeuroNet<NType>::getLenNeuronsCalc()
{
    return this->lenNeurons.sumElements(1);
}

template <typename NType>
int INeuroNet<NType>::getLenWeight()
{
    INLayer<NType>* pLay;
    int num = 0;
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay = this->lay[i];
        num += pLay->getSizeData();
    }
    return num;
}

template <typename NType>
int INeuroNet<NType>::getLenWeightCalc()
{
    int num = 0;
    for(int i = 1; i < this->lenNeurons.getLength(); i++)
    {
        num += lenNeurons[i] * (lenNeurons[i-1] + 1);
    }
    return num;
}

template <typename NType>
void INeuroNet<NType>::makeShot()
{
    int k, i, j, len;
    INLayer<NType>* layer;
    NType* data;

    if(this->getExtern()) {throw std::out_of_range("INeuroNet<NType>::makeShot() : is extern");}
    this->shot.clear();

    for(k = 0; k < this->lay.getLength(); k++)
    {
        layer = this->lay[k];

        data = layer->weigth.getData();
        len = layer->weigth.getSizeColumn();
        for(i = 0; i < layer->weigth.getLenRow(); i++)
        {
            for(j = 0; j < layer->weigth.getLenColumn(); j++)
            {
                this->shot.push(data[i*len+j]);
            }
        }

        data = layer->bias.getData();
        for(i = 0; i < layer->bias.getLength(); i++)
        {
            this->shot.push(data[i]);
        }
    }
}

template <typename NType>
void INeuroNet<NType>::restorShot()
{
    int k, i, j, len, pos = 0;
    INLayer<NType>* layer;
    NType* data;

    for(k = 0; k < this->lay.getLength(); k++)
    {
        layer = this->lay[k];
        //if(layer->empty()) {layer->init(this->lenNeurons[k], this->lenNeurons[k+1], 0);}

        data = layer->weigth.getData();
        len = layer->weigth.getSizeColumn();
        for(i = 0; i < layer->weigth.getLenRow(); i++)
        {
            for(j = 0; j < layer->weigth.getLenColumn(); j++)
            {
                data[i*len+j] = this->shot.get(pos);
                pos++;
            }
        }

        data = layer->bias.getData();
        for(i = 0; i < layer->bias.getLength(); i++)
        {
            data[i] = this->shot.get(pos);
            pos++;
        }
    }
}

template <typename NType>
bool INeuroNet<NType>::isShotData()
{
    if(!this->shot.empty() && !this->lay.empty())
    {
        return this->shot.getData() == this->lay[0]->shotlay;
    }
    else
    {
        return false;
    }
}

template <typename NType>
void INeuroNet<NType>::updateData()
{
    int pos = 0;
    int len = this->shot.getLength();
    INLayer<NType>** pLay = this->lay.getData();
    NType* pShot = this->shot.getData();

    for(int i = 0; i < this->lay.getLength(); i++)
    {
        if(pLay[i]->getExtern())
        {
            pLay[i]->shotlay = pShot + pos;
            pLay[i]->updateData();
            pos += pLay[i]->getSizeData();
            if(pos > len) {throw std::out_of_range("INeuroNet<NType>::updateData() : index is out of range");}
        }
        else
        {
            pLay[i]->shotlay = nullptr;
        }
    }
}

template <typename NType>
void INeuroNet<NType>::deleteData()
{
    INLayer<NType>** pLay = this->lay.getData();

    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay[i]->deleteData();
        pLay[i]->setLock(true);
    }
    this->setExtern(true);
}

template <typename NType>
void INeuroNet<NType>::createData()
{
    INLayer<NType>** pLay = this->lay.getData();

    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay[i]->createData();
        pLay[i]->setLock(false);
    }
    this->setExtern(false);
}

template <typename NType>
bool INeuroNet<NType>::check()
{
    erCheck = NError::NErrorNone;
    INLayer<NType>** pLay = this->lay.getData();
    int lenlay = this->lay.getLength();

    if(lenlay > 0)
    {
        //Проверка структуры нейросети
        if(pLay[0]->weigth.getLenColumn() != pLay[0]->bias.getLength()) {erCheck = NError::NErrorBias;}
        for(int i = 1; i < lenlay && erCheck == NError::NErrorNone; i++)
        {
            if(pLay[i]->weigth.getLenRow() != pLay[i-1]->weigth.getLenColumn()) {erCheck = NError::NErrorStruct;}
            if(pLay[i]->weigth.getLenColumn() != pLay[i]->bias.getLength()) {erCheck = NError::NErrorBias;}
        }
        if(erCheck != NError::NErrorNone) {return false;}

        if(pLay[0]->weigth.getLenRow() != tmp_examples->getLenIn()) {erCheck = NError::NErrorStruct;}
        if(pLay[lenlay-1]->weigth.getLenColumn() != tmp_examples->getLenOut()) {erCheck = NError::NErrorStruct;}
        if(erCheck != NError::NErrorNone) {return false;}

        //Проверка примеров
        if(!tmp_examples->getIsOnline()
                && (tmp_examples->getTestset() < tmp_examples->getBeginset()
                || tmp_examples->getValidset() < tmp_examples->getTestset()
                || tmp_examples->getEndset() < tmp_examples->getValidset()
                || tmp_examples->getEndset() > tmp_examples->getLength()))
        {
            erCheck = NError::NErrorCrossExm;
        }
        if(erCheck != NError::NErrorNone) {return false;}

        for(int indExm = 0; indExm < tmp_examples->getLength() && erCheck == NError::NErrorNone; indExm++)
        {
           if(tmp_examples->get(indExm)->original->input.empty()) {erCheck = NError::NErrorExmEmpty;}
           if(tmp_examples->get(indExm)->original->output.empty()) {erCheck = NError::NErrorExmEmpty;}
           if(tmp_examples->get(indExm)->outrun.empty()) {erCheck = NError::NErrorExmEmpty;}
        }
        if(erCheck != NError::NErrorNone) {return false;}

        //Количество слоёв
        if(this->typeLay.getLength() != lenlay
                || this->lenNeurons.getLength() != lenlay+1
                || this->koefLay.getLength() != lenlay
                || this->dropout.getLength() != lenlay)
        {
            erCheck = NError::NErrorLay;
        }
        if(erCheck != NError::NErrorNone) {return false;}

        //Целевая функция
        if(this->typeLay.endElement() == NLayerType::NFuncSoftMax && this->typeTarget == NTargetFunc::NTargetMSE)
        {
            erCheck = NError::NErrorTarget;
        }
        if(erCheck != NError::NErrorNone) {return false;}
    }

    return (erCheck == NError::NErrorNone);
}

template <typename NType>
int INeuroNet<NType>::getIndexOutNumClass()
{
    return this->examples->getIndexOutNumClass(this->shiftout);
}

template <typename NType>
NType INeuroNet<NType>::maxOut()
{
    return this->tmp_examples->outrun->maxElements();
}

template <typename NType>
int INeuroNet<NType>::maxArg()
{
    return this->tmp_examples->outrun->maxArg();
}

template <typename NType>
int INeuroNet<NType>::maxArg(NArray<NType>& vecout)
{
    return vecout.maxArg(this->shiftout, this->getLenOut()) - this->shiftout;
}

template <typename NType>
NType INeuroNet<NType>::maxOut(NArray<NType>& vecout)
{
    return vecout.maxElements(this->shiftout, this->getLenOut());
}

template <typename NType>
int INeuroNet<NType>::actNet(NArray<NType>& outrun)
{
    return this->tmp_examples->actNet(outrun, this->shiftout);
}

template <typename NType>
void INeuroNet<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    string field = parent + ".lay";;

    this->updateTypeLay();
    this->updateLenNeurons();
    this->updateKoefLay();
    this->updateDropout();
    if(!this->isShotData()) {this->makeShot();}

    dt.addGroup(parent, "");
    //to_array_string(str_vec, enm_arr); dt.addElement(parent, "typeLay", str_vec, typeid(int).name());
    to_array_string(str_vec, this->typeLay); dt.addElement(parent, "typeLay", str_vec, typeid(int).name());
    to_array_string(str_vec, this->lenNeurons); dt.addElement(parent, "lenNeurons", str_vec, typeid(int).name());
    to_array_string(str_vec, this->koefLay); dt.addElement(parent, "koefLay", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->dropout); dt.addElement(parent, "dropout", str_vec, typeid(NType).name());
    str_val = to_string(this->valueWeight); dt.addElement(parent, "valueWeight", str_val, typeid(NType).name());
    str_val = to_string(this->energyRegularization); dt.addElement(parent, "energyRegularization", str_val, typeid(NType).name());
    str_val = to_string(this->shiftout); dt.addElement(parent, "shiftout", str_val, typeid(int).name());
    //str_val = to_string((unsigned int)this->typeInit); dt.addElement(parent, "typeInit", str_val, typeid(int).name());
    str_val = to_string(this->typeInit); dt.addElement(parent, "typeInit", str_val, typeid(int).name());
    str_val = to_string(this->status); dt.addElement(parent, "status", str_val, typeid(int).name());
    to_array_string(str_vec, this->lock); dt.addElement(parent, "lock", str_vec, typeid(bool).name());
    str_val = to_vstring(this->blLock); dt.addElement(parent, "blLock", str_val, typeid(bool).name());
    str_val = to_string(this->typeTarget); dt.addElement(parent, "typeTarget", str_val, typeid(int).name());
    to_array_string(str_vec, this->shot); dt.addElement(parent, "shotNet", str_vec, typeid(NType).name());

    for(int i = 0; i < this->lay.getLength(); i++)
    {
        this->lay[i]->saveECSV(dt, field);
    }
}

template <typename NType>
void INeuroNet<NType>::loadECSV(DataECSV& dt, string& parent)
{
    INLayer<NType>* pLay;
    StructECSV* iter;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    vector<string> subpath;
    string fieldLay = parent + ".lay";
    bool isShot = false;
    NArray<NType> state;

    //Выполнение операций перед загрузкой
    INeuroNet<NType>::deinit();
    this->extrn = false;
    this->energyRegularization = 0;
    this->createData();

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        pLay = INeuroNet::newLayer(NLayerType::NFuncLinear);
        pLay->setExtern(this->extrn);
        to_matrix_value(pLay->weigth, iter->mtrx);
        this->lay.push(pLay);
    }
    else
    {
        //Поля класса
        size_t ind = dt.getShift();
        iter = dt.modules[ind];
        if(iter->cmpPath(parent, true)) {ind++;}

        while(dt.isNextBlock(ind, parent))
        {
            iter = dt.modules[ind];

            //if(iter->getFieldValue(parent, "typeLay", str_vec)) {to_array_value(enm_arr, str_vec); this->typeLay.convertUInt(enm_arr); ind++;}
                 if(iter->getFieldValue(parent, "typeLay", str_vec)) {to_array_value(this->typeLay, str_vec); ind++;}
            else if(iter->getFieldValue(parent, "lenNeurons", str_vec)) {to_array_value(this->lenNeurons, str_vec); ind++;}
            else if(iter->getFieldValue(parent, "koefLay", str_vec)) {to_array_value(this->koefLay, str_vec); ind++;}
            else if(iter->getFieldValue(parent, "dropout", str_vec)) {to_array_value(this->dropout, str_vec); ind++;}
            else if(iter->getFieldValue(parent, "valueWeight", str_val)) {to_value(this->valueWeight, str_val); ind++;}
            else if(iter->getFieldValue(parent, "energyRegularization", str_val)) {to_value(this->energyRegularization, str_val); ind++;}
            else if(iter->getFieldValue(parent, "shiftout", str_val)) {to_value(this->shiftout, str_val); ind++;}
            //else if(iter->getFieldValue(parent, "typeInit", str_val)) {to_value(enm, str_val); this->typeInit = (NInitType)enm; ind++;}
            else if(iter->getFieldValue(parent, "typeInit", str_val)) {to_value(this->typeInit, str_val); ind++;}
            else if(iter->getFieldValue(parent, "lock", str_vec)) {to_array_value(this->lock, str_vec); ind++;}
            else if(iter->getFieldValue(parent, "blLock", str_val)) {to_value(this->blLock, str_val); ind++;}
            else if(iter->getFieldValue(parent, "typeTarget", str_val)) {to_value(this->typeTarget, str_val); ind++;}
            else if(iter->getFieldValue(parent, "shot", str_vec)) //Восстанавливаем сеть из снимка
            {
                to_array_value(state, str_vec);
                isShot = true; ind++;
            }
            else if(iter->cmpPath(fieldLay, true)) //Субполя класса
            {
                dt.setShift(ind);
                pLay = new INLayer<NType>();
                pLay->setExtern(this->extrn);
                pLay->loadECSV(dt, fieldLay);
                this->lay.push(pLay);
                ind = dt.getShift();
            }
            else
            {
                ind++;
            }
        }

        dt.setShift(ind);
    }

    //Выполнение операций после загрузки
    if(isShot)
    {
        this->deleteData();
        if(this->lay.empty())
        {
            this->init_struct();
            this->shot.memCopy(state.getData());
        }
        else
        {
            this->shot = state;
            this->update_struct();
        }
        this->updateData();
    }
    else
    {
        this->makeShot();
        this->deleteData();
        this->updateData();
    }

    this->updateTypeLay();
    this->updateLenNeurons();
    this->updateFuncNeurons();
    this->updateKoefLay();
    this->updateDropout();

    this->derivEnrg.init(this->lenNeurons.endElement(), 0);
    this->derivLay.init(this->lenNeurons.endElement(), 0);
    if(this->lock.empty()) {this->lock.init(this->getLenNeuronsCalc(), false);}
}

#endif // INEURONET_H
