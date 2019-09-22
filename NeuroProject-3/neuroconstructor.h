#ifndef NEUROCONSTRUCTOR_H
#define NEUROCONSTRUCTOR_H

#include "narray.h"
#include "nmatrix.h"
#include "noperators.h"

#include "dataecsv.h"
#include "nmatrixecsv.h"

#include "ineuronet.h"
#include "trainbp.h"
#include "nagent.h"
#include "nfilesconstruct.h"
#include "smarttimer.h"


template <typename NType>
class NeuroConstructor
{
public:
    NExamples<NType>* zad;
    INeuroNet<NType>* net;
    TrainBP<NType>* bp;
    NTrainType train_type; //Тип алгоритма
    NFilesConstruct fin; //Входные данные
    NFilesConstruct fout; //Выходные данные
    string name_outdata; //Имя выходного файла с данными
    string name_input; //Имя входного файла с данными
    float search_time; //Искомое время выполнения

protected:
    DataECSV dt; //Основные данные
    DataECSV dt_csv; //Дополнительные данные (файлы формата CSV)
    SmartTimer timer; //Таймер

public:
    NeuroConstructor();
    ~NeuroConstructor();

public:
    void setCurrentDir(const string& dir);
    DataECSV& getDT();
    DataECSV& getDT_CSV();
    void finit(); //Инициализация файлов
    TrainBP<NType>* createTrain(); //Создание алгоритма обучения
    void init_zad(); //Инициализация задачи
    void init_net(bool isTrain); //Конструктор нейросети
    void init_bp(bool isTrain); //Параметры обучения
    void init_constructor(bool isTrain); //Инициализация комитета
    void load_zad(); //Загрузка задачи
    void load_net(bool isTrain); //Загрузка нейросети
    void load_bp(bool isTrain); //Загрузка обучения
    void save_zad(); //Сохранение задачи
    void save_net(); //Сохранение нейросети
    void save_bp(); //Сохранение обучения
    void outdata_net_saveECSV(string& parent); //Сохранение выходных данных нейросети
    void outdata_zad_saveECSV(string& parent); //Сохранение выходных данных по задаче
    void outdata_bp_saveECSV(string& parent); //Сохранение выходных данных по обучению
    void outdata_save(); //Сохранение выходных данных по комитету в один файл
    void load(bool isTrain); //Загрузка данных из соответствующих файлов
    void save(bool isTrain); //Сохранение данных в соответствующие файлы
    void deinit_zad(bool isSave); //Деинициализация задачи
    void deinit_net(bool isSave); //Деинициализация нейросети
    void deinit_bp(bool isSave); //Деинициализация обучения
    void deinit_constructor(bool isTrain); //Деинициализация комитета
    void train(); //Выполнение обучения
};


template <typename NType>
NeuroConstructor<NType>::NeuroConstructor()
{
    this->net = nullptr;
    this->zad = nullptr;
    this->bp = nullptr;
    this->train_type = NTrainType::NTrainNone;
    this->name_outdata = "";
    this->name_input = "";
    this->dt.setArrayBR(true);
}

template <typename NType>
NeuroConstructor<NType>::~NeuroConstructor()
{
    if(this->zad != nullptr) {delete this->zad; this->zad = nullptr;}
    if(this->net != nullptr) {delete this->net; this->net = nullptr;}
    if(this->bp != nullptr) {delete this->bp; this->bp = nullptr;}
}

template <typename NType>
void NeuroConstructor<NType>::setCurrentDir(const string& dir)
{
    this->fin.current_dir = dir;
    this->fout.current_dir = dir;
}

template <typename NType>
DataECSV& NeuroConstructor<NType>::getDT()
{
    return this->dt;
}

template <typename NType>
DataECSV& NeuroConstructor<NType>::getDT_CSV()
{
    return this->dt_csv;
}

template <typename NType>
void NeuroConstructor<NType>::finit()
{
    this->fin.init();
    this->fout.init();
}

template <typename NType>
TrainBP<NType>* NeuroConstructor<NType>::createTrain()
{
         if(this->train_type == NTrainType::NTrainBPType) {this->bp = new TrainBP<NType>;}
    else if(this->train_type == NTrainType::NAgentType) {this->bp = new NAgent<NType>;}
    else {this->bp = nullptr;}
    return this->bp;
}

template <typename NType>
void NeuroConstructor<NType>::init_zad()
{
    if(this->fin.current_dir == "" && this->fout.current_dir == "") {return;}
    if(this->zad == nullptr) {this->zad = new NExamples<NType>;}
    this->load_zad();
}

template <typename NType>
void NeuroConstructor<NType>::init_net(bool isTrain)
{
    if(this->fin.current_dir == "" && this->fout.current_dir == "") {return;}
    if(this->net == nullptr) {this->net = new INeuroNet<NType>;}
    this->net->examples = this->zad;
    this->net->tmp_examples = this->zad;
    this->load_net(isTrain);
}

template <typename NType>
void NeuroConstructor<NType>::init_bp(bool isTrain)
{
    if(this->fin.current_dir == "" && this->fout.current_dir == "") {return;}
    if(isTrain && this->bp == nullptr) {this->createTrain();}
    if(isTrain) {this->bp->net = this->net;}
    this->load_bp(isTrain);
}

template <typename NType>
void NeuroConstructor<NType>::init_constructor(bool isTrain)
{
    this->finit();
    if(this->fin.current_dir == "") {return;}

    this->init_zad();
    this->init_net(isTrain);
    this->init_bp(isTrain);

    dt.clear();
}

