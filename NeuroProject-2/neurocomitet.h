#ifndef NEUROCOMITET_H
#define NEUROCOMITET_H

#include "ineuronet.h"
#include "trainbp.h"

template <typename NType>
class NeuroComitet: public IObjectECSV
{
public:
    NeuroComitet();
public:
    NArray<INeuroNet<NType>*> nets; //Массив нейросетей
    TrainBP<NType>* train; //Алгоритм обучения
    NExamples<NType>* zad; //Данные по задаче
public:
    void deinit();
public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NeuroComitet<NType>::NeuroComitet()
{
    this->train = nullptr;
    this->zad = nullptr;
}

template <typename NType>
void NeuroComitet<NType>::deinit()
{
    INeuroNet<NType>* net;
    int len = this->nets.getLength();
    for(int i = 0; i < len; i++)
    {
        net = this->nets.pop();
        delete net;
    }

    this->train = nullptr;
    this->zad = nullptr;
}

template <typename NType>
void NeuroComitet<NType>::saveECSV(DataECSV& dt, string& parent)
{
    string fieldTrain = parent + ".train";
    string fieldNet = parent + ".net";

    dt.addGroup(parent, "");
    this->train->saveECSV(dt, fieldTrain);

    for(int i = 0; i < this->nets.getLength(); i++)
    {
        nets[i]->saveECSV(dt, fieldNet);
    }
}

template <typename NType>
void NeuroComitet<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    INeuroNet<NType>* net;
    string fieldTrain = parent + ".train";
    string fieldNet = parent + ".net";

    this->deinit();

    size_t ind = dt.getShift();
    iter = dt.modules[ind];
    if(iter->cmpPath(parent, true)) {ind++;}

    while(dt.isNextBlock(ind, parent))
    {
        iter = dt.modules[ind];

        if(iter->cmpPath(fieldTrain, true))
        {
            dt.setShift(ind);
            this->train = new TrainBP<NType>();
            this->train->loadECSV(dt, fieldTrain);
            ind = dt.getShift();
        }
        else if(iter->cmpPath(fieldNet, true))
        {
            dt.setShift(ind);
            net = new INeuroNet<NType>;
            net->loadECSV(dt, fieldNet);
            this->nets.push(net);
            ind = dt.getShift();
        }
        else
        {
            ind++;
        }
    }

    dt.setShift(ind);
}

#endif // NEUROCOMITET_H
