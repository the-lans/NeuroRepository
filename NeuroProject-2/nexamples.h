#ifndef NEXAMPLES_H
#define NEXAMPLES_H

#include "narray.h"
#include "nexample.h"
#include "ngroupenergy.h"
#include "iobjectecsv.h"

template <typename NType>
class NExamples: public NArray<NExample<NType>*>, public IObjectECSV
{
public:
    NExamples();
    virtual ~NExamples();

public:
    //NArray<NExample<NType>*> exam;
    //NArray<NType> outpostrun; //Выходной вектор после постобработки
    NArray<NType>* outrun; //Указатель на выходной расчётный вектор примера
    NMatrix<NType>* tab; //Таблица для предобработки данных

public:
    NType (*funcNormalizationIn)(NType&, NType&); //Функция нормализации входа
    NType (*funcNormalizationOut)(NType&, NType&); //Функция нормализации выхода
    NType (*reversNormalizationOut)(NType&, NType&);

protected:
    NArray<NExample<NType>*> original; //Оригинальное расположение примеров
    bool blOriginal; //Переключатель восстановленных примеров
    int beginset; //Начало множества примеров
    int testset; //Разделение обучающего и тестового множеств
    int validset; //Разделение тестового и валидационного множеств
    int endset; //Конец множества примеров
    int limitset; //Ограничение на загрузку примеров
    int lenIn; //Количество входов
    int lenOut; //Количество выходов
    bool blTabExtern; //Признак внешних данных для таблицы tab
    bool blClass; //Преобразование номера класса в вектор
    bool blFilter; //Фильтр выходного вектора
    NType valFilter; //Значение фильтра
    NType energyAver; //Средняя энергия примеров
    NType energyMax; //Максимальная энергия примеров
    NType energySum; //Сумма энергий примеров
    NNormalizationFunc typeNormIn; //Тип нормализации входа
    NNormalizationFunc typeNormOut; //Тип нормализации выхода
    NType inMin; //Минимум входа
    NType inMax; //Максимум входа
    NType outMin; //Минимум выхода
    NType outMax; //Максимум выхода
    NType inKoef; //Коэффициент входа
    NType outKoef; //Коэффициент выхода
    int lenRand; //Количество случайных примеров
    bool blClearTypeSet; //Обучение Time: Очищать тип примера при перемещении окна обучения?

public:
    void setTestset(int val);
    void setValidset(int val);
    void setBeginset(int val);
    void setEndset(int val);
    void setLimitset(int val);
    void setBlOriginal(bool bl);
    int getTestset();
    int getValidset();
    int getBeginset();
    int getEndset();
    int getLimitset();
    bool getBlOriginal();
    void setEnergyAver(NType enrg);
    void setEnergyMax(NType enrg);
    void setEnergySum(NType enrg);
    NType getEnergyAver();
    NType getEnergyMax();
    NType getEnergySum();
    int getLenIn();
    int getLenOut();

public:
    void shiftset(int delta); //Смещение выборок по массиву примеров
    virtual void doShift(int shift, bool blShift); //Переместить на величину
    void portion(int begExm);
    void portion(int begExm, int trainExm, int testExm, int validExm); //Задание значений обучающей, тестовой и валидационной выборок
    void portion(int begExm, int endExm, int trainExm, int testExm, int validExm);
    void filloutput(); //Заполнить выходной эталонный вектор
    void filloutrun(); //Заполнить выходной расчётный вектор также как эталонный
    virtual void deinit(); //Деинициализация
    void emptyExamples(); //Очистка примеров
    void update(); //Обновление оригинального расположения примеров
    void recovery(); //Восстановление последовательности примеров из оригинального расположения
    void mixset(NSetType st); //Перемешивание примеров
    void mixTrain(); //Перемешать обучающее множество примеров
    void mixTest(); //Перемешать обучающее + тестовое множество примеров
    void mixTypeSet(); //Перемешать примеры в зависимости от типа
    void initTypeSet(); //Установить тип примеров
    void deinitTypeSet(); //Обнулить тип примеров
    NType funcNormIn(NType& x);
    NType funcNormOut(NType& x);
    NType funcReversNormOut(NType& y);
    void updateFunc(); //Обновить функции нормализации
    virtual void prerun(); //Предобработка данных
    virtual void postrun(); //Постобработка данных
    void getGroupEnergy(NGroupEnergy<NType>& ge); //Возврат групповой энергии
    void setGroupEnergy(NGroupEnergy<NType>& ge); //Установка групповой энергии

public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NExamples<NType>::NExamples()
{
    this->testset = 0;
    this->validset = 0;
    this->beginset = 0;
    this->endset = 0;
    this->limitset = 0;
    this->energyAver = 0;
    this->energyMax = 0;
    this->energySum = 0;
    this->lenIn = 0;
    this->lenOut = 0;
    this->blTabExtern = true;
    this->blClass = false;
    this->blFilter = false;
    this->outrun = nullptr;
    this->tab = nullptr;
    this->blOriginal = true;
    this->typeNormIn = NNormalizationFunc::NNormNone;
    this->typeNormOut = NNormalizationFunc::NNormNone;
    this->funcNormalizationIn = nullptr;
    this->funcNormalizationOut = nullptr;
    this->reversNormalizationOut = nullptr;
    this->inMin = 0;
    this->inMax = 1;
    this->outMin = 0;
    this->outMax = 1;
    this->inKoef = 1;
    this->outKoef = 1;
    this->lenRand = 0;
    this->blClearTypeSet = false;
}

template <typename NType>
NExamples<NType>::~NExamples()
{
    this->deinit();
}

template <typename NType>
void NExamples<NType>::setTestset(int val)
{
    this->testset = val;
}

template <typename NType>
void NExamples<NType>::setValidset(int val)
{
    this->validset = val;
}

template <typename NType>
int NExamples<NType>::getTestset()
{
    return this->testset;
}

template <typename NType>
int NExamples<NType>::getValidset()
{
    return this->validset;
}

template <typename NType>
void NExamples<NType>::setBeginset(int val)
{
    this->beginset = val;
}

template <typename NType>
void NExamples<NType>::setEndset(int val)
{
    this->endset = val;
}

template <typename NType>
void NExamples<NType>::setLimitset(int val)
{
    this->limitset = val;
}

template <typename NType>
void NExamples<NType>::setBlOriginal(bool bl)
{
    this->blOriginal = bl;
}

template <typename NType>
int NExamples<NType>::getBeginset()
{
    return this->beginset;
}

template <typename NType>
int NExamples<NType>::getEndset()
{
    return this->endset;
}

template <typename NType>
int NExamples<NType>::getLimitset()
{
    return this->limitset;
}

template <typename NType>
bool NExamples<NType>::getBlOriginal()
{
    return this->blOriginal;
}

template <typename NType>
void NExamples<NType>::setEnergyAver(NType enrg)
{
    this->energyAver = enrg;
}

template <typename NType>
void NExamples<NType>::setEnergyMax(NType enrg)
{
    this->energyMax = enrg;
}

template <typename NType>
void NExamples<NType>::setEnergySum(NType enrg)
{
    this->energySum = enrg;
}

template <typename NType>
NType NExamples<NType>::getEnergyAver()
{
    return this->energyAver;
}

template <typename NType>
NType NExamples<NType>::getEnergyMax()
{
    return this->energyMax;
}

template <typename NType>
NType NExamples<NType>::getEnergySum()
{
    return this->energySum;
}

template <typename NType>
int NExamples<NType>::getLenIn()
{
    return this->lenIn;
}

template <typename NType>
int NExamples<NType>::getLenOut()
{
    return this->lenOut;
}

template <typename NType>
void NExamples<NType>::shiftset(int delta)
{
    this->beginset += delta;
    this->testset += delta;
    this->validset += delta;
    this->endset += delta;
}

template <typename NType>
void NExamples<NType>::doShift(int shift, bool blShift)
{
    NExample<NType> *exm1, *exm2;
    int ind;

    //Задание типов примерам
    for(ind = 0; ind < shift; ind++)
    {
        exm1 = this->get(this->beginset + ind);
        if(blShift)
        {
            exm2 = this->get(this->endset + ind);
            exm2->setTypeSet(exm1->getTypeSet());
        }
        if(blClearTypeSet)
        {
            //exm1->setEnergy(0);
            exm1->setTypeSet(NSetType::NSetNone);
        }
    }

    this->shiftset(shift);
}

template <typename NType>
void NExamples<NType>::portion(int begExm)
{
    this->shiftset(begExm - this->beginset);
}

template <typename NType>
void NExamples<NType>::portion(int begExm, int trainExm, int testExm, int validExm)
{
    this->beginset = begExm;
    this->testset = this->beginset + trainExm;
    this->validset = this->testset + testExm;
    this->endset = this->validset + validExm;
}

template <typename NType>
void NExamples<NType>::portion(int begExm, int endExm, int trainExm, int testExm, int validExm)
{
    int num = endExm - begExm;
    int sum = trainExm + testExm + validExm;
    this->endset = endExm;
    this->validset = this->endset - num * ((double)validExm / sum);
    this->testset = this->validset - num * ((double)testExm / sum);
    this->beginset = begExm;
}

template <typename NType>
void NExamples<NType>::filloutput()
{
    NExample<NType>* exm;
    for(int i = 0; i < this->length; i++)
    {
        exm = this->get(i);
        if(exm->output.empty()) {exm->output.init(this->lenOut, 0);}
    }
}

template <typename NType>
void NExamples<NType>::filloutrun()
{
    NExample<NType>* exm;
    for(int i = 0; i < this->length; i++)
    {
        exm = this->get(i);
        exm->outrun = exm->output;
    }
}

template <typename NType>
void NExamples<NType>::deinit()
{
    this->emptyExamples();
    if(!this->blTabExtern && this->tab != nullptr) {delete this->tab;}
    this->lenIn = 0;
    this->lenOut = 0;

    this->original.clear();
    //this->outpostrun.clear();
    this->blOriginal = true;
    this->blTabExtern = true;

    this->energyAver = 0;
    this->energyMax = 0;
    this->energySum = 0;
    this->typeNormIn = NNormalizationFunc::NNormNone;
    this->typeNormOut = NNormalizationFunc::NNormNone;
    this->funcNormalizationIn = nullptr;
    this->funcNormalizationOut = nullptr;
    this->reversNormalizationOut = nullptr;
    this->inMin = 0;
    this->inMax = 1;
    this->outMin = 0;
    this->outMax = 1;
    this->inKoef = 1;
    this->outKoef = 1;
    this->lenRand = 0;
}

template <typename NType>
void NExamples<NType>::emptyExamples()
{
    NExample<NType>* exm;
    int num = this->length;
    for(int i = 0; i < num; i++)
    {
        exm = this->pop();
        delete exm;
    }
}

template <typename NType>
void NExamples<NType>::update()
{
    this->original = *((NArray<NExample<NType>*>*)this);
}

template <typename NType>
void NExamples<NType>::recovery()
{
    *((NArray<NExample<NType>*>*)this) = this->original;
    this->blOriginal = true;
}

template <typename NType>
void NExamples<NType>::mixset(NSetType st)
{
    if(st == NSetType::NSetTrain)
    {
        this->mix(this->beginset, this->testset);
    }
    else if(st == NSetType::NSetTest)
    {
        this->mix(this->testset, this->validset);
    }
    else if(st == NSetType::NSetValidate)
    {
        this->mix(this->validset, this->endset);
    }
    else
    {
        this->mix(this->beginset, this->endset);
    }

    this->blOriginal = false;
}

template <typename NType>
void NExamples<NType>::mixTrain()
{
    this->mix(this->beginset, this->testset);
    this->blOriginal = false;
}

template <typename NType>
void NExamples<NType>::mixTest()
{
    this->mix(this->beginset, this->validset);
    this->blOriginal = false;
}

template <typename NType>
void NExamples<NType>::mixTypeSet()
{
    int ind_Train = this->beginset;
    int ind_Test = this->testset;
    int ind_Valid = this->validset;
    NArray<NExample<NType>*> arrexm = *((NArray<NExample<NType>*>*)this);
    NExample<NType>* exm;

    for(int ind = this->beginset; ind < this->endset; ind++)
    {
        exm = arrexm[ind];
        if(exm->getTypeSet() == NSetType::NSetTrain)
        {
            this->set(exm, ind_Train);
            ind_Train++;
        }
        else if(exm->getTypeSet() == NSetType::NSetTest)
        {
            this->set(exm, ind_Test);
            ind_Test++;
        }
        else if(exm->getTypeSet() == NSetType::NSetValidate)
        {
            this->set(exm, ind_Valid);
            ind_Valid++;
        }
    }

    this->blOriginal = false;
    if(ind_Train > this->testset) {throw std::out_of_range("testset: not of the range");}
    if(ind_Test > this->validset) {throw std::out_of_range("validset: not of the range");}
    if(ind_Valid > this->endset) {throw std::out_of_range("endset: not of the range");}
}

template <typename NType>
void NExamples<NType>::initTypeSet()
{
    for(int ind = this->beginset; ind < this->testset; ind++) {this->get(ind)->setTypeSet(NSetType::NSetTrain);}
    for(int ind = this->testset; ind < this->validset; ind++) {this->get(ind)->setTypeSet(NSetType::NSetTest);}
    for(int ind = this->validset; ind < this->endset; ind++) {this->get(ind)->setTypeSet(NSetType::NSetValidate);}
}

template <typename NType>
void NExamples<NType>::deinitTypeSet()
{
    for(int ind = this->beginset; ind < this->endset; ind++) {this->get(ind)->setTypeSet(NSetType::NSetNone);}
}

template <typename NType>
NType NExamples<NType>::funcNormIn(NType& x)
{
    if(this->funcNormalizationIn == nullptr) {return x;}
    else {return this->funcNormalizationIn(x, this->inKoef);}
}

template <typename NType>
NType NExamples<NType>::funcNormOut(NType& x)
{
    if(this->funcNormalizationOut == nullptr) {return x;}
    else {return this->funcNormalizationOut(x, this->outKoef);}
}

template <typename NType>
NType NExamples<NType>::funcReversNormOut(NType& y)
{
    if(this->reversNormalizationOut == nullptr) {return y;}
    else {return this->reversNormalizationOut(y, this->outKoef);}
}

template <typename NType>
void NExamples<NType>::updateFunc()
{
         if(this->typeNormIn == NNormalizationFunc::NNormLinear) {this->funcNormalizationIn = funcLinear;}
    else if(this->typeNormIn == NNormalizationFunc::NNormSoftStep) {this->funcNormalizationIn = funcSoftStep;}
    else if(this->typeNormIn == NNormalizationFunc::NNormArctg) {this->funcNormalizationIn = funcArctg;}
    else if(this->typeNormIn == NNormalizationFunc::NNormGaussian) {this->funcNormalizationIn = funcGaussian;}
    else {this->funcNormalizationIn = nullptr;}

         if(this->typeNormOut == NNormalizationFunc::NNormLinear) {this->funcNormalizationOut = funcLinear;}
    else if(this->typeNormOut == NNormalizationFunc::NNormSoftStep) {this->funcNormalizationOut = funcSoftStep;}
    else if(this->typeNormOut == NNormalizationFunc::NNormArctg) {this->funcNormalizationOut = funcArctg;}
    else if(this->typeNormOut == NNormalizationFunc::NNormGaussian) {this->funcNormalizationOut = funcGaussian;}
    else {this->funcNormalizationOut = nullptr;}

         if(this->typeNormOut == NNormalizationFunc::NNormLinear) {this->reversNormalizationOut = reversLinear;}
    else if(this->typeNormOut == NNormalizationFunc::NNormSoftStep) {this->reversNormalizationOut = reversSoftStep;}
    else if(this->typeNormOut == NNormalizationFunc::NNormArctg) {this->reversNormalizationOut = reversArctg;}
    else if(this->typeNormOut == NNormalizationFunc::NNormGaussian) {this->reversNormalizationOut = reversGaussian;}
    else {this->reversNormalizationOut = nullptr;}
}

template <typename NType>
void NExamples<NType>::prerun()
{
    int ind, jnd, count;
    int shift;
    NType value, value_class;
    NType inDelta = this->inMax - this->inMin;
    NType outDelta = this->outMax - this->outMin;

    if(this->tab != nullptr)
    {
        this->emptyExamples();
        if(this->blTabExtern)
        {
            this->lenIn = tab->getLenColumn();
            this->lenOut = 0;
        }

        //Рандомные значения
        uniform_real_distribution<> distribution(this->inMin, this->inMax);
        NArray<NType> vec(tab->getSizeColumn());
        vec.init(tab->getLenColumn(), 0);
        for(ind = 0; ind < this->lenRand; ind++)
        {
            //vec.init_rand(NRandGenerator, this->inMin, this->inMax);
            for(jnd = 0; jnd < this->lenIn; jnd++)
            {
                vec[jnd] = distribution(NRandGenerator);
            }
            tab->pushRow(vec);
        }

        //Предобработка примеров
        count = (this->limitset == 0 || this->limitset > tab->getLenRow() ? tab->getLenRow() : this->limitset);
        if(this->blTabExtern)
        {
            this->lenIn = tab->getLenColumn();
            this->lenOut = 0;
        }

        for(ind = 0; ind < count; ind++)
        {
            NExample<NType>* pExam = new NExample<NType>();

            //Вход
            for(jnd = 0; jnd < this->lenIn; jnd++)
            {
                value = tab->get(ind, jnd);
                if(this->typeNormIn == NNormalizationFunc::NNormLinear) {value = (value - this->inMin)/inDelta;}
                value = this->funcNormIn(value);
                pExam->input.push(value);
            }

            //Выход
            if(this->blClass)
            {
                for(shift = this->lenIn; shift < this->tab->getLenColumn(); shift++)
                {
                    value_class = tab->get(ind, shift);
                    for(jnd = 1; jnd <= this->lenOut; jnd++)
                    {
                        value = (isEqual(value_class, (NType)jnd) ? 1.0 : 0.0);
                        pExam->output.push(value);
                    }
                }
            }
            else
            {
                for(jnd = this->lenIn; jnd < this->tab->getLenColumn(); jnd++)
                {
                    value = tab->get(ind, jnd);
                    if(this->typeNormOut == NNormalizationFunc::NNormLinear) {value = (value - this->outMin)/outDelta;}
                    value = this->funcNormOut(value);
                    pExam->output.push(value);
                }
            }

            this->push(pExam);
        }
    }

    this->filloutput();
    this->filloutrun();
    this->update();
}

template <typename NType>
void NExamples<NType>::postrun()
{
    int ind, pos;
    NType value;
    NExample<NType>* exm;
    NType outDelta = this->outMax - this->outMin;

    if(!this->blOriginal) {this->recovery();}

    for(pos = this->beginset; pos < this->endset; pos++)
    {
        exm = this->get(pos);
        exm->outpostrun.clear();

        if(this->blFilter)
        {
            if(this->blClass)
            {
                value = 0;
                for(ind = 0; ind < exm->outrun.getLength(); ind++)
                {
                    if(exm->outrun[ind] > this->valFilter) {value = ind+1;}
                }
                exm->outpostrun.push(value);
            }
            else
            {
                for(ind = 0; ind < exm->outrun.getLength(); ind++)
                {
                    value = exm->outrun[ind];
                    if(value > this->valFilter) {value = 1;}
                    else if(value < -this->valFilter) {value = -1;}
                    else {value = 0;}
                    exm->outpostrun.push(value);
                }
            }
        }
        else
        {
            for(ind = 0; ind < exm->outrun.getLength(); ind++)
            {
                value = exm->outrun[ind];
                value = this->funcReversNormOut(value);
                if(this->typeNormOut == NNormalizationFunc::NNormLinear) {value = value * outDelta + this->outMin;}
                exm->outpostrun.push(value);
            }
        }
    }
}

template <typename NType>
void NExamples<NType>::getGroupEnergy(NGroupEnergy<NType>& ge)
{
    ge.energyAver = this->energyAver;
    ge.energyMax = this->energyMax;
    ge.energySum = this->energySum;
}

template <typename NType>
void NExamples<NType>::setGroupEnergy(NGroupEnergy<NType>& ge)
{
    this->energyAver = ge.energyAver;
    this->energyMax = ge.energyMax;
    this->energySum = ge.energySum;
}

template <typename NType>
void NExamples<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<NType> mtrx;
    NExample<NType>* exm;
    NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;
    string field = parent + ".data";