template <typename NType>
void NeuroConstructor<NType>::load_zad()
{
    bool isLoad;
    string parent = "";

    dt.clear();
    if(this->fin.current_dir == "" && this->fout.current_dir == "") {return;}

    this->fin.isFileExamples = dt.read(this->fin.current_dir + "\\" + this->fin.name_examples);
    if(this->fin.isFileExamples)
    {
        this->zad->loadECSV(dt, parent);
    }
    else
    {
        this->fout.isFileExamples = dt.read(this->fout.current_dir + "\\" + this->fout.name_examples);
        if(this->fout.isFileExamples) {this->zad->loadECSV(dt, parent);}
    }

    // Примеры
    dt_csv.clear();
    //cout << this->fin.current_dir + "\\" + this->name_input << endl;
    isLoad = dt_csv.read(this->fin.current_dir + "\\" + this->name_input);
    if(isLoad) {this->zad->loadECSV(dt_csv, parent);}
}

template <typename NType>
void NeuroConstructor<NType>::load_net(bool isTrain)
{
    string parent = "";
    dt.clear();
    if(this->fin.current_dir == "" && this->fout.current_dir == "") {return;}

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
}

template <typename NType>
void NeuroConstructor<NType>::load_bp(bool isTrain)
{
    string parent = "";
    dt.clear();
    if(this->fin.current_dir == "" && this->fout.current_dir == "") {return;}

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

        if(!this->fin.isFileNeuronet) {return;}
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
}

template <typename NType>
void NeuroConstructor<NType>::save_zad()
{
    string parent = "";
    dt.clear();
    this->zad->saveECSV(dt, parent);
    this->fout.isFileExamples = dt.write(this->fout.current_dir + "\\" + this->fout.name_examples);
}

template <typename NType>
void NeuroConstructor<NType>::save_net()
{
    string parent = "";
    dt.clear();
    this->net->saveECSV(dt, parent);
    this->fout.isFileNeuronet = dt.write(this->fout.current_dir + "\\" + this->fout.name_neuronet);
}

template <typename NType>
void NeuroConstructor<NType>::save_bp()
{
    string parent = "";
    dt.clear();
    this->bp->saveECSV(dt, parent);
    this->fout.isFileTrainBP = dt.write(this->fout.current_dir + "\\" + this->fout.name_trainbp);
}

template <typename NType>
void NeuroConstructor<NType>::outdata_net_saveECSV(string& parent)
{
    this->dt.addGroup(parent, "");
}

template <typename NType>
void NeuroConstructor<NType>::outdata_zad_saveECSV(string& parent)
{
    this->zad->outdata_saveECSV(this->dt, parent);
}

template <typename NType>
void NeuroConstructor<NType>::outdata_bp_saveECSV(string& parent)
{
    this->bp->outdata_saveECSV(this->dt, parent);
}

template <typename NType>
void NeuroConstructor<NType>::outdata_save()
{
    string parent = "";
    dt.clear();
    this->outdata_zad_saveECSV(parent);
    this->outdata_bp_saveECSV(parent);
    dt.write(this->fout.current_dir + "\\" + this->name_outdata);
}

template <typename NType>
void NeuroConstructor<NType>::load(bool isTrain)
{
    this->finit();
    if(this->fin.current_dir == "") {return;}
    this->load_zad();
    this->load_net(isTrain);
    this->load_bp(isTrain);
    dt.clear();
}

template <typename NType>
void NeuroConstructor<NType>::save(bool isTrain)
{
    if(this->fout.current_dir == "") {return;}
    if(!isTrain || this->bp->getCountTime() == 1) {this->save_zad();}
    if(!isTrain || this->bp->getCountTime() == 1) {this->save_net();}
    if(isTrain && this->bp->getCountTime() == 1) {this->save_bp();}
    dt.clear();
}

template <typename NType>
void NeuroConstructor<NType>::deinit_zad(bool isSave)
{
    if(isSave) {this->save_zad();}
    if(this->zad != nullptr) {delete this->zad; this->zad = nullptr;}
}

template <typename NType>
void NeuroConstructor<NType>::deinit_net(bool isSave)
{
    if(isSave) {this->save_net();}
    if(this->net != nullptr) {delete this->net; this->net = nullptr;}
}

template <typename NType>
void NeuroConstructor<NType>::deinit_bp(bool isSave)
{
    if(isSave) {this->save_bp();}
    if(this->bp != nullptr) {delete this->bp; this->bp = nullptr;}
    this->train_type = NTrainType::NTrainNone;
}

template <typename NType>
void NeuroConstructor<NType>::deinit_constructor(bool isTrain)
{
    if(this->fout.current_dir == "") {return;}
    this->deinit_zad(!isTrain || this->bp->getCountTime() == 1);
    this->deinit_net(!isTrain || this->bp->getCountTime() == 1);
    this->deinit_bp(isTrain && this->bp->getCountTime() == 1);
    dt.clear();
}

template <typename NType>
void NeuroConstructor<NType>::train()
{
    this->timer.start(); //Начальное время

    //Процесс обучения
    if(this->bp->getCountTime() > 1)
    {
        this->bp->trainTime();
    }
    else
    {
        this->bp->pretrain();
        this->bp->train();
        this->bp->posttrain();
    }

    this->timer.stop(); //Конечное время
    search_time = this->timer.deltasec(); //Искомое время
}

#endif // NEUROCONSTRUCTOR_H
