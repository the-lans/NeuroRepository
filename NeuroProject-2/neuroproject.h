#ifndef NEUROPROJECT_H
#define NEUROPROJECT_H

#include <math.h>
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <limits>
#include <random>
#include "funcecsv.h"

using namespace std;

//Виды функций активации
enum class NLayerType
{
    NFuncNone, //0 - Нет функции активации
    NFuncTanh, //1 - Гиперболический тангенс
    NFuncSoftsign, //2 - SoftSign
    NFuncArctg, //3 - Арктангенс
    NFuncLinear, //4 - Линейная
    NFuncStep, //5 - Пороговая
    NFuncSoftStep, //6 - Логистическая
    NFuncReLU, //7 - Rectified linear unit (ReLU)
    NFuncPReLU, //8 - Parameteric rectified linear unit (PReLU)
    NFuncELU, //9 - Exponential linear unit (ELU)
    NFuncSoftPlus, //10 - SoftPlus
    NFuncSin, //11 - Синус
    NFuncSinc, //12 - Sinc
    NFuncGaussian, //13 - Гаусс
    NFuncLinearInt, //14 - Линейная с целой частью
    NFuncSoftMax //15 - SoftMax
};

//Виды целевых функций
enum class NTargetFunc
{
    NTargetNone, //0 - Нет целевой функции
    NTargetMSE,  //1 - Квадратичная функция
    NTargetCross //2 - Кросс-энтропия
};

//Виды функций для нормализации значений
enum class NNormalizationFunc
{
    NNormNone, //0 - Нет нормализации
    NNormLinear, //1 - Линейная нормализация
    NNormSoftStep, //2 - SoftStep
    NNormGaussian, //3 - Gaussian
    NNormArctg //4 - Arctg
};

enum class NLayerDerivat {NDerivatNone, NDerivatSum, NDerivatOut}; //Аргумент подсчёта производной функции активации
enum class NInitType {NInitNone, NInitEmpty, NInitG}; //Тип инициализации слоя->нейросети
enum class NTypeEnum {NTypeFloat, NTypeDouble, NTypeInt}; //Разрешённые типы для нейросетей
enum class NSetType {NSetNone, NSetTrain, NSetTest, NSetValidate}; //Множество примеров

//Ошибки нейросети
enum NError
{
    NErrorNone, //0 - Нет ошибок
    NErrorOther, //1 - Прочие ошибки
    NErrorStruct, //2 - Размерность weight соответствует структуре нейросети
    NErrorBias, //3 - Размерность bias соответствует размерности weight
    NErrorCrossExm, //4 - Множества примеров не пересекаются
    NErrorExmEmpty, //5 - Не должно быть пустых примеров
    NErrorLay, //6 - Количество слоёв lay
    NErrorTarget //7 - Используемая целевая функция
};

//typedef float (*TFuncActivation)(float& x, float& koef);

#define NFUNC_SQR(arg) ((arg)*(arg))
#define NFUNC_SQRT(arg) sqrtf(arg)
#define NFUNC_EXP(arg) expf(arg)
#define NFUNC_EXP87 6.07603E37
#define NFUNC_LN(arg) logf(arg)
#define NFUNC_SIN(arg) sinf(arg)
#define NFUNC_COS(arg) cosf(arg)
#define NFUNC_TAN(arg) tanf(arg)
#define NFUNC_ATAN(arg) atanf(arg)
#define NFUNC_TANH(arg) tanhf(arg)
#define NFUNC_EPSILON std::numeric_limits<NType>::epsilon()
#define NFUNC_LIMITS_MAX std::numeric_limits<NType>::max()

extern default_random_engine NRandGenerator;


void init_random_generator();
float deltatime(unsigned int start_time, unsigned int end_time);
void time_srand();

void to_value(NLayerType& value, string& str_val);
void to_value(NLayerDerivat& value, string& str_val);
void to_value(NInitType& value, string& str_val);
void to_value(NSetType& value, string& str_val);
void to_value(NTargetFunc& value, string& str_val);
void to_value(NNormalizationFunc& value, string& str_val);

string to_string(NLayerType& value);
string to_string(NLayerDerivat& value);
string to_string(NInitType& value);
string to_string(NSetType& value);
string to_string(NTargetFunc& value);
string to_string(NNormalizationFunc& value);

bool equalf(float A, float B, int maxUlps);

