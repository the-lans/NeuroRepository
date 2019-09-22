#ifndef INLAYERSTRUCT_H
#define INLAYERSTRUCT_H

#include "narray.h"
#include "nmatrix.h"
#include "iobjectecsv.h"
#include "neuroproject.h"

template <typename NType>
class INLayerStruct: public IObjectECSV
{
public:
    INLayerStruct();
    INLayerStruct(INLayerStruct<NType>& obj);
    INLayerStruct<NType>& operator=(INLayerStruct<NType>& obj);
    virtual ~INLayerStruct();

public:
    NMatrix<NType> weigth; //Матрица весов
    NArray<NType> bias; //Вектор смещений
    NArray<NLayerType> typeNeurons; //Типы нейронов
    NArray<NType> koefNeurons; //Коэффициенты функции активации нейрона
    bool* lock; //Маска для блокировки нейронов от изменения
    NType* shotlay; //Начало снимка слоя

protected:
    NType koef; //Коэффициент функции активации
    NLayerType type; //Тип слоя
    NLayerDerivat typeDerivat; //Тип аргумента производной
    NType dropout; //Параметр выключения нейронов dropout

public:
    void setKoef(NType koef);
    NType getKoef();
    void setType(NLayerType param);
    NLayerType getType();
    void setTypeDerivat(NLayerDerivat param);
    NLayerDerivat getTypeDerivat();
    int getLenNeurons();
    int getSizeData();
    void setExtern(bool param);
    bool getExtern();
    void setLock(bool param);
    bool getLock();
    void setDropout(NType param);
    NType getDropout();

public:
    void deinit(); //Деинициализация
    void init(NType value); //Инициализация значением
    void init(int lenRow, int lenColumn, NType value); //Инициализация значением
    void update_struct(int lenRow, int lenColumn); //Обновление структуры слоя
    void ginit(NType value); //Инициализация Нгуен-Видроу
    void ginit(int lenRow, int lenColumn, NType value); //Инициализация Нгуен-Видроу
    bool empty(); //У сети не заданы веса?
    void updateFuncNeurons(); //Обновление функций активации
    void updateData(); //Обновить указатели на данные
    int& updateData(NType* sl, int& pos);
    void deleteData(); //Удаление альтернативной структуры данных
    void createData(); //Создание альтернативной структуры данных

public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
INLayerStruct<NType>::INLayerStruct()
{
    this->type = NLayerType::NFuncNone;
    this->koef = 1;
    this->typeDerivat = NLayerDerivat::NDerivatOut;
    this->lock = nullptr;
    this->shotlay = nullptr;
    this->setExtern(true);
    this->dropout = 0;
}

template <typename NType>
INLayerStruct<NType>::INLayerStruct(INLayerStruct<NType>& obj):
    typeNeurons(obj.typeNeurons), koefNeurons(obj.koefNeurons)
{
    this->weigth.copyFields(obj.weigth);
    this->bias.copyFields(obj.bias);
    this->type = obj.getType();
    this->koef = obj.getKoef();
    this->typeDerivat = obj.getTypeDerivat();
    this->lock = nullptr;
    this->shotlay = nullptr;
    this->dropout = obj.getDropout();
}

template <typename NType>
INLayerStruct<NType>& INLayerStruct<NType>::operator=(INLayerStruct<NType>& obj)
{
    if(this->getExtern())
    {
        this->weigth.copyFields(obj.weigth);
        this->bias.copyFields(obj.bias);
    }
    else
    {
        this->weigth = obj.weigth;
        this->bias = obj.bias;
    }

    this->typeNeurons = obj.typeNeurons;
    this->koefNeurons = obj.koefNeurons;

    this->type = obj.getType();
    this->koef = obj.getKoef();
    this->typeDerivat = obj.getTypeDerivat();
    this->lock = nullptr;
    this->shotlay = nullptr;
    this->dropout = obj.getDropout();

    return *this;
}

template <typename NType>
INLayerStruct<NType>::~INLayerStruct()
{
    this->deinit();
}

template <typename NType>
void INLayerStruct<NType>::setKoef(NType koef)
{
    this->koef = koef;
}

template <typename NType>
NType INLayerStruct<NType>::getKoef()
{
    return this->koef;
}

template <typename NType>
void INLayerStruct<NType>::setType(NLayerType param)
{
    this->type = param;
}

template <typename NType>
NLayerType INLayerStruct<NType>::getType()
{
    return this->type;
}

template <typename NType>
void INLayerStruct<NType>::setTypeDerivat(NLayerDerivat param)
{
    this->typeDerivat = param;
}

template <typename NType>
NLayerDerivat INLayerStruct<NType>::getTypeDerivat()
{
    return this->typeDerivat;
}

template <typename NType>
int INLayerStruct<NType>::getLenNeurons()
{
    return this->weigth.getLenColumn();
}

template <typename NType>
int INLayerStruct<NType>::getSizeData()
{
    return this->weigth.getSize() + this->bias.getSize();
}

template <typename NType>
void INLayerStruct<NType>::setExtern(bool param)
{
    this->weigth.setExtrn(param);
    this->bias.setExtrn(param);
}

template <typename NType>
bool INLayerStruct<NType>::getExtern()
{
    return this->weigth.getExtrn();
}

template <typename NType>
void INLayerStruct<NType>::setLock(bool param)
{
    this->weigth.setLock(param);
    this->bias.setLock(param);
}

template <typename NType>
bool INLayerStruct<NType>::getLock()
{
    return this->weigth.getLock();
}

template <typename NType>
void INLayerStruct<NType>::setDropout(NType param)
{
    this->dropout = param;
}

template <typename NType>
NType INLayerStruct<NType>::getDropout()
{
    return this->dropout;
}


template <typename NType>
void INLayerStruct<NType>::deinit()
{
    this->weigth.clear();
    this->bias.clear();
    //this->typeNeurons.clear();
    //this->koefNeurons.clear();

    //Если нет альтернативной структуры данных
    if(this->getExtern())
    {
        this->weigth.setData(nullptr);
        this->bias.setData(nullptr);
    }
}

template <typename NType>
void INLayerStruct<NType>::init(NType value)
{
    int lenRow = this->weigth.getLenRow();
    int lenColumn = this->weigth.getLenColumn();
    this->init(lenRow, lenColumn, value);
}

template <typename NType>
void INLayerStruct<NType>::init(int lenRow, int lenColumn, NType value)
{
    int i, j, pos = 0;
    NType ran;
    bool isNullRand = isEqual(value);
    uniform_real_distribution<> distribution(-value, value);

    this->koefNeurons.clear();
    this->update_struct(lenRow, lenColumn);

    //Заполнение
    for(j = 0; j < lenColumn; j++)
    {
        if(this->lock == nullptr || !this->lock[pos])
        {
            for(i = 0; i < lenRow; i++)
            {
                ran = (isNullRand ? 0 : distribution(NRandGenerator));
                this->weigth.set(ran, i, j);
            }
            ran = (isNullRand ? 0 : distribution(NRandGenerator));
            this->bias.set(ran, j);
        }
        pos++;
    }
}

template <typename NType>
void INLayerStruct<NType>::update_struct(int lenRow, int lenColumn)
{
    //Инициализация
    if(this->getExtern())
    {
        this->weigth.setLength(lenRow, lenColumn);
        this->weigth.setSize(lenRow, lenColumn);
        this->bias.setLength(lenColumn);
        this->bias.setSize(lenColumn);
        this->updateData();
    }
    else
    {
        this->weigth.init(lenRow, lenColumn, 0);
        this->bias.init(lenColumn, 0);
        this->setLock(false);
    }
    this->updateFuncNeurons();
}

template <typename NType>
void INLayerStruct<NType>::ginit(NType value)
{
    int lenRow = this->weigth.getLenRow();
    int lenColumn = this->weigth.getLenColumn();
    this->ginit(lenRow, lenColumn, value);
}

template <typename NType>
void INLayerStruct<NType>::ginit(int lenRow, int lenColumn, NType value)
{
    int i, j, pos = 0;
    this->init(lenRow, lenColumn, 0);
    NType beta = 0.7 * pow(lenColumn, 1/lenRow);

    uniform_real_distribution<> distribution(-value, value);
    uniform_real_distribution<> distributionBeta(-beta, beta);

    NType ran, S;
    for(j = 0; j < lenColumn; j++)
    {
        if(this->lock == nullptr || !this->lock[pos])
        {
            S = 0;
            for(i = 0; i < lenRow; i++)
            {
                //ran = 2 * value * ((NType)rand()/(NType)RAND_MAX) - value;
                ran = distribution(NRandGenerator);
                this->weigth.set(ran, i, j);
                S += NFUNC_SQR(ran);
            }

            S = NFUNC_SQRT(S);
            for(i = 0; i < lenRow; i++)
            {
                ran = this->weigth[i][j];
                ran = beta * ran / S;
                this->weigth.set(ran, i, j);
            }
            //ran = 2 * beta * ((NType)rand()/(NType)RAND_MAX) - beta;
            ran = distributionBeta(NRandGenerator);
            this->bias.set(ran, j);
        }
        pos++;
    }
}

template <typename NType>
bool INLayerStruct<NType>::empty()
{
    return this->weigth.empty();
}

template <typename NType>
void INLayerStruct<NType>::updateFuncNeurons()
{
    int numNeurons = this->getLenNeurons();
    if(this->type != NLayerType::NFuncNone)
    {
        this->typeNeurons.clear();
        this->typeNeurons.init(numNeurons, this->type);
    }

    if(this->koefNeurons.empty())
    {
        this->koefNeurons.init(numNeurons, this->koef);
    }
}

template <typename NType>
void INLayerStruct<NType>::updateData()
{
    if(this->shotlay != nullptr && this->getExtern())
    {
        this->weigth.setData(this->shotlay);
        this->bias.setData(this->shotlay + this->weigth.getSize());
        this->setLock(true);
        this->setExtern(true);
    }
}

template <typename NType>
int& INLayerStruct<NType>::updateData(NType* sl, int& pos)
{
    if(this->getExtern())
    {
        this->shotlay = sl + pos;
        this->weigth.setData(this->shotlay);
        this->bias.setData(this->shotlay + this->weigth.getSize());
        this->setLock(true);
        this->setExtern(true);
        pos += this->getSizeData();
    }
    else
    {
        this->shotlay = nullptr;
    }

    return pos;
}

template <typename NType>
void INLayerStruct<NType>::deleteData()
{
    this->weigth.destruct();
    this->bias.destruct();
}

template <typename NType>
void INLayerStruct<NType>::createData()
{
    this->shotlay = nullptr;
    this->setLock(false);
    this->setExtern(false);
    this->weigth.renew();
    this->bias.renew();
}

template <typename NType>
void INLayerStruct<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;

