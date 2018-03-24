#ifndef INLAYER_H
#define INLAYER_H

#include "narray.h"
#include "nmatrix.h"
#include "iobjectecsv.h"
#include "neuroproject.h"

template <typename NType>
class INLayer: public IObjectECSV
{
public:
    INLayer();
    INLayer(INLayer<NType>& obj);
    INLayer<NType>& operator=(INLayer<NType>& obj);
    virtual ~INLayer();

public:
    NMatrix<NType> weigth; //Матрица весов
    NArray<NType> bias; //Вектор смещений
    NArray<NType> slay; //Множитель dropout
    NArray<NType> output; //Расчётный выход слоя
    NArray<NType> sum; //Промежуточные значения сумм
    NArray<NLayerType> typeNeurons; //Типы нейронов
    NArray<NType> koefNeurons; //Коэффициенты функции активации нейрона
    bool* lock; //Маска для блокировки нейронов от изменения
    NType* shotlay; //Начало снимка слоя

public:
    NArray< NType (*)(NType&, NType&) > activNeurons; //Функции активации для нейронов
    NArray< NType (*)(NType&, NType&, NType&) > derivNeurons; //Производные функций активации для нейронов
    NType (*activation)(NType&, NType&); //Функция активации нейрона
    NType (*derivative)(NType&, NType&, NType&); //Производная функции активации

protected:
    NType koef; //Коэффициент функции активации
    NLayerType type; //Тип слоя
    NType energyRegularization; //Энергия регуляризации слоя
    NLayerDerivat typeDerivat; //Тип аргумента производной
    NType dropout; //Параметр выключения нейронов dropout
    NType koefDropout; //= 1 / (1 - dropout)

public:
    void setKoef(NType koef);
    NType getKoef();
    void setType(NLayerType param);
    NLayerType getType();
    NType getEnergyRegularization();
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
    NType getKoefDropout();

public:
    void deinit(); //Деинициализация
    void init(NType value); //Инициализация значением
    void init(int lenRow, int lenColumn, NType value); //Инициализация значением
    void update_struct(int lenRow, int lenColumn); //Обновление структуры слоя
    void ginit(NType value); //Инициализация Нгуен-Видроу
    void ginit(int lenRow, int lenColumn, NType value); //Инициализация Нгуен-Видроу
    //virtual NType activation(NType& x) = 0; //Функция активации нейрона
    //virtual NType derivative(NType& y) = 0; //Производная функции активации
    bool empty(); //У сети не заданы веса?
    void init_slay(); //Задаём множители
    void initKoefDropout(); //Инициализация коэффициента dropout
    virtual NArray<NType>* run(NArray<NType>* X); //Функционирование слоя
    virtual NArray<NType>* run_dropout(NArray<NType>* X);
    NArray<NType>* derivRun(NArray<NType>* mas); //Расчёт производных слоя
    virtual NType funcRegularization(); //Функция регуляризации слоя
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
INLayer<NType>::INLayer()
{
    this->type = NLayerType::NFuncNone;
    this->koef = 1;
    this->energyRegularization = 0;
    this->typeDerivat = NLayerDerivat::NDerivatOut;
    this->activation = nullptr;
    this->derivative = nullptr;
    this->lock = nullptr;
    this->shotlay = nullptr;
    this->setExtern(true);
    this->dropout = 0;
}

template <typename NType>
INLayer<NType>::INLayer(INLayer<NType>& obj):
    output(obj.output), sum(obj.sum),
    typeNeurons(obj.typeNeurons), activNeurons(obj.activNeurons), derivNeurons(obj.derivNeurons),
    koefNeurons(obj.koefNeurons), slay(obj.slay)
{
    this->weigth.copyFields(obj.weigth);
    this->bias.copyFields(obj.bias);

    this->type = obj.getType();
    this->koef = obj.getKoef();
    this->energyRegularization = obj.getEnergyRegularization();
    this->typeDerivat = obj.getTypeDerivat();
    this->activation = obj.activation;
    this->derivative = obj.derivative;
    this->lock = nullptr;
    this->shotlay = nullptr;
    this->dropout = obj.getDropout();
}

template <typename NType>
INLayer<NType>& INLayer<NType>::operator=(INLayer<NType>& obj)
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

