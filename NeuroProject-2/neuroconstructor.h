#ifndef NEUROCONSTRUCTOR_H
#define NEUROCONSTRUCTOR_H

#include "narray.h"
#include "nmatrix.h"
#include "noperators.h"

#include "dataecsv.h"
#include "nmatrixecsv.h"

#include "ineuronet.h"
#include "trainbp.h"
#include "neurocomitet.h"
#include "nfilesconstruct.h"


template <typename NType>
class NeuroConstructor
{
public:
    NExamples<NType>* zad;
    INeuroNet<NType>* net;
    TrainBP<NType>* bp;
    NFilesConstruct fin; //Входные данные
    NFilesConstruct fout; //Выходные данные
    float search_time;

protected:
    DataECSV dt;
    unsigned int start_time;
    unsigned int end_time;

public:
    NeuroConstructor();
    ~NeuroConstructor();

    void init_constructor(bool isTrain);
    void deinit_constructor(bool isTrain);
    void train();
};


template <typename NType>
NeuroConstructor<NType>::NeuroConstructor()
{
    this->net = nullptr;
    this->zad = nullptr;
    this->bp = nullptr;
    this->dt.setArrayBR(true);
}

template <typename NType>
NeuroConstructor<NType>::~NeuroConstructor()
{
    if(this->zad != nullptr) {delete this->zad;} this->zad = nullptr;
    if(this->net != nullptr) {delete this->net;} this->net = nullptr;
    if(this->bp != nullptr) {delete this->bp;} this->bp = nullptr;
}

template <typename NType>
void NeuroConstructor<NType>::init_constructor(bool isTrain)
{
    string parent = "";

    this->fin.init();
    this->fout.init();
    if(this->fin.current_dir == "") {return;}

    // Задача
    if(this->zad == nullptr) {this->zad = new NExamples<NType>;}
    dt.clear();
    this->fin.isFileExamples = dt.read(this->fin.current_dir + "\\" + this->fin.name_examples);
    if(this->fin.isFileExamples) {this->zad->loadECSV(dt, parent);}

    if(!this->fin.isFileExamples)
    {
        this->fout.isFileExamples = dt.read(this->fout.current_dir + "\\" + this->fout.name_examples);
        if(this->fout.isFileExamples) {this->zad->loadECSV(dt, parent);}
    }

    // Конструктор нейросети
    if(this->net == nullptr) {this->net = new INeuroNet<NType>;}
    this->net->examples = zad;

    dt.clear();
    if(!isTrain)
    {
        this->fout.isFileNeuronet = dt.read(this->fout.current_dir + "\\" + this->fout.name_neuronet);
        if(this->fout.isFileNeuronet) {this->net->loadECSV(dt, parent);}
    }
    if(isTrain || !this->fout.isFileNeuronet)
    {
        this->fin.isFileNeuronet = dt.read(this->fin.current_dir + "\\" + this->fin.name_neuronet);
        if(this->fin.isFileNeuronet) {this->net->loadECSV(dt, parent);}
    }

    // Параметры обучения
    if(isTrain)
    {
        if(this->net->blLock)
        {
            if(this->net->getTypeInit() == NInitType::NInitEmpty) {this->net->init();}
            else if(this->net->getTypeInit() == NInitType::NInitG) {this->net->ginit();}
        }
        else
        {
            if(this->net->getTypeInit() == NInitType::NInitEmpty) {this->net->init(this->net->lenNeurons, this->net->typeLay);}
            else if(this->net->getTypeInit() == NInitType::NInitG) {this->net->ginit(this->net->lenNeurons, this->net->typeLay);}
        }

        if(this->bp == nullptr) {this->bp = new TrainBP<NType>;}
        this->bp->net = this->net;
        if(!this->fin.isFileNeuronet) {return;}

        dt.clear();
        this->fin.isFileTrainBP = dt.read(this->fin.current_dir + "\\" + this->fin.name_trainbp);
        if(this->fin.isFileTrainBP) {this->bp->loadECSV(dt, parent);}

        this->bp->path = this->fout;
        this->bp->init();
    }
    else
    {
        if(this->net->koefLay.empty())
        {
            this->net->updateKoefLay();
        }
        if(this->net->dropout.empty())
        {
            this->net->updateDropout();
        }
    }

    dt.clear();
}

template <typename NType>
void NeuroConstructor<NType>::deinit_constructor(bool isTrain)
{
    string parent = "";

    // Сохранение
    if(!isTrain || this->bp->getCountTime() == 1)
    {
        dt.clear();
        this->zad->saveECSV(dt, parent);
        this->fout.isFileExamples = dt.write(this->fout.current_dir + "\\" + this->fout.name_examples);

        dt.clear();
        this->net->saveECSV(dt, parent);
        this->fout.isFileNeuronet = dt.write(this->fout.current_dir + "\\" + this->fout.name_neuronet);

        if(isTrain)
        {
            dt.clear();
            this->bp->saveECSV(dt, parent);
            this->fout.isFileTrainBP = dt.write(this->fout.current_dir + "\\" + this->fout.name_trainbp);
        }
    }

    if(this->zad != nullptr) {delete this->zad;} this->zad = nullptr;
    if(this->net != nullptr) {delete this->net;} this->net = nullptr;
    if(this->bp != nullptr) {delete this->bp;} this->bp = nullptr;
    dt.clear();
}

template <typename NType>
void NeuroConstructor<NType>::train()
{
    start_time = clock(); //Начальное время
    //Процесс обучения
    if(this->bp->getCountTime() > 1)
    {
        this->bp->trainTime();
    }
    else
    {
        this->bp->train();
    }
    end_time = clock(); //Конечное время
    search_time = deltatime(start_time, end_time); //Искомое время
    this->net->funcRegularization();
}

#endif // NEUROCONSTRUCTOR_H