    //Выполнение операций перед выгрузкой
    if(!this->blOriginal) {this->recovery();}

    dt.addGroup(parent, "");
    str_val = to_string(this->beginset); dt.addElement(parent, "beginset", str_val, typeid(int).name());
    str_val = to_string(this->testset); dt.addElement(parent, "testset", str_val, typeid(int).name());
    str_val = to_string(this->validset); dt.addElement(parent, "validset", str_val, typeid(int).name());
    str_val = to_string(this->endset); dt.addElement(parent, "endset", str_val, typeid(int).name());
    str_val = to_string(this->limitset); dt.addElement(parent, "limitset", str_val, typeid(int).name());

    str_val = to_string(this->lenIn); dt.addElement(parent, "lenIn", str_val, typeid(int).name());
    str_val = to_string(this->lenOut); dt.addElement(parent, "lenOut", str_val, typeid(int).name());
    str_val = to_vstring(this->blClass); dt.addElement(parent, "blClass", str_val, typeid(bool).name());
    str_val = to_vstring(this->blFilter); dt.addElement(parent, "blFilter", str_val, typeid(bool).name());
    str_val = to_string(this->valFilter); dt.addElement(parent, "valFilter", str_val, typeid(NType).name());

    str_val = to_string(this->typeNormIn); dt.addElement(parent, "typeNormIn", str_val, typeid(int).name());
    str_val = to_string(this->typeNormOut); dt.addElement(parent, "typeNormOut", str_val, typeid(int).name());
    str_val = to_string(this->inMin); dt.addElement(parent, "inMin", str_val, typeid(NType).name());
    str_val = to_string(this->inMax); dt.addElement(parent, "inMax", str_val, typeid(NType).name());
    str_val = to_string(this->outMin); dt.addElement(parent, "outMin", str_val, typeid(NType).name());
    str_val = to_string(this->outMax); dt.addElement(parent, "outMax", str_val, typeid(NType).name());
    str_val = to_string(this->inKoef); dt.addElement(parent, "inKoef", str_val, typeid(NType).name());
    str_val = to_string(this->outKoef); dt.addElement(parent, "outKoef", str_val, typeid(NType).name());
    str_val = to_string(this->lenRand); dt.addElement(parent, "lenRand", str_val, typeid(int).name());
    str_val = to_vstring(this->blClearTypeSet); dt.addElement(parent, "blClearTypeSet", str_val, typeid(bool).name());

