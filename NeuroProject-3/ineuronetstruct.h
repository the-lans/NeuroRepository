#ifndef INEURONETSTRUCT_H
#define INEURONETSTRUCT_H

#include "inlayerstruct.h"
#include "iobjectecsv.h"
//#include <iostream>
//#include <ctime>

template <typename NType>
class INeuroNetStruct: public IObjectECSV
{
public:
    INeuroNetStruct();
    INeuroNetStruct(int size);
    INeuroNetStruct(INeuroNetStruct<NType>& obj);
    INeuroNetStruct<NType>& operator=(const INeuroNetStruct<NType>& obj);
    virtual ~INeuroNetStruct();

public:
    NArray<INLayerStruct<NType>*> lay; //Структура нейросети: слои
    NArray<NLayerType> typeLay; //Структура нейросети: Типы слоёв
    NArray<int> lenNeurons; //Структура нейросети: Количество нейронов
    NArray<NType> koefLay; //Структура нейросети: Коэффициенты слоя
    NArray<NType> shot; //Снимок нейросети
    NArray<bool> lock; //Маска для блокировки нейронов от изменения
    bool blLock; //Выключатель блокировки нейронов для нейросети в целом
    NArray<NType> dropout; //Параметр выключения нейронов dropout

protected:
    NType valueWeight; //Структура нейросети: параметр инициализации весов
    int shiftout; //Структура нейросети: cмещение выходного вектора примеров для нейросети
    NInitType typeInit; //Структура нейросети: тип инициализации
    bool extrn; //Внешние данные
    NTargetFunc typeTarget; //Вид целевой функции
    NError erCheck; //Отслеживание ошибок
    NStatusNet status; //Статус нейросети

public:
    void setValueWeight(NType value);
    NType getValueWeight();
    int getLenIn();
    int getLenOut();
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
    void copyNet(INeuroNetStruct<NType>* obj); //Копирование структуры нейросети
    static INLayerStruct<NType>* newLayer(NLayerType layType); //Добавление нового слоя
    virtual void deinit(); //Деинициализация
    void init(); //Инициализация
    void init(NArray<int>& num, NArray<NLayerType>& layType); //Инициализация
    void init_struct(); //Инициализация структуры
    void update_struct(); //Обновление структуры нейросети
    void emtyLayer(); //Очистить слои
    void ginit(); //Инициализация Нгуен-Видроу
    void ginit(NArray<int>& num, NArray<NLayerType>& layType); //Инициализация Нгуен-Видроу
    void ginit_struct(); //Инициализация структуры
    void updateTypeLay(); //Обновить типы слоёв
    void updateLenNeurons(); //Обновить структуру нейросети
    void updateKoefLay(); //Обновить коэффициенты слоёв
    void updateDropout(); //Обновить dropout
    //void updateFuncNeurons(); //Обновление функций
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

public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
INeuroNetStruct<NType>::INeuroNetStruct()
{
    this->valueWeight = 0;
    this->shiftout = 0;
    this->typeInit = NInitType::NInitNone;
    this->blLock = false;
    this->extrn = true;
    this->typeTarget = NTargetFunc::NTargetNone;
    this->erCheck = NError::NErrorNone;
    this->status = NStatusNet::NStatusCreate;
}

template <typename NType>
INeuroNetStruct<NType>::INeuroNetStruct(int size):
    lay(size)
{
    this->valueWeight = 0;
    this->shiftout = 0;
    this->typeInit = NInitType::NInitNone;
    this->blLock = false;
    this->extrn = true;
    this->typeTarget = NTargetFunc::NTargetNone;
    this->erCheck = NError::NErrorNone;
    this->status = NStatusNet::NStatusCreate;
}

template <typename NType>
INeuroNetStruct<NType>::INeuroNetStruct(INeuroNetStruct<NType>& obj):
    lay(obj.lay.getLength())
{
    this->blLock = obj.blLock;
    this->lock = obj.lock;
    this->shot = obj.shot;
    this->extrn = obj.getExtern();

    //Копирование слоёв
    INLayerStruct<NType>** pLay = obj.lay.getData();
    INLayerStruct<NType>* layer;
    for(int i = 0; i < obj.lay.getLength(); i++)
    {
        layer = INLayerStruct<NType>::newLayer(pLay[i]->getType());
        //layer->setExtern(pLay[i]->getExtern());
        layer->setExtern(this->extrn);
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    //Копирование других полей
    this->valueWeight = obj.getValueWeight();
    this->shiftout = obj.getShiftout();
    this->typeInit = obj.getTypeInit();
    this->status = obj.getStatus();

    this->updateData();

    //Копирование примеров
    this->typeLay = obj.typeLay;
    this->lenNeurons = obj.lenNeurons;
    this->koefLay = obj.koefLay;
    this->dropout = obj.dropout;
}

template <typename NType>
INeuroNetStruct<NType>& INeuroNetStruct<NType>::operator=(const INeuroNetStruct<NType>& obj)
{
    this->copyNet(&obj);
    return *this;
}

template <typename NType>
INeuroNetStruct<NType>::~INeuroNetStruct()
{
    this->deinit();
}


template <typename NType>
void INeuroNetStruct<NType>::setValueWeight(NType value)
{
    this->valueWeight = value;
}

template <typename NType>
NType INeuroNetStruct<NType>::getValueWeight()
{
    return this->valueWeight;
}

template <typename NType>
int INeuroNetStruct<NType>::getLenIn()
{
    return this->lay[0]->weigth.getLenRow();
}

template <typename NType>
int INeuroNetStruct<NType>::getLenOut()
{
    return this->lay.endElement()->weigth.getLenColumn();
}

template <typename NType>
void INeuroNetStruct<NType>::setShiftout(int shift)
{
    this->shiftout = shift;
}

template <typename NType>
int INeuroNetStruct<NType>::getShiftout()
{
    return this->shiftout;
}

template <typename NType>
void INeuroNetStruct<NType>::setTypeInit(NInitType param)
{
    this->typeInit = param;
}

template <typename NType>
NInitType INeuroNetStruct<NType>::getTypeInit()
{
    return this->typeInit;
}

template <typename NType>
void INeuroNetStruct<NType>::setStatus(NStatusNet param)
{
    this->status = param;
}

template <typename NType>
NStatusNet INeuroNetStruct<NType>::getStatus()
{
    return this->status;
}

template <typename NType>
void INeuroNetStruct<NType>::setExtern(bool param)
{
    this->extrn = param;
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        this->lay[i]->setExtern(param);
    }
}

template <typename NType>
bool INeuroNetStruct<NType>::getExtern()
{
    return this->extrn;
}

template <typename NType>
void INeuroNetStruct<NType>::setTypeTarget(NTargetFunc param)
{
    this->typeTarget = param;
}

template <typename NType>
NTargetFunc INeuroNetStruct<NType>::getTypeTarget()
{
    return this->typeTarget;
}

template <typename NType>
NError INeuroNetStruct<NType>::getCheck()
{
    return this->erCheck;
}


template <typename NType>
void INeuroNetStruct<NType>::copyNet(INeuroNetStruct<NType>* obj)
{
    //Копирование слоёв
    this->emtyLayer();
    this->lay.resize(obj->lay.getLength());

    this->blLock = obj->blLock;
    this->lock = obj->lock;
    this->shot = obj->shot;
    //this->extrn = obj->extrn;

    INLayerStruct<NType>** pLay = obj->lay.getData();
    INLayerStruct<NType>* layer;
    for(int i = 0; i < obj->lay.getLength(); i++)
    {
        layer = INLayerStruct<NType>::newLayer(pLay[i]->getType());
        //layer->setExtern(pLay[i]->getExtern());
        layer->setExtern(this->extrn);
        *layer = *(pLay[i]);
        this->lay.push(layer);
    }

    //Копирование других полей
    this->valueWeight = obj->getValueWeight();
    this->shiftout = obj->getShiftout();
    this->typeInit = obj->getTypeInit();
    this->status = obj->getStatus();
    this->typeLay = obj->typeLay;
    this->lenNeurons = obj->lenNeurons;
    this->koefLay = obj->koefLay;
    this->dropout = obj->dropout;

    this->updateData();
}

template <typename NType>
INLayerStruct<NType>* INeuroNetStruct<NType>::newLayer(NLayerType layType)
{
    INLayerStruct<NType>* layer = new INLayerStruct<NType>();
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

    //layer->updateFuncNeurons();

    return layer;
}

template <typename NType>
void INeuroNetStruct<NType>::deinit()
{
    this->emtyLayer();
    this->typeLay.clear();
    this->lenNeurons.clear();
    this->koefLay.clear();
    this->lock.clear();
    this->dropout.clear();
}

template <typename NType>
void INeuroNetStruct<NType>::init()
{
    this->updateData();

    int pos = 0;
    INLayerStruct<NType>** pLay = this->lay.getData();
    bool* pLock = this->lock.getData();
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        if(this->blLock) {pLay[i]->lock = pLock + pos;} else {pLay[i]->lock = nullptr;}
        if(i >= this->koefLay.getLength()) {this->koefLay.push(pLay[i]->getKoef());}
        if(i >= this->dropout.getLength()) {this->dropout.push(pLay[i]->getDropout());}
        pLay[i]->init(this->valueWeight);
        pos += pLay[i]->getLenNeurons();
    }

