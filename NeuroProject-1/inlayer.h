#ifndef INLAYER_H
#define INLAYER_H

#include "narray.h"
#include "nmatrix.h"
#include <iostream>
#include <ctime>
#include <math.h>

enum class NLayerType {NFuncNone, NFuncTanh, NFuncSoftsign, NFuncArctg, NFuncLinear};
enum class NLayerDerivat {NDerivatSum, NDerivatOut};

template <typename NType>
class INLayer
{
public:
    INLayer();
    INLayer(INLayer<NType>& obj);
    INLayer<NType>& operator=(INLayer<NType>& obj);
    virtual ~INLayer();
public:
    NMatrix<NType> weigth;
    NArray<NType> bias;
    NArray<NType> output;
    NArray<NType> sum;
protected:
    NType koef;
    NLayerType type;
    NLayerDerivat typeDerivat;
    NType energyRegularization;
public:
    void setKoef(NType koef);
    NType getKoef();
    NLayerType getType();
    NLayerDerivat getTypeDerivat();
    NType getEnergyRegularization();
public:
    virtual void init(NType value);
    virtual void init(int lenRow, int lenColumn, NType value);
    virtual void ginit(NType value);
    virtual void ginit(int lenRow, int lenColumn, NType value);
    virtual NType activation(NType& x) = 0;
    virtual NType derivative(NType& y) = 0;
    virtual NArray<NType>* run(NArray<NType>* X);
    virtual NType funcRegularization();
};

#endif // INLAYER_H
