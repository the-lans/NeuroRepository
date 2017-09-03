#ifndef NEXAMPLES_H
#define NEXAMPLES_H

#include "narray.h"
#include "nexample.h"
#include "iobjectecsv.h"

template <typename NType>
class NExamples: public NArray<NExample<NType>*>, public IObjectECSV
{
public:
    NExamples();
    virtual ~NExamples();
public:
    //NArray<NExample<NType>*> exam;
    NArray<NType> outpostrun; //Выходной вектор после постобработки
    NArray<NType>* outrun; //Указатель на выходной расчётный вектор примера
protected:
    int beginset; //Начало множества примеров
    int testset; //Разделение обучающего и тестового множеств
    int endset; //Конец множества примеров
    NType energyAver; //Средняя энергия примеров
    NType energyMax; //Максимальная энергия примеров
    NType energySum; //Сумма энергий примеров
public:
    void setTestset(int testset);
    void setBeginset(int val);
    void setEndset(int val);
    int getTestset();
    int getBeginset();
    int getEndset();
    void setEnergyAver(NType enrg);
    void setEnergyMax(NType enrg);
    void setEnergySum(NType enrg);
    NType getEnergyAver();
    NType getEnergyMax();
    NType getEnergySum();
public:
    virtual void deinit(); //Деинициализация
    void emptyExamples(); //Очистка примеров
    virtual void prerun(NMatrix<NType>& tab); //Предобработка данных
    virtual NArray<NType>* postrun(); //Постобработка данных
public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NExamples<NType>::NExamples()
{
    this->testset = 0;
    this->beginset = 0;
    this->endset = 0;
    this->energyAver = 0;
    this->energyMax = 0;
    this->energySum = 0;
    this->outrun = nullptr;
}

template <typename NType>
NExamples<NType>::~NExamples()
{
    this->deinit();
}

template <typename NType>
void NExamples<NType>::setTestset(int testset)
{
    this->testset = testset;
}

template <typename NType>
int NExamples<NType>::getTestset()
{
    return this->testset;
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
void NExamples<NType>::deinit()
{
    this->emptyExamples();
    this->outpostrun.clear();
}

template <typename NType>
void NExamples<NType>::emptyExamples()
{
    NExample<NType>* exm;
    for(int i = 0; i < this->getLength(); i++)
    {
        exm = this->pop();
        delete exm;
    }
}

template <typename NType>
void NExamples<NType>::prerun(NMatrix<NType>& tab)
{
    int ind, jnd;
    NType value;
    this->emptyExamples();

    for(ind = 0; ind < tab.getLenRow(); ind++)
    {
        NExample<NType>* pExam = new NExample<NType>();
        for(jnd = 0; jnd < tab.getLenColumn(); jnd++)
        {
            value = tab[ind][jnd];
            pExam->input.push(value);
        }
        this->push(pExam);
    }

    this->beginset = 0;
    this->testset = this->getLength();
    this->endset = this->getLength();
}

template <typename NType>
NArray<NType>* NExamples<NType>::postrun()
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

template <typename NType>
void NExamples<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NExample<NType>* exm;
    //NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;
    string field;

    str_val = to_string(this->beginset); dt.addElement(parent, "beginset", str_val, typeid(int).name());
    str_val = to_string(this->testset); dt.addElement(parent, "testset", str_val, typeid(int).name());
    str_val = to_string(this->endset); dt.addElement(parent, "endset", str_val, typeid(int).name());

    str_val = to_string(this->energyAver); dt.addElement(parent, "energyAver", str_val, typeid(NType).name());
    str_val = to_string(this->energyMax); dt.addElement(parent, "energyMax", str_val, typeid(NType).name());
    str_val = to_string(this->energySum); dt.addElement(parent, "energySum", str_val, typeid(NType).name());

    field = parent + ".data";
    for(int i = 0; i < this->getLength(); i++)
    {
        exm = this->get(i);
        exm->saveECSV(dt, field);
    }
}

template <typename NType>
void NExamples<NType>::loadECSV(DataECSV& dt, string& parent)
{
    NExample<NType>* exm;
    StructECSV* iter;
    //unsigned int enm;
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    string field;
    vector<string> subpath;
    bool* isLoad = newbool(6, false);

    //Выполнение операций перед загрузкой
    NExamples<NType>::deinit();

    if(dt.isOneMatrix()) //Одиночная матрица CSV
    {
        iter = dt.modules[0];
        for(int i = 0; i < iter->mtrx.getLenRow(); i++)
        {
            exm = this->get(i);
            iter->mtrx.popRow(str_vec);
            to_array_value(exm->input, str_vec);
        }
    }
    else
    {
        //Поля класса
        size_t ind = dt.getShift();
        while(!booland(isLoad, 6) && ind < dt.modules.size())
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "beginset", str_val)) {to_value(this->beginset, str_val); isLoad[0] = true;}
            if(iter->getFieldValue(parent, "testset", str_val)) {to_value(this->testset, str_val); isLoad[1] = true;}
            if(iter->getFieldValue(parent, "endset", str_val)) {to_value(this->endset, str_val); isLoad[2] = true;}

            if(iter->getFieldValue(parent, "energyAver", str_val)) {to_value(this->energyAver, str_val); isLoad[3] = true;}
            if(iter->getFieldValue(parent, "energyMax", str_val)) {to_value(this->energyMax, str_val); isLoad[4] = true;}
            if(iter->getFieldValue(parent, "energySum", str_val)) {to_value(this->energySum, str_val); isLoad[5] = true;}

            ind++;
        }

        //Субполя класса
        if(ind >= dt.modules.size()) {ind = dt.getShift();}
        dt.setShift(ind);

        field = parent + ".data";
        dt.splitPath(subpath, field);

        while(ind < dt.modules.size())
        {
            iter = dt.modules[ind];
            if(iter->cmpPath(subpath))
            {
                exm = new NExample<NType>();
                exm->loadECSV(dt, field);
                this->push(exm);
                ind = dt.getShift();
            }
            else
            {
                ind++; dt.setShift(ind);
            }
        }
    }
}

#endif // NEXAMPLES_H