    //this->lock.init(this->getLenNeurons(), false);
    //this->updateFuncNeurons();
    this->status = NStatusNet::NStatusInit;
}

template <typename NType>
void INeuroNetStruct<NType>::init(NArray<int>& num, NArray<NLayerType>& layType)
{
    this->typeLay = layType;
    this->lenNeurons = num;
    this->init_struct();
}

template <typename NType>
void INeuroNetStruct<NType>::init_struct()
{
    int pos = 0;
    INLayerStruct<NType>* layer;
    this->emtyLayer();
    this->shot.init(this->getLenWeightCalc(), 0);

    //Создание слоёв
    for(int i = 0; i < this->lenNeurons.getLength()-1; i++)
    {
        layer = INeuroNetStruct<NType>::newLayer(this->typeLay[i]);
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

    //this->updateFuncNeurons();
    this->status = NStatusNet::NStatusInit;
}

template <typename NType>
void INeuroNetStruct<NType>::update_struct()
{
    int pos = 0;
    INLayerStruct<NType>* layer;

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

    //this->updateFuncNeurons();
}

template <typename NType>
void INeuroNetStruct<NType>::emtyLayer()
{
    INLayerStruct<NType>* layer;
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
void INeuroNetStruct<NType>::ginit()
{
    this->updateData();

    //srand(time(0));
    int pos = 0;
    INLayerStruct<NType>** pLay = this->lay.getData();
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        if(this->blLock) {pLay[i]->lock = this->lock.getData() + pos;}
        else {pLay[i]->lock = nullptr;}
        if(i >= this->koefLay.getLength()) {this->koefLay.push(pLay[i]->getKoef());}
        if(i >= this->dropout.getLength()) {this->dropout.push(pLay[i]->getDropout());}
        pLay[i]->ginit(this->valueWeight);
        pos += pLay[i]->getLenNeurons();
    }
    //this->updateFuncNeurons();
    this->status = NStatusNet::NStatusInit;
}

template <typename NType>
void INeuroNetStruct<NType>::ginit(NArray<int>& num, NArray<NLayerType>& layType)
{
    this->typeLay = layType;
    this->lenNeurons = num;
    this->ginit_struct();
}

template <typename NType>
void INeuroNetStruct<NType>::ginit_struct()
{
    int i, pos = 0;
    this->emtyLayer();

    //srand(time(0));
    int lenLayer = this->lenNeurons.getLength()-1;
    this->lay.clear();
    this->shot.init(this->getLenWeightCalc());
    INLayerStruct<NType>* layer;

    //Создание слоёв
    for(i = 0; i < lenLayer; i++)
    {
        layer = INLayerStruct<NType>::newLayer(this->typeLay[i]);
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
    //this->updateFuncNeurons();
    this->status = NStatusNet::NStatusInit;
}

template <typename NType>
void INeuroNetStruct<NType>::updateTypeLay()
{
    if(this->lay.getLength() > 0)
    {
        this->typeLay.clear();
        INLayerStruct<NType>* pLay;
        for(int i = 0; i < this->lay.getLength(); i++)
        {
            pLay = this->lay[i];
            this->typeLay.push(pLay->getType());
        }
    }
}

template <typename NType>
void INeuroNetStruct<NType>::updateLenNeurons()
{
    if(this->lay.getLength() > 0)
    {
        this->lenNeurons.clear();
        INLayerStruct<NType>* pLay;
        this->lenNeurons.push(this->getLenIn());
        for(int i = 0; i < this->lay.getLength(); i++)
        {
            pLay = this->lay[i];
            this->lenNeurons.push(pLay->getLenNeurons());
        }
    }
}

template <typename NType>
void INeuroNetStruct<NType>::updateKoefLay()
{
    if(this->lay.getLength() > 0)
    {
        this->koefLay.clear();
        INLayerStruct<NType>* pLay;
        for(int i = 0; i < this->lay.getLength(); i++)
        {
            pLay = this->lay[i];
            this->koefLay.push(pLay->getKoef());
        }
    }
}

template <typename NType>
void INeuroNetStruct<NType>::updateDropout()
{
    if(this->lay.getLength() > 0)
    {
        this->dropout.clear();
        INLayerStruct<NType>* pLay;
        for(int i = 0; i < this->lay.getLength(); i++)
        {
            pLay = this->lay[i];
            this->dropout.push(pLay->getKoef());
        }
    }
}

template <typename NType>
int INeuroNetStruct<NType>::getLenNeurons()
{
    INLayerStruct<NType>* pLay;
    int num = 0;
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay = this->lay[i];
        num += pLay->getLenNeurons();
    }
    return num;
}

template <typename NType>
int INeuroNetStruct<NType>::getLenNeuronsCalc()
{
    return this->lenNeurons.sumElements(1);
}

template <typename NType>
int INeuroNetStruct<NType>::getLenWeight()
{
    INLayerStruct<NType>* pLay;
    int num = 0;
    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay = this->lay[i];
        num += pLay->getSizeData();
    }
    return num;
}