    this->output = obj.output;
    this->sum = obj.sum;
    this->slay = obj.slay;
    this->typeNeurons = obj.typeNeurons;
    this->activNeurons = obj.activNeurons;
    this->derivNeurons = obj.derivNeurons;
    this->koefNeurons = obj.koefNeurons;

    this->type = obj.getType();
    this->koef = obj.getKoef();
    this->energyRegularization = obj.getEnergyRegularization();
    this->typeDerivat = obj.getTypeDerivat();
    this->activation = obj.activation;
    this->derivative = obj.derivative;
    this->lock = nullptr;
    this->shotlay = nullptr;
    this->dropout = obj.getDropout();

    return *this;
}

template <typename NType>
INLayer<NType>::~INLayer()
{
    this->deinit();
}

template <typename NType>
void INLayer<NType>::setKoef(NType koef)
{
    this->koef = koef;
}

template <typename NType>
NType INLayer<NType>::getKoef()
{
    return this->koef;
}

template <typename NType>
void INLayer<NType>::setType(NLayerType param)
{
    this->type = param;
}

template <typename NType>
NLayerType INLayer<NType>::getType()
{
    return this->type;
}

template <typename NType>
NType INLayer<NType>::getEnergyRegularization()
{
    return this->energyRegularization;
}

template <typename NType>
void INLayer<NType>::setTypeDerivat(NLayerDerivat param)
{
    this->typeDerivat = param;
}

template <typename NType>
NLayerDerivat INLayer<NType>::getTypeDerivat()
{
    return this->typeDerivat;
}

template <typename NType>
int INLayer<NType>::getLenNeurons()
{
    return this->weigth.getLenColumn();
}

template <typename NType>
int INLayer<NType>::getSizeData()
{
    return this->weigth.getSize() + this->bias.getSize();
}

template <typename NType>
void INLayer<NType>::setExtern(bool param)
{
    this->weigth.setExtrn(param);
    this->bias.setExtrn(param);
}

template <typename NType>
bool INLayer<NType>::getExtern()
{
    return this->weigth.getExtrn();
}

template <typename NType>
void INLayer<NType>::setLock(bool param)
{
    this->weigth.setLock(param);
    this->bias.setLock(param);
}

template <typename NType>
bool INLayer<NType>::getLock()
{
    return this->weigth.getLock();
}

template <typename NType>
void INLayer<NType>::setDropout(NType param)
{
    this->dropout = param;
}

template <typename NType>
NType INLayer<NType>::getDropout()
{
    return this->dropout;
}

template <typename NType>
NType INLayer<NType>::getKoefDropout()
{
    return this->koefDropout;
}


template <typename NType>
void INLayer<NType>::deinit()
{
    this->weigth.clear();
    this->bias.clear();
    this->output.clear();
    this->sum.clear();
    this->slay.clear();
    //this->typeNeurons.clear();
    //this->activNeurons.clear();
    //this->derivNeurons.clear();
    //this->koefNeurons.clear();
    this->energyRegularization = 0;

    //Если нет альтернативной структуры данных
    if(this->getExtern())
    {
        this->weigth.setData(nullptr);
        this->bias.setData(nullptr);
    }
}

template <typename NType>
void INLayer<NType>::init(NType value)
{
    int lenRow = this->weigth.getLenRow();
    int lenColumn = this->weigth.getLenColumn();
    this->init(lenRow, lenColumn, value);
}

template <typename NType>
void INLayer<NType>::init(int lenRow, int lenColumn, NType value)
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
void INLayer<NType>::update_struct(int lenRow, int lenColumn)
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

    this->output.init(lenColumn, 0);
    this->sum.init(lenColumn, 0);
    this->slay.init(lenColumn, 1);
    this->updateFuncNeurons();
}

template <typename NType>
void INLayer<NType>::ginit(NType value)
{
    int lenRow = this->weigth.getLenRow();
    int lenColumn = this->weigth.getLenColumn();
    this->ginit(lenRow, lenColumn, value);
}

template <typename NType>
void INLayer<NType>::ginit(int lenRow, int lenColumn, NType value)
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
bool INLayer<NType>::empty()
{
    return this->weigth.empty();
}