template <typename NType>
bool isEqual(const NType& v1)
{
    if(v1 >= 0) {return v1 <= NFUNC_EPSILON;}
    else {return -v1 <= NFUNC_EPSILON;}
}

template <typename NType>
bool isEqual(const NType& v1, const NType& v2)
{
    NType eql = v1 - v2;
    if(eql >= 0) {return eql <= NFUNC_EPSILON;}
    else {return -eql <= NFUNC_EPSILON;}
}

template <typename NType>
inline bool interval(NType& value1, NType& value2, NType& delta)
{
    return ((value2 >= value1 - delta) && (value2 <= value1 + delta));
}


//Функции активации нейрона
template <typename NType>
NType funcTanh(NType& x, NType& koef)
{
    return NFUNC_TANH(koef * x);
}

template <typename NType>
NType funcSoftsign(NType& x, NType& koef)
{
    if(x >= 0)
    {
        return x/(1/koef + x);
    }
    else
    {
        return x/(1/koef - x);
    }
}

template <typename NType>
NType funcArctg(NType& x, NType& koef)
{
    return NFUNC_ATAN(koef * x);
}

template <typename NType>
NType funcLinear(NType& x, NType& koef)
{
    return koef * x;
}

template <typename NType>
NType funcStep(NType& x, NType& koef)
{
    return (x < koef ? 0 : 1);
}

template <typename NType>
NType funcSoftStep(NType& x, NType& koef)
{
    return (NFUNC_TANH(koef * x) + 1) / 2;
}

template <typename NType>
NType funcReLU(NType& x, NType& koef)
{
    return (x < 0 ? 0 : x);
}

template <typename NType>
NType funcPReLU(NType& x, NType& koef)
{
    return (x < 0 ? (koef * x) : x);
}

template <typename NType>
NType funcELU(NType& x, NType& koef)
{
    return (x < 0 ? koef * (NFUNC_EXP(x) - 1) : x);
}

template <typename NType>
NType funcSoftPlus(NType& x, NType& koef)
{
    return NFUNC_LN(1 + NFUNC_EXP(koef * x));
}

template <typename NType>
NType funcSin(NType& x, NType& koef)
{
    return NFUNC_SIN(koef * x);
}

template <typename NType>
NType funcSinc(NType& x, NType& koef)
{
    NType aval = koef * x;
    return (isEqual(x) ? 1 : NFUNC_SIN(aval) / aval);
}

template <typename NType>
NType funcGaussian(NType& x, NType& koef)
{
    NType aval = koef * x;
    return NFUNC_EXP(-NFUNC_SQR(aval));
}

template <typename NType>
NType funcLinearInt(NType& x, NType& koef)
{
    return ((int)(koef * x))/koef;
}

template <typename NType>
NType funcSoftMax(NType& x, NType& koef)
{
    NType val = koef * x;
    if(val > 87.0) {return NFUNC_EXP87;}
    else {return NFUNC_EXP(val);}
}


//Производные функций активации
template <typename NType>
NType derivTanh(NType& sum, NType& y, NType& koef)
{
    return (koef * (1 - NFUNC_SQR(y)));
}

template <typename NType>
NType derivSoftsign(NType& sum, NType& y, NType& koef)
{
    if(y >= 0)
    {
        NType aval = y - 1;
        return koef * NFUNC_SQR(aval);
    }
    else
    {
        NType aval = y + 1;
        return koef * NFUNC_SQR(aval);
    }
}

template <typename NType>
NType derivArctg(NType& sum, NType& y, NType& koef)
{
    NType aval = koef * sum;
    return koef / (NFUNC_SQR(aval) + 1);
}

template <typename NType>
NType derivLinear(NType& sum, NType& y, NType& koef)
{
    return koef;
}

template <typename NType>
NType derivStep(NType& sum, NType& y, NType& koef)
{
    return (isEqual(sum) ? koef : 0);
}

template <typename NType>
NType derivSoftStep(NType& sum, NType& y, NType& koef)
{
    return (koef * y * (1 - y));
}

template <typename NType>
NType derivReLU(NType& sum, NType& y, NType& koef)
{
    return (sum < 0 ? 0 : 1);
}

template <typename NType>
NType derivPReLU(NType& sum, NType& y, NType& koef)
{
    return (sum < 0 ? koef : 1);
}

template <typename NType>
NType derivELU(NType& sum, NType& y, NType& koef)
{
    return (y < 0 ? koef + y : 1);
}

