#ifndef NEUROPROJECT_H
#define NEUROPROJECT_H

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
enum class NTrainType {NTrainNone, NTrainBPType, NAgentType}; //Алгоритмы обучения
enum class NOptimizerType {NOptimizerNone, NOptimizerSGD, NOptimizerAdam}; //Алгоритмы оптимизации

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

//Команды для нейросети
enum class NCommandNet
{
    NComNone, //0 - Пустая команда
    NComRunTrain, //1 - Запустить обучение (send)
    NComRunFunc, //2 - Запустить функционирование (send)
    NComUpdateOut, //3 - Обновились выходные данные (inbox)
    NComBeg, //4 - Начало работы NPT (inbox)
    NComEnd, //5 - Окончание работы NPT (inbox)
    NComFinish, //6 - Завершить работу NPT (send)
    NComBegExtern, //7 - Начало работы внешней программы (send)
    NComEndExtern, //8 - Окончание работы внешней программы (send)
    NComEndTrain, //9 - Окончание обучения (inbox)
    NComNCSave, //10 - Сохранить данные комитета (send)
    NComIsSave //11 - Сохранение данных произведено (inbox)
};

//Статусы нейросети
enum class NStatusNet
{
    NStatusNone, //0 - Нет статуса
    NStatusCreate, //1 - Нейросеть создана
    NStatusInit, //2 - Нейросеть инициализирована
    NStatusTrain //3 - Нейросеть обучена
};

//Статусы параллельных заданий
enum class NStatusTask
{
    NStatusNone, //0 - Задание не выполнено
    NStatusRun, //1 - Задание в процессе выполнения
    NStatusError, //2 - Задание выполнено с ошибкой
    NStatusSuccess //3 - Задание успешно выполнено
};

//typedef float (*TFuncActivation)(float& x, float& koef);

extern default_random_engine NRandGenerator;


void init_random_generator();
float deltatime(clock_t start_time, clock_t end_time);
void time_srand();

void to_value(NLayerType& value, string str_val);
void to_value(NLayerDerivat& value, string str_val);
void to_value(NInitType& value, string str_val);
void to_value(NSetType& value, string str_val);
void to_value(NTargetFunc& value, string str_val);
void to_value(NNormalizationFunc& value, string str_val);
void to_value(NCommandNet& value, string str_val);
void to_value(NTrainType& value, string str_val);
void to_value(NOptimizerType& value, string str_val);

string to_string(const NLayerType& value);
string to_string(const NLayerDerivat& value);
string to_string(const NInitType& value);
string to_string(const NSetType& value);
string to_string(const NTargetFunc& value);
string to_string(const NNormalizationFunc& value);
string to_string(const NCommandNet& value);
string to_string(const NStatusNet& value);
string to_string(const NTrainType& value);
string to_string(const NOptimizerType& value);

bool equalf(float A, float B, int maxUlps);

template <typename NType>
inline bool isEqual(const NType& v1)
{
    if(v1 >= 0) {return v1 <= NFUNC_EPSILON;}
    else {return -v1 <= NFUNC_EPSILON;}
}

template <typename NType>
inline bool isEqual(const NType& v1, const NType& v2)
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
NType funcTanh(const NType& x, const NType& koef)
{
    return NFUNC_TANH(koef * x);
}

template <typename NType>
NType funcSoftsign(const NType& x, const NType& koef)
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
NType funcArctg(const NType& x, const NType& koef)
{
    return NFUNC_ATAN(koef * x);
}

template <typename NType>
NType funcLinear(const NType& x, const NType& koef)
{
    return koef * x;
}

template <typename NType>
NType funcStep(const NType& x, const NType& koef)
{
    return (x < koef ? 0 : 1);
}

template <typename NType>
NType funcSoftStep(const NType& x, const NType& koef)
{
    return (NFUNC_TANH(koef * x) + 1) / 2;
}

template <typename NType>
NType funcReLU(const NType& x, const NType& /*koef*/)
{
    return (x < 0 ? 0 : x);
}

template <typename NType>
NType funcPReLU(const NType& x, const NType& koef)
{
    return (x < 0 ? (koef * x) : x);
}

template <typename NType>
NType funcELU(const NType& x, const NType& koef)
{
    return (x < 0 ? koef * (NFUNC_EXP(x) - 1) : x);
}

template <typename NType>
NType funcSoftPlus(const NType& x, const NType& koef)
{
    return NFUNC_LN(1 + NFUNC_EXP(koef * x));
}

template <typename NType>
NType funcSin(const NType& x, const NType& koef)
{
    return NFUNC_SIN(koef * x);
}

template <typename NType>
NType funcSinc(const NType& x,const  NType& koef)
{
    NType aval = koef * x;
    return (isEqual(x) ? 1 : NFUNC_SIN(aval) / aval);
}