    dt.addGroup(parent, "");
    to_matrix_string(str_mtrx, this->weigth); dt.addElement(parent, "weigth", str_mtrx, typeid(NType).name());

    to_array_string(str_vec, this->bias); dt.addElement(parent, "bias", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->typeNeurons); dt.addElement(parent, "typeNeurons", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->koefNeurons); dt.addElement(parent, "koefNeurons", str_vec, typeid(NType).name());

    str_val = to_string(this->koef); dt.addElement(parent, "koef", str_val, typeid(NType).name());
    str_val = to_string(this->type); dt.addElement(parent, "type", str_val, typeid(int).name());
    str_val = to_string(this->typeDerivat); dt.addElement(parent, "typeDerivat", str_val, typeid(int).name());
    str_val = to_string(this->dropout); dt.addElement(parent, "dropout", str_val, typeid(NType).name());
}

template <typename NType>
void INLayerStruct<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;

    //Выполнение операций перед загрузкой
    INLayerStruct<NType>::deinit();
    bool lk = this->getLock();
    this->setLock(false);

    if(dt.isOneMatrix())
    {
        iter = dt.modules[0];
        to_matrix_value(this->weigth, iter->mtrx);
    }
    else
    {
        size_t ind = dt.getShift();
        iter = dt.modules[ind];
        if(iter->cmpPath(parent, true)) {ind++;}

        while(dt.isNextBlock(ind, parent))
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "weigth", str_mtrx))
            {
                this->shotlay = nullptr;
                this->weigth.renew(str_mtrx.getLenRow(), str_mtrx.getLenColumn());
                to_matrix_value(this->weigth, str_mtrx);
            }
            else if(iter->getFieldValue(parent, "bias", str_vec))
            {
                this->shotlay = nullptr;
                this->bias.renew(str_vec.size());
                to_array_value(this->bias, str_vec);
            }

            else if(iter->getFieldValue(parent, "typeNeurons", str_vec)) {to_array_value(this->typeNeurons, str_vec);}
            else if(iter->getFieldValue(parent, "koefNeurons", str_vec)) {to_array_value(this->koefNeurons, str_vec);}

            else if(iter->getFieldValue(parent, "koef", str_val)) {to_value(this->koef, str_val);}
            else if(iter->getFieldValue(parent, "type", str_val)) {to_value(this->type, str_val);}
            else if(iter->getFieldValue(parent, "typeDerivat", str_val)) {to_value(this->typeDerivat, str_val);}
            else if(iter->getFieldValue(parent, "dropout", str_val)) {to_value(this->dropout, str_val);}

            ind++;
        }

        dt.setShift(ind);

        //Выполнение операций после загрузки
        this->setLock(lk);
        this->updateFuncNeurons();
    }
}

#endif // INLAYERSTRUCT_H