template <typename NType>
NType derivSoftPlus(NType& sum, NType& y, NType& koef)
{
    return (koef * (NFUNC_TANH(koef * sum / 2) + 1) / 2);
}

template <typename NType>
NType derivSin(NType& sum, NType& y, NType& koef)
{
    return koef * NFUNC_COS(koef * sum);
}

template <typename NType>
NType derivSinc(NType& sum, NType& y, NType& koef)
{
    NType aval = koef * sum;
    return (isEqual(sum) ? 0 : koef * (y / NFUNC_TAN(aval) - y / aval));
}

template <typename NType>
NType derivGaussian(NType& sum, NType& y, NType& koef)
{
    NType aval = koef * sum;
    return (-2 * koef * aval * NFUNC_EXP(-NFUNC_SQR(aval)));
}

template <typename NType>
NType derivLinearInt(NType& sum, NType& y, NType& koef)
{
    return 1;
}

template <typename NType>
NType derivSoftMax(NType& sum, NType& y, NType& koef)
{
    if(isEqual(y))
    {
        return koef;
    }
    else
    {
        return koef * y;
    }
    //return (isEqual(y) ? koef : koef * y);
}


//Обратные функции активации
template <typename NType>
NType reversLinear(NType& y, NType& koef)
{
    return y / koef;
}

template <typename NType>
NType reversArctg(NType& y, NType& koef)
{
    return NFUNC_TAN(y) / koef;
}

template <typename NType>
NType reversSoftStep(NType& y, NType& koef)
{
    return NFUNC_LN(y / (1 - y)) / (2 * koef);
}

template <typename NType>
NType reversGaussian(NType& y, NType& koef)
{
    return NFUNC_SQRT(-NFUNC_LN(y)) / koef;
}


//Функции активации слоя
template <typename NType>
void funcLaySoftMax(NArray<NType>& out)
{
    NType sum = out.sumElements();
    if(!isEqual(sum))
    {
        out.valdiv(sum);
    }
}


//Производные функции активации слоя
template <typename NType>
void derivLaySoftMax(NArray<NType>& derivout, NArray<NType>& output, NArray<NType>& outrun)
{
    for(int i = 0; i < outrun.getLength(); i++)
    {
        //derivout[i] = (isEqual(output[i]) || isEqual(outrun[i]) ? 1.0 : 1.0 - outrun[i] / output[i]);
        if(isEqual(output[i]))
        {
            derivout[i] = 1.0;
        }
        else if(isEqual(outrun[i]))
        {
            derivout[i] = 1.0;
        }
        else
        {
            derivout[i] = 1.0 - outrun[i]/output[i];
        }
    }
}


//Целевые функции
template <typename NType>
NType targetMSE(NType* output, NType* outrun, int count)
{
    NType enrg = 0;
    for(int i = 0; i < count; i++)
    {
        enrg += NFUNC_SQR(output[i] - outrun[i]);
    }
    return (enrg / 2);
}

template <typename NType>
NType targetMSEAver(NType& enrgSum, int& lenExm)
{
    return (lenExm == 0 ? 0 : NFUNC_SQRT(2 * enrgSum / lenExm));
}

template <typename NType>
NType targetMSEMax(NType& enrgMax)
{
    return NFUNC_SQRT(2 * enrgMax);
}

template <typename NType>
NType targetCross(NType* output, NType* outrun, int count)
{
    NType enrg = 0;
    for(int i = 0; i < count; i++)
    {
        if(output[i] > 0)
        {
            if(outrun[i] > 0)
            {
                enrg += output[i] * NFUNC_LN(outrun[i]);
            }
            else
            {
                return NFUNC_LIMITS_MAX;
            }
        }
    }
    return -enrg;
}


//Производные целевых функций
template <typename NType>
void derivTargetMSE(NArray<NType>& derivEnrg, NType* output, NType* outrun, int count)
{
    for(int i = 0; i < count; i++)
    {
        derivEnrg[i] = output[i] - outrun[i];
    }
}

template <typename NType>
void derivTargetCross(NArray<NType>& derivEnrg, NType* output, NType* outrun, int count)
{
    for(int i = 0; i < count; i++)
    {
        if(isEqual(output[i]))
        {
            derivEnrg[i] = -1.0;
        }
        else if(isEqual(outrun[i]))
        {
            derivEnrg[i] = output[i];
        }
        else
        {
            derivEnrg[i] = output[i]/outrun[i];
        }
    }
}

#endif // NEUROPROJECT_H
