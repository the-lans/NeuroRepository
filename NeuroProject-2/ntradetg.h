#ifndef NTRADETG_H
#define NTRADETG_H

#include "nexamples.h"
#include <math.h>

enum NPriceType {PriceHigh, PriceLow, PriceClose};

template <typename NType>
class NTradeTg: public NExamples<NType>
{
public:
    NTradeTg();
    NTradeTg(NTradeTg<NType>& obj);
    virtual ~NTradeTg();
protected:
    int period; //Период агрегации баров
    NType koefTg; //Количество пунктов в цене
    NType koefPrice; //Коэффициент преобразования цены
    NType koefVolume; //Коэффициент преобразования объёма
    NPriceType pt; //Предсказание цены
    int lenIn; //Количество входных баров
    NMatrix<NType>* dataMtrx; //Указатель на таблицу с данными для предобработки
    NMatrix<NType> dataPeriod; //Бары, преобразованные по периодам
    NMatrix<NType> dataTg; //Преобразованная таблица данных
public:
    void setPeriod(int val);
    int getPeriod();
    void setKoefTg(NType val);
    NType getKoefTg();
    void setKoefPrice(NType val);
    NType getKoefPrice();
    void setKoefVolume(NType val);
    NType getKoefVolume();
    void setPT(NPriceType val);
    NPriceType getPT();
    void setLenIn(int val);
    int getLenIn();
    void setMtrx(NMatrix<NType>* mtrx);
public:
    virtual void deinit(); //Деинициализация
    virtual void prerun(NMatrix<NType>& tab); //Предобработка данных
    virtual NArray<NType>* postrun(); //Постобработка данных
    void calcPeriod(); //Бары, преобразованные по периодам
    void calcTg(); //Преобразованная таблица данных
    void calcExample(); //Формирование примеров
public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NTradeTg<NType>::NTradeTg():
    NExamples<NType>()
{
    this->koefTg = 10000;
    this->koefPrice = 1;
    this->koefVolume = 1;
    this->period = 1;
    this->pt = NPriceType::PriceClose;
    this->lenIn = 0;
    this->dataMtrx = nullptr;
}

template <typename NType>
NTradeTg<NType>::NTradeTg(NTradeTg<NType>& obj):
    NExamples<NType>(obj)
{
    this->koefTg = obj.getKoefTg();
    this->koefPrice = obj.getKoefPrice();
    this->koefVolume = obj.getKoefVolume();
    this->period = obj.getPeriod();
    this->pt = obj.getPT();
    this->lenIn = obj.getLenIn();
    this->dataMtrx = nullptr;
}

template <typename NType>
NTradeTg<NType>::~NTradeTg()
{
    this->deinit();
}

template <typename NType>
void NTradeTg<NType>::setPeriod(int val)
{
    this->period = val;
}

template <typename NType>
int NTradeTg<NType>::getPeriod()
{
    return this->period;
}

template <typename NType>
void NTradeTg<NType>::setKoefTg(NType val)
{
    this->koefTg = val;
}

template <typename NType>
NType NTradeTg<NType>::getKoefTg()
{
    return this->koefTg;
}

template <typename NType>
void NTradeTg<NType>::setKoefPrice(NType val)
{
    this->koefPrice = val;
}

template <typename NType>
NType NTradeTg<NType>::getKoefPrice()
{
    return this->koefPrice;
}

template <typename NType>
void NTradeTg<NType>::setKoefVolume(NType val)
{
    this->koefVolume = val;
}

template <typename NType>
NType NTradeTg<NType>::getKoefVolume()
{
    return this->koefVolume;
}

template <typename NType>
void NTradeTg<NType>::setPT(NPriceType val)
{
    this->pt = val;
}

template <typename NType>
NPriceType NTradeTg<NType>::getPT()
{
    return this->pt;
}

template <typename NType>
void NTradeTg<NType>::setLenIn(int val)
{
    this->lenIn = val;
}

template <typename NType>
int NTradeTg<NType>::getLenIn()
{
    return this->lenIn;
}

template <typename NType>
void NTradeTg<NType>::setMtrx(NMatrix<NType>* mtrx)
{
    this->dataMtrx = mtrx;
}

template <typename NType>
void NTradeTg<NType>::deinit()
{
    this->dataPeriod.clear();
    this->dataTg.clear();
    NExamples<NType>::deinit();
}

template <typename NType>
void NTradeTg<NType>::prerun(NMatrix<NType>& tab)
{
    this->dataMtrx = &tab;
    this->calcPeriod();
    this->calcTg();
    this->calcExample();
}

template <typename NType>
NArray<NType>* NTradeTg<NType>::postrun()
{
    this->outpostrun.clear();
    NType value = tan(this->outrun->get(0)) / (this->koefTg * this->koefPrice);
    this->outpostrun.push(value);
    return &(this->outpostrun);
}

template <typename NType>
void NTradeTg<NType>::calcPeriod()
{
    int ind, knd = 0;
    this->dataPeriod.init(this->dataMtrx->getLenRow() / this->period, 5, 0);

    NType vOpen = this->dataMtrx->get(0, 2);
    NType vClose = this->dataMtrx->get(0, 5);
    NType curHigh, vHigh = this->dataMtrx->get(0, 3);
    NType curLow, vLow = this->dataMtrx->get(0, 4);
    NType vVolume = 0;

    for(ind = 1; ind <= this->dataMtrx->getLenRow(); ind++)
    {
        vClose = this->dataMtrx->get(ind-1, 5);
        curHigh = this->dataMtrx->get(ind-1, 3);
        curLow = this->dataMtrx->get(ind-1, 4);
        vVolume += this->dataMtrx->get(ind-1, 6);

        if(vHigh < curHigh) {vHigh = curHigh;}
        if(vLow > curLow) {vLow = curLow;}
        if(ind % this->period == 0)
        {
            this->dataPeriod.set(vOpen, knd, 0);
            this->dataPeriod.set(vClose, knd, 3);
            this->dataPeriod.set(vHigh, knd, 1);
            this->dataPeriod.set(vLow, knd, 2);
            this->dataPeriod.set(vVolume, knd, 4);

            if(ind < this->dataMtrx->getLenRow())
            {
                vOpen = this->dataMtrx->get(ind, 2);
                vHigh = this->dataMtrx->get(ind, 3);
                vLow = this->dataMtrx->get(ind, 4);
                vVolume = 0;
                knd++;
            }
        }
    }
}

template <typename NType>
void NTradeTg<NType>::calcTg()
{
    this->dataTg.init(this->dataPeriod.getLenRow()-1, this->dataPeriod.getLenColumn(), 0);
    NType dValue;

    for(int ind = 1; ind < this->dataPeriod.getLenRow(); ind++)
    {
        for(int jnd = 0; jnd < this->dataPeriod.getLenColumn(); jnd++)
        {
            dValue = this->dataPeriod[ind][jnd] - this->dataPeriod[ind-1][jnd];
            if(jnd == this->dataPeriod.getLenColumn()-1) // Volume
            {
                dValue = this->koefVolume * dValue;
            }
            else // Price
            {
                dValue = this->koefTg * this->koefPrice * dValue;
            }
            dValue = atan(dValue);
            this->dataTg.set(dValue, ind-1, jnd);
        }
    }
}

template <typename NType>
void NTradeTg<NType>::calcExample()
{
    this->emptyExamples();
    int len = this->dataTg.getLenRow() - (this->lenIn + 1) + 1;
    NType value = 0;

    for(int ind = 0; ind < len; ind++)
    {
        NExample<NType>* pExam = new NExample<NType>();

        for(int knd = 0; knd < this->lenIn; knd++)
        {
            for(int jnd = 1; jnd < this->dataTg.getLenColumn(); jnd++)
            {
                value = this->dataTg[ind + knd][jnd];
                pExam->input.push(value);
            }
        }

        if(this->pt == NPriceType::PriceHigh)
        {
            value = this->dataTg[ind + this->lenIn][1];
        }
        else if(this->pt == NPriceType::PriceLow)
        {
            value = this->dataTg[ind + this->lenIn][2];
        }
        else if(this->pt == NPriceType::PriceClose)
        {
            value = this->dataTg[ind + this->lenIn][3];
        }
        pExam->output.push(value);

        this->push(pExam);
    }

    this->beginset = 0;
    this->testset = this->getLength();
    this->endset = this->getLength();
}

template <typename NType>
void NTradeTg<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    //string field;