template <typename NType>
int INeuroNetStruct<NType>::getLenWeightCalc()
{
    int num = 0;
    for(int i = 1; i < this->lenNeurons.getLength(); i++)
    {
        num += lenNeurons[i] * (lenNeurons[i-1] + 1);
    }
    return num;
}

template <typename NType>
void INeuroNetStruct<NType>::makeShot()
{
    int k, i, j, len;
    INLayerStruct<NType>* layer;
    NType* data;

    if(this->getExtern()) {throw std::out_of_range("INeuroNetStruct<NType>::makeShot() : is extern");}
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
void INeuroNetStruct<NType>::restorShot()
{
    int k, i, j, len, pos = 0;
    INLayerStruct<NType>* layer;
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
bool INeuroNetStruct<NType>::isShotData()
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
void INeuroNetStruct<NType>::updateData()
{
    int pos = 0;
    int len = this->shot.getLength();
    INLayerStruct<NType>** pLay = this->lay.getData();
    NType* pShot = this->shot.getData();

    for(int i = 0; i < this->lay.getLength(); i++)
    {
        if(pLay[i]->getExtern())
        {
            pLay[i]->shotlay = pShot + pos;
            pLay[i]->updateData();
            pos += pLay[i]->getSizeData();
            if(pos > len) {throw std::out_of_range("INeuroNetStruct<NType>::updateData() : index is out of range");}
        }
        else
        {
            pLay[i]->shotlay = nullptr;
        }
    }
}

template <typename NType>
void INeuroNetStruct<NType>::deleteData()
{
    INLayerStruct<NType>** pLay = this->lay.getData();

    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay[i]->deleteData();
        pLay[i]->setLock(true);
    }
    this->setExtern(true);
}

template <typename NType>
void INeuroNetStruct<NType>::createData()
{
    INLayerStruct<NType>** pLay = this->lay.getData();

    for(int i = 0; i < this->lay.getLength(); i++)
    {
        pLay[i]->createData();
        pLay[i]->setLock(false);
    }
    this->setExtern(false);
}

template <typename NType>
bool INeuroNetStruct<NType>::check()
{
    erCheck = NError::NErrorNone;
    INLayerStruct<NType>** pLay = this->lay.getData();
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
void INeuroNetStruct<NType>::saveECSV(DataECSV& dt, string& parent)
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
    str_val = to_string(this->shiftout); dt.addElement(parent, "shiftout", str_val, typeid(int).name());
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
void INeuroNetStruct<NType>::loadECSV(DataECSV& dt, string& parent)
{
    INLayerStruct<NType>* pLay;
    StructECSV* iter;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    vector<string> subpath;
    string fieldLay = parent + ".lay";
    bool isShot = false;
    NArray<NType> state;

    //Выполнение операций перед загрузкой
    INeuroNetStruct<NType>::deinit();
    this->extrn = false;
    this->createData();

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        pLay = INeuroNetStruct<NType>::newLayer(NLayerType::NFuncLinear);
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
            else if(iter->getFieldValue(parent, "shiftout", str_val)) {to_value(this->shiftout, str_val); ind++;}
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
                pLay = new INLayerStruct<NType>();
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
    //this->updateFuncNeurons();
    this->updateKoefLay();
    this->updateDropout();

    if(this->lock.empty()) {this->lock.init(this->getLenNeuronsCalc(), false);}
}

#endif // INEURONETSTRUCT_H
