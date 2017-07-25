#include "ntradetg.h"
#include <math.h>

template <typename NType>
NTradeTg<NType>::NTradeTg():
    INeuroNet<NType>()
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
    INeuroNet<NType>(obj)
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
NTradeTg<NType>::NTradeTg(int size):
    INeuroNet<NType>(size)
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

    NType vOpen = this->dataMtrx->get(0, 0);
    NType vClose = this->dataMtrx->get(0, 3);
    NType curHigh, vHigh = this->dataMtrx->get(0, 1);
    NType curLow, vLow = this->dataMtrx->get(0, 2);
    NType vVolume = 0;

    for(ind = 1; ind <= this->dataMtrx->getLenRow(); ind++)
    {
        vClose = this->dataMtrx->get(ind-1, 3);
        curHigh = this->dataMtrx->get(ind-1, 1);
        curLow = this->dataMtrx->get(ind-1, 2);
        vVolume += this->dataMtrx->get(ind-1, 4);

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
                vOpen = this->dataMtrx->get(ind, 0);
                vHigh = this->dataMtrx->get(ind, 1);
                vLow = this->dataMtrx->get(ind, 2);
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
            dValue = this->dataPeriod.get(ind, jnd) - this->dataPeriod.get(ind-1, jnd);
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
                value = this->dataTg.get(ind + knd, jnd);
                pExam->input.push(value);
            }
        }

        if(this->pt == NPriceType::PriceHigh)
        {
            value = this->dataTg.get(ind + this->lenIn, 1);
        }
        else if(this->pt == NPriceType::PriceLow)
        {
            value = this->dataTg.get(ind + this->lenIn, 2);
        }
        else if(this->pt == NPriceType::PriceClose)
        {
            value = this->dataTg.get(ind + this->lenIn, 3);
        }
        pExam->output.push(value);

        this->exam.push(pExam);
    }

    this->beginset = 0;
    this->testset = this->exam.getLength();
    this->endset = this->exam.getLength();
}