template <typename NType>
void INLayer<NType>::init_slay()
{
    NType value;
    NType koefDropout = 1/(1 - this->dropout);
    std::uniform_real_distribution<NType> distribution(0, 1);
    NType* pSlay = this->slay.getData();

    for(int ind = 0; ind < this->slay.getLength(); ind++)
    {
        value = distribution(NRandGenerator);
        if(value < this->dropout) {pSlay[ind] = 0;}
        else {pSlay[ind] = koefDropout;}
    }
}

template <typename NType>
void INLayer<NType>::initKoefDropout()
{
    this->koefDropout = 1 / (1 - this->dropout);
}

template <typename NType>
NArray<NType>* INLayer<NType>::run(NArray<NType>* X)
{
    NType* pOut = output.getData();
    NType* pSum = sum.getData();

    sum.mul(*X, weigth, true); //X.length = weigth.lenRow
    sum.sum(bias);

    for(int i = 0; i < output.getLength(); i++)
    {
        activation = activNeurons[i];
        pOut[i] = activation(pSum[i], koefNeurons[i]);
    }

    return &(output);
}

template <typename NType>
NArray<NType>* INLayer<NType>::run_dropout(NArray<NType>* X)
{
    run(X);
    output.mul(slay);

    return &(output);
}

template <typename NType>
NArray<NType>* INLayer<NType>::derivRun(NArray<NType>* mas)
{
    NType* pOutrun = output.getData();
    NType* pOutsum = sum.getData();

    for(int i = 0; i < weigth.getLenColumn(); i++)
    {
        this->derivative = derivNeurons[i];
        mas->set(this->derivative(pOutsum[i], pOutrun[i], koefNeurons[i]), i);
    }

    return mas;
}

template <typename NType>
NType INLayer<NType>::funcRegularization()
{
    int i, j;
    energyRegularization = 0;
    NType* pDataWeigth = this->weigth.getData();
    NType* pDataBias = this->bias.getData();
    int lenRow = this->weigth.getLenRow();
    int lenColumn = this->weigth.getLenColumn();
    int sz = this->weigth.getSizeColumn();

    for(j = 0; j < lenColumn; j++)
    {
        for(i = 0; i < lenRow; i++)
        {
            energyRegularization += NFUNC_SQR(pDataWeigth[i * sz + j]);
        }
        energyRegularization += NFUNC_SQR(pDataBias[j]);
    }

    energyRegularization *= 0.5;
    return energyRegularization;
}

template <typename NType>
void INLayer<NType>::updateFuncNeurons()
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

    NLayerType lt;
    this->activNeurons.clear();
    this->derivNeurons.clear();
    for(int i = 0; i < numNeurons; i++)
    {
        lt = this->typeNeurons[i];

             if(lt == NLayerType::NFuncTanh)       {this->activNeurons.push(funcTanh);        this->derivNeurons.push(derivTanh);}
        else if(lt == NLayerType::NFuncSoftsign)   {this->activNeurons.push(funcSoftsign);    this->derivNeurons.push(derivSoftsign);}
        else if(lt == NLayerType::NFuncArctg)      {this->activNeurons.push(funcArctg);       this->derivNeurons.push(derivArctg);}
        else if(lt == NLayerType::NFuncLinear)     {this->activNeurons.push(funcLinear);      this->derivNeurons.push(derivLinear);}
        else if(lt == NLayerType::NFuncStep)       {this->activNeurons.push(funcStep);        this->derivNeurons.push(derivStep);}
        else if(lt == NLayerType::NFuncSoftStep)   {this->activNeurons.push(funcSoftStep);    this->derivNeurons.push(derivSoftStep);}
        else if(lt == NLayerType::NFuncReLU)       {this->activNeurons.push(funcReLU);        this->derivNeurons.push(derivReLU);}
        else if(lt == NLayerType::NFuncPReLU)      {this->activNeurons.push(funcPReLU);       this->derivNeurons.push(derivPReLU);}
        else if(lt == NLayerType::NFuncELU)        {this->activNeurons.push(funcELU);         this->derivNeurons.push(derivELU);}
        else if(lt == NLayerType::NFuncSoftPlus)   {this->activNeurons.push(funcSoftPlus);    this->derivNeurons.push(derivSoftPlus);}
        else if(lt == NLayerType::NFuncSin)        {this->activNeurons.push(funcSin);         this->derivNeurons.push(derivSin);}
        else if(lt == NLayerType::NFuncSinc)       {this->activNeurons.push(funcSinc);        this->derivNeurons.push(derivSinc);}
        else if(lt == NLayerType::NFuncGaussian)   {this->activNeurons.push(funcGaussian);    this->derivNeurons.push(derivGaussian);}
        else if(lt == NLayerType::NFuncLinearInt)  {this->activNeurons.push(funcLinearInt);   this->derivNeurons.push(derivLinearInt);}
        else if(lt == NLayerType::NFuncSoftMax)    {this->activNeurons.push(funcSoftMax);     this->derivNeurons.push(derivSoftMax);}
    }

    this->activation = (numNeurons > 0 ? this->activNeurons[0] : nullptr);
    this->derivative = (numNeurons > 0 ? this->derivNeurons[0] : nullptr);
}

