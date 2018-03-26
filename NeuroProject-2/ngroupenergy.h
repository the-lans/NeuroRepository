#ifndef NGROUPENERGY_H
#define NGROUPENERGY_H

template <typename NType>
struct NGroupEnergy
{
    NType energyAver; //Средняя энергия примеров
    NType energyMax; //Максимальная энергия примеров
    NType energySum; //Сумма энергий примеров
    int energyClass; //Количество ошибочно определённых классов
    int totalClass; //Общее количество примеров определённых классов

    NGroupEnergy();
    void clear();
};

template <typename NType>
NGroupEnergy<NType>::NGroupEnergy()
{
    this->clear();
}

template <typename NType>
void NGroupEnergy<NType>::clear()
{
    energyAver = 0;
    energyMax = 0;
    energySum = 0;
    energyClass = 0;
    totalClass = 0;
}

#endif // NGROUPENERGY_H