    str_val = to_string(this->energyAver); dt.addElement(parent, "energyAver", str_val, typeid(NType).name());
    str_val = to_string(this->energyMax); dt.addElement(parent, "energyMax", str_val, typeid(NType).name());
    str_val = to_string(this->energySum); dt.addElement(parent, "energySum", str_val, typeid(NType).name());

    mtrx.clear();
    for(int i = 0; i < this->getLength(); i++) {exm = this->get(i); mtrx.pushRow(exm->input);}
    to_matrix_string(str_mtrx, mtrx); dt.addElement(field, "input", str_mtrx, typeid(NType).name());

    mtrx.clear();
    for(int i = 0; i < this->getLength(); i++) {exm = this->get(i); mtrx.pushRow(exm->output);}
    to_matrix_string(str_mtrx, mtrx); dt.addElement(field, "output", str_mtrx, typeid(NType).name());

    mtrx.clear();
    for(int i = 0; i < this->getLength(); i++) {exm = this->get(i); mtrx.pushRow(exm->outrun);}
    to_matrix_string(str_mtrx, mtrx); dt.addElement(field, "outrun", str_mtrx, typeid(NType).name());

    mtrx.clear();
    for(int i = this->beginset; i < this->endset; i++) {exm = this->get(i); mtrx.pushRow(exm->outpostrun);}
    to_matrix_string(str_mtrx, mtrx); dt.addElement(field, "outpostrun", str_mtrx, typeid(NType).name());
}