    str_val = to_string(this->period); dt.addElement(parent, "period", str_val, typeid(int).name());
    str_val = to_string(this->koefTg); dt.addElement(parent, "koefTg", str_val, typeid(NType).name());
    str_val = to_string(this->koefPrice); dt.addElement(parent, "koefPrice", str_val, typeid(NType).name());
    str_val = to_string(this->koefVolume); dt.addElement(parent, "koefVolume", str_val, typeid(NType).name());
    str_val = to_string((unsigned int)this->pt); dt.addElement(parent, "pt", str_val, typeid(int).name());
    str_val = to_string(this->lenIn); dt.addElement(parent, "lenIn", str_val, typeid(int).name());

    to_matrix_string(str_mtrx, this->dataPeriod); dt.addElement(parent, "dataPeriod", str_mtrx, typeid(NType).name());
    to_matrix_string(str_mtrx, this->dataTg); dt.addElement(parent, "dataTg", str_mtrx, typeid(NType).name());

    NExamples<NType>::saveECSV(dt, parent);
}

template <typename NType>
void NTradeTg<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    //NExample<NType>* exm;
    unsigned int enm;
    NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;
    string field;
    bool* isLoad = newbool(8, false);

    NTradeTg<NType>::deinit();

    if(dt.isOneMatrix())
    {
        //((NExamples<NType>*)this)->loadECSV(dt, parent);
        NExamples<NType>::loadECSV(dt, parent);
    }
    else
    {
        size_t ind = dt.getShift();
        while(!booland(isLoad, 8) && ind < dt.modules.size())
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "period", str_val)) {to_value(this->period, str_val); isLoad[0] = true;}
            if(iter->getFieldValue(parent, "koefTg", str_val)) {to_value(this->koefTg, str_val); isLoad[1] = true;}
            if(iter->getFieldValue(parent, "koefPrice", str_val)) {to_value(this->koefPrice, str_val); isLoad[2] = true;}
            if(iter->getFieldValue(parent, "koefVolume", str_val)) {to_value(this->koefVolume, str_val); isLoad[3] = true;}
            if(iter->getFieldValue(parent, "pt", str_val)) {to_value(enm, str_val); this->pt = (NPriceType)enm; isLoad[4] = true;}
            if(iter->getFieldValue(parent, "lenIn", str_val)) {to_value(this->lenIn, str_val); isLoad[5] = true;}

            if(iter->getFieldValue(parent, "dataPeriod", str_mtrx)) {to_matrix_value(this->dataPeriod, str_mtrx); isLoad[6] = true;}
            if(iter->getFieldValue(parent, "dataTg", str_mtrx)) {to_matrix_value(this->dataTg, str_mtrx); isLoad[7] = true;}

            ind++;
        }

        //((NExamples<NType>*)this)->loadECSV(dt, parent);
        NExamples<NType>::loadECSV(dt, parent);
    }
}

#endif // NTRADETG_H