template <typename NType>
NType funcGaussian(const NType& x, const NType& koef)
{
    NType aval = koef * x;
    return NFUNC_EXP(-NFUNC_SQR(aval));
}

template <typename NType>
NType funcLinearInt(const NType& x, const NType& koef)
{
    return static_cast<int>(koef * x)/koef;
}

template <typename NType>
NType funcSoftMax(const NType& x, const NType& koef)
{
    return koef * x;
}


//Производные функций активации
template <typename NType>
NType derivTanh(const NType& /*sum*/, const NType& y, const NType& koef)
{
    return (koef * (1 - NFUNC_SQR(y)));
}

template <typename NType>
NType derivSoftsign(const NType& /*sum*/, const NType& y, const NType& koef)
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
NType derivArctg(const NType& sum, const NType& /*y*/, const NType& koef)
{
    NType aval = koef * sum;
    return koef / (NFUNC_SQR(aval) + 1);
}

template <typename NType>
NType derivLinear(const NType& /*sum*/, const NType& /*y*/, const NType& koef)
{
    return koef;
}

template <typename NType>
NType derivStep(const NType& sum, const NType& /*y*/, const NType& koef)
{
    return (isEqual(sum) ? koef : 0);
}

template <typename NType>
NType derivSoftStep(const NType& /*sum*/, const NType& y, const NType& koef)
{
    return (koef * y * (1 - y));
}

template <typename NType>
NType derivReLU(const NType& sum, const NType& /*y*/, const NType& /*koef*/)
{
    return (sum < 0 ? 0 : 1);
}

template <typename NType>
NType derivPReLU(const NType& sum, const NType& /*y*/, const NType& koef)
{
    return (sum < 0 ? koef : 1);
}

template <typename NType>
NType derivELU(const NType& /*sum*/, const NType& y, const NType& koef)
{
    return (y < 0 ? koef + y : 1);
}

template <typename NType>
NType derivSoftPlus(const NType& sum, const NType& /*y*/, const NType& koef)
{
    return (koef * (NFUNC_TANH(koef * sum / 2) + 1) / 2);
}

template <typename NType>
NType derivSin(const NType& sum, const NType& /*y*/, const NType& koef)
{
    return koef * NFUNC_COS(koef * sum);
}

template <typename NType>
NType derivSinc(const NType& sum, const NType& y, const NType& koef)
{
    NType aval = koef * sum;
    return (isEqual(sum) ? 0 : koef * (y / NFUNC_TAN(aval) - y / aval));
}

template <typename NType>
NType derivGaussian(const NType& sum, const NType& /*y*/, const NType& koef)
{
    NType aval = koef * sum;
    return (-2 * koef * aval * NFUNC_EXP(-NFUNC_SQR(aval)));
}

template <typename NType>
NType derivLinearInt(const NType& /*sum*/, const NType& /*y*/, const NType& /*koef*/)
{
    return 1;
}

template <typename NType>
NType derivSoftMax(const NType& /*sum*/, const NType& /*y*/, const NType& koef)
{
    return koef;
}


//Обратные функции активации
template <typename NType>
NType reversLinear(const NType& y, const NType& koef)
{
    return y / koef;
}

template <typename NType>
NType reversArctg(const NType& y, const NType& koef)
{
    return NFUNC_TAN(y) / koef;
}

template <typename NType>
NType reversSoftStep(const NType& y, const NType& koef)
{
    return NFUNC_LN(y / (1 - y)) / (2 * koef);
}

template <typename NType>
NType reversGaussian(const NType& y, const NType& koef)
{
    return NFUNC_SQRT(-NFUNC_LN(y)) / koef;
}


//Функции активации слоя
template <typename NType>
void funcLaySoftMax(NArray<NType>& out)
{
    NType val = -out.maxElements();
    out.valsum(val);
    for(int i = 0; i < out.getLength(); i++)
    {
        out[i] = NFUNC_EXP(out[i]);
    }
    val = out.sumElements();
    out.valdiv(val);
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
            derivout[i] = outrun[i];
        }
        else if(isEqual(outrun[i]))
        {
            derivout[i] = 1.0;
        }
        else
        {
            derivout[i] = outrun[i] * (1.0 - outrun[i]/output[i]);
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
NType targetMSEAver(const NType& enrgSum, int lenExm)
{
    return (lenExm == 0 ? 0 : NFUNC_SQRT(2 * enrgSum / lenExm));
}

template <typename NType>
NType targetMSEMax(const NType& enrgMax)
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
                enrg -= output[i] * NFUNC_LN(outrun[i]);
            }
            else
            {
                return -NFUNC_LIMITS_MAX;
            }
        }
    }
    return enrg;
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