template <typename NType>
void NExamples<NType>::loadECSV(DataECSV& dt, string& parent)
{
    NMatrix<NType> mtrx_input, mtrx_output, mtrx_outrun, mtrx_outpostrun;
    NExample<NType>* exm;
    StructECSV* iter;
    NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;
    vector<string> subpath;
    string field = parent + ".data";

    //Выполнение операций перед загрузкой
    NExamples<NType>::deinit();

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        to_matrix_value(mtrx_input, iter->mtrx);

        //Субполя класса
        for(int pos = 0; pos < mtrx_input.getLenRow(); pos++)
        {
            exm = new NExample<NType>();
            mtrx_input.getRow(pos, exm->input);
            this->push(exm);
        }
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
            //cout << iter->getPath() << endl;

                 if(iter->getFieldValue(parent, "beginset", str_val)) {to_value(this->beginset, str_val);}
            else if(iter->getFieldValue(parent, "testset", str_val)) {to_value(this->testset, str_val);}
            else if(iter->getFieldValue(parent, "validset", str_val)) {to_value(this->validset, str_val);}
            else if(iter->getFieldValue(parent, "endset", str_val)) {to_value(this->endset, str_val);}
            else if(iter->getFieldValue(parent, "limitset", str_val)) {to_value(this->limitset, str_val);}

            else if(iter->getFieldValue(parent, "lenIn", str_val)) {to_value(this->lenIn, str_val);}
            else if(iter->getFieldValue(parent, "lenOut", str_val)) {to_value(this->lenOut, str_val);}
            else if(iter->getFieldValue(parent, "blClass", str_val)) {to_value(this->blClass, str_val);}
            else if(iter->getFieldValue(parent, "blFilter", str_val)) {to_value(this->blFilter, str_val);}
            else if(iter->getFieldValue(parent, "valFilter", str_val)) {to_value(this->valFilter, str_val);}

            else if(iter->getFieldValue(parent, "typeNormIn", str_val)) {to_value(this->typeNormIn, str_val);}
            else if(iter->getFieldValue(parent, "typeNormOut", str_val)) {to_value(this->typeNormOut, str_val);}
            else if(iter->getFieldValue(parent, "inMin", str_val)) {to_value(this->inMin, str_val);}
            else if(iter->getFieldValue(parent, "inMax", str_val)) {to_value(this->inMax, str_val);}
            else if(iter->getFieldValue(parent, "outMin", str_val)) {to_value(this->outMin, str_val);}
            else if(iter->getFieldValue(parent, "outMax", str_val)) {to_value(this->outMax, str_val);}
            else if(iter->getFieldValue(parent, "inKoef", str_val)) {to_value(this->inKoef, str_val);}
            else if(iter->getFieldValue(parent, "outKoef", str_val)) {to_value(this->outKoef, str_val);}
            else if(iter->getFieldValue(parent, "lenRand", str_val)) {to_value(this->lenRand, str_val);}
            else if(iter->getFieldValue(parent, "blClearTypeSet", str_val)) {to_value(this->blClearTypeSet, str_val);}

            else if(iter->getFieldValue(parent, "energyAver", str_val)) {to_value(this->energyAver, str_val);}
            else if(iter->getFieldValue(parent, "energyMax", str_val)) {to_value(this->energyMax, str_val);}
            else if(iter->getFieldValue(parent, "energySum", str_val)) {to_value(this->energySum, str_val);}

            else if(iter->getFieldValue(field, "input", str_mtrx)) {to_matrix_value(mtrx_input, str_mtrx);}
            else if(iter->getFieldValue(field, "output", str_mtrx)) {to_matrix_value(mtrx_output, str_mtrx);}
            else if(iter->getFieldValue(field, "outrun", str_mtrx)) {to_matrix_value(mtrx_outrun, str_mtrx);}
            else if(iter->getFieldValue(field, "outpostrun", str_mtrx)) {to_matrix_value(mtrx_outpostrun, str_mtrx);}

            else if(iter->getFieldValue(field, "tab", str_mtrx))
            {
                this->tab = new NMatrix<NType>;
                this->blTabExtern = false;
                to_matrix_value(*(this->tab), str_mtrx);
            }

            ind++;
        }

        dt.setShift(ind);

        //Субполя класса
        int lenVec = mtrx_output.getLenColumn();
        for(int pos = 0; pos < mtrx_input.getLenRow(); pos++)
        {
            exm = new NExample<NType>();
            mtrx_input.getRow(pos, exm->input);
            if(pos < mtrx_output.getLenRow()) {mtrx_output.getRow(pos, exm->output);} else {exm->output.init(lenVec, 0);}
            if(mtrx_outrun.empty()) {exm->outrun.init(lenVec, 0);} else {mtrx_outrun.getRow(pos, exm->outrun);}
            if(mtrx_outpostrun.empty()) {exm->outpostrun.init(lenVec, 0);} else {mtrx_outpostrun.getRow(pos, exm->outpostrun);}
            this->push(exm);
        }
    }

    //Выполнение операций после загрузки
    if(!mtrx_input.empty() && this->lenIn == 0) {this->lenIn = mtrx_input.getLenColumn();}
    if(!mtrx_output.empty() && this->lenOut == 0) {this->lenOut = mtrx_output.getLenColumn();}
    this->update();
    this->updateFunc();
}

#endif // NEXAMPLES_H
