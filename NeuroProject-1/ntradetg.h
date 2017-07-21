#ifndef NTRADETG_H
#define NTRADETG_H

#include "ineuronet.h"

enum NPriceType {PriceHigh, PriceLow, PriceClose};

template <typename NType>
class NTradeTg: public INeuroNet<NType>
{
public:
    NTradeTg();
    NTradeTg(NTradeTg<NType>& obj);
    NTradeTg(int size);
protected:
    int period;
    NPriceType pt;
    int lenIn;
    NMatrix<NType>* dataMtrx;
    NMatrix<NType> dataPeriod;
    NMatrix<NType> dataTg;
public:
    void setPeriod(int val);
    int getPeriod();
    void setPT(NPriceType val);
    NPriceType getPT();
    void setLenIn(int val);
    int getLenIn();
    void setMtrx(NMatrix<NType>* mtrx);
public:
    virtual void prerun(NMatrix<NType>& tab);
    virtual NArray<NType>* postrun();
    void calcPeriod();
    void calcTg();
    void calcExample();
};

#endif // NTRADETG_H