template <typename NType>
void INLayer<NType>::updateData()
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
int& INLayer<NType>::updateData(NType* sl, int& pos)
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
void INLayer<NType>::deleteData()
{
    this->weigth.destruct();
    this->bias.destruct();
}

template <typename NType>
void INLayer<NType>::createData()
{
    this->shotlay = nullptr;
    this->setLock(false);
    this->setExtern(false);
    this->weigth.renew();
    this->bias.renew();
}

template <typename NType>
void INLayer<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;

    dt.addGroup(parent, "");
    to_matrix_string(str_mtrx, this->weigth); dt.addElement(parent, "weigth", str_mtrx, typeid(NType).name());

    to_array_string(str_vec, this->bias); dt.addElement(parent, "bias", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->slay); dt.addElement(parent, "slay", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->output); dt.addElement(parent, "output", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->sum); dt.addElement(parent, "sum", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->typeNeurons); dt.addElement(parent, "typeNeurons", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->koefNeurons); dt.addElement(parent, "koefNeurons", str_vec, typeid(NType).name());

    str_val = to_string(this->koef); dt.addElement(parent, "koef", str_val, typeid(NType).name());
    str_val = to_string(this->energyRegularization); dt.addElement(parent, "energyRegularization", str_val, typeid(NType).name());
    str_val = to_string(this->type); dt.addElement(parent, "type", str_val, typeid(int).name());
    //str_val = to_string((unsigned int)this->typeDerivat); dt.addElement(parent, "typeDerivat", str_val, typeid(int).name());
    str_val = to_string(this->typeDerivat); dt.addElement(parent, "typeDerivat", str_val, typeid(int).name());
    str_val = to_string(this->dropout); dt.addElement(parent, "dropout", str_val, typeid(NType).name());
}

template <typename NType>
void INLayer<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;

    //Выполнение операций перед загрузкой
    INLayer<NType>::deinit();
    this->energyRegularization = 0;
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

            else if(iter->getFieldValue(parent, "output", str_vec)) {to_array_value(this->output, str_vec);}
            else if(iter->getFieldValue(parent, "sum", str_vec)) {to_array_value(this->sum, str_vec);}
            else if(iter->getFieldValue(parent, "typeNeurons", str_vec)) {to_array_value(this->typeNeurons, str_vec);}
            else if(iter->getFieldValue(parent, "koefNeurons", str_vec)) {to_array_value(this->koefNeurons, str_vec);}

            else if(iter->getFieldValue(parent, "koef", str_val)) {to_value(this->koef, str_val);}
            else if(iter->getFieldValue(parent, "energyRegularization", str_val)) {to_value(this->energyRegularization, str_val);}
            else if(iter->getFieldValue(parent, "type", str_val)) {to_value(this->type, str_val);}
            //if(iter->getFieldValue(parent, "typeDerivat", str_val)) {to_value(enm, str_val); this->typeDerivat = (NLayerDerivat)enm;}
            else if(iter->getFieldValue(parent, "typeDerivat", str_val)) {to_value(this->typeDerivat, str_val);}
            else if(iter->getFieldValue(parent, "dropout", str_val)) {to_value(this->dropout, str_val);}

            ind++;
        }

        dt.setShift(ind);

        //Выполнение операций после загрузки
        this->setLock(lk);
        if(this->output.empty()) {this->output.init(this->getLenNeurons(), 0);}
        if(this->sum.empty()) {this->sum.init(this->getLenNeurons(), 0);}
        this->updateFuncNeurons();
    }
}

#endif // INLAYER_H
