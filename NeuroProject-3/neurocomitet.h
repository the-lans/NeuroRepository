#ifndef NEUROCOMITET_H
#define NEUROCOMITET_H

//#include "ineuronet.h"
//#include "trainbp.h"
#include "neuroconstructor.h"
#include "smarttimer.h"
#include <thread>

using namespace std;

template <typename NType>
class NeuroComitet: public IObjectECSV
{
public:
    NeuroComitet();
    ~NeuroComitet();

public:
    NArray<string> names; //Имена нейросетей
    NArray<INeuroNet<NType>*> nets; //Массив нейросетей
    NArray<TrainBP<NType>*> trains; //Алгоритмы обучения
    NArray<NTrainType> trains_type; //Типы алгоритмов обучения
    NExamples<NType>* zad; //Данные по задаче
    NArray<NExamples<NType>*> tmp_examples; //Массив временных примеров
    NeuroConstructor<NType>* construct; //Конструктор нейросети
    NArray<NError> erCheck; //Проверка нейросетей
    SmartTimer timer; //Измерение времени

protected:
    int count_parallel; //Количество одновременных потоков
    NArray<thread*> nthreads; //Параллельные потоки
    NArray<bool> nthreads_work; //Статусы потоков
    NArray<NStatusTask> task_status; //Статусы заданий
    NArray<int> nthreads_task; //Идентификаторы заданий в потоках
    thread* threadManager; //Менеджер параллельных потоков
    bool threadManager_work; //Статус потока менеджера
    int msec; //Задержка для менеджера потоков
    bool blShiftEnd; //Нужно ли сдвигать выборку в конец?
    bool blOutdataAppend; //Нужно ли дописывать информацию в конец файла outdata?
    bool blNCSave; //Нужно ли сохранять данные после обучения
    string curtime; //Представление даты/времени

public:
    void setCountParallel(int count);
    int getCountParallel();
    void setMsec(unsigned long msec);
    bool getBlShiftEnd();
    bool getBlNCSave();

public:
    int getLength();
    void init_load(const string& path, const string& name_comitet, bool isTrain); //Загрузка и инициализация
    void load_zad(const string& path); //Загрузка задачи
    void nc_load(const string& path, const string& name_comitet, bool isTrain); //Загрузка иерархии комитета
    void nc_save(const string& path, bool isTrain); //Сохранение иерархии комитета
    void updateCurtime(); //Обновление текущего времени
    void outdata_saveECSV(string& parent); //Подготовка отчёта
    void outdata_save(const string& path); //Сохранение отчёта в файл
    bool outdata_create(const string& path); //Создание пустого файла отчёта
    void init_examples(); //Инициализация временных примеров
    void update_examples(); //Обновление временных примеров
    void init_train(); //Инициализация обучения
    void initProccess(); //Инициализация процессов
    void deinit(); //Деинициализация комитета
    void emptyExamples(); //Очистить временные задачи
    bool check(); //Проверки нейросетей
    void runFunc(int indTask, NSetType st, bool post = true); //Функционирование нейросети
    void runFuncName(const string& name, NSetType st, bool post = true);
    void runFunc(NSetType st, bool post = true);
    void runTrain(int indTask, int indParallel); //Обучение нейросети
    void runTrainName(const string& name, int indParallel);

public:
    int thread_train(int indTask); //Запуск параллельного обучения
    int thread_train(const string& name);
    bool thread_train(int indTask, int indParallel);
    bool thread_train(const string& name, int indParallel);
    void thread_train();
    void thread_trainManager(); //Запуск менеджера процессов
    void waitManager(); //Ожидание выполнения менеджера
    void waitProccess(int indParallel); //Ожидание выполнения процесса
    void waitProccess(); //Ожидание всех процессов

public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NeuroComitet<NType>::NeuroComitet()
{
    this->zad = nullptr;
    this->construct = nullptr;
    this->count_parallel = (thread::hardware_concurrency() > 0 ? thread::hardware_concurrency() : 1);
    this->threadManager = nullptr;
    this->threadManager_work = false;
    this->msec = 300;
    this->blShiftEnd = false;
    this->blOutdataAppend = false;
    this->curtime = "";
}

template <typename NType>
NeuroComitet<NType>::~NeuroComitet()
{
    this->deinit();
}

template <typename NType>
void NeuroComitet<NType>::setCountParallel(int count)
{
    this->count_parallel = count;
    this->nthreads.resize(count);
    this->nthreads_work.resize(count);
    this->task_status.resize(count);
    this->nthreads_task.resize(count);
}

template <typename NType>
int NeuroComitet<NType>::getCountParallel()
{
    return this->count_parallel;
}

template <typename NType>
void NeuroComitet<NType>::setMsec(unsigned long msec)
{
    this->msec = msec;
}

template <typename NType>
bool NeuroComitet<NType>::getBlShiftEnd()
{
    return this->blShiftEnd;
}

template <typename NType>
bool NeuroComitet<NType>::getBlNCSave()
{
    return this->blNCSave;
}


template <typename NType>
int NeuroComitet<NType>::getLength()
{
    return this->names.getLength();
}

template <typename NType>
void NeuroComitet<NType>::init_load(const string& path, const string& name_comitet, bool isTrain)
{
    string dir = path;
    string parent = "";
    DataECSV dt;

    this->construct->finit();
    this->construct->setCurrentDir(dir);

    //Комитет
    bool isFileComitet = dt.read(dir + "\\" + name_comitet);
    if(isFileComitet) {this->loadECSV(dt, parent);}

    //Задача
    this->construct->zad = this->zad;
    this->construct->init_zad();
    this->zad = this->construct->zad;
    this->init_examples();

    if(dir[dir.length()-1] != '/') {dir += "/";}

    for(int ind = 0; ind < this->getLength(); ind++)
    {
        this->construct->finit();
        this->construct->setCurrentDir(dir + this->names[ind]);

        //Нейросеть
        this->construct->net = nullptr;
        this->construct->init_net(isTrain);
        this->nets.push(this->construct->net);

        //Обучение
        this->construct->bp = nullptr;
        this->construct->train_type = this->trains_type[ind];
        this->construct->init_bp(isTrain);
        this->trains.push(this->construct->bp);
    }
    this->update_examples();

    this->construct->finit();
    this->construct->setCurrentDir(path);
}

template <typename NType>
void NeuroComitet<NType>::load_zad(const string& path)
{
    this->construct->finit();
    this->construct->setCurrentDir(path);
    this->construct->zad = this->zad;
    this->construct->load_zad();
    this->zad = this->construct->zad;
}

template <typename NType>
void NeuroComitet<NType>::nc_load(const string& path, const string& name_comitet, bool isTrain)
{
    string dir = path;
    string parent = "";
    DataECSV dt;

    this->construct->finit();
    this->construct->setCurrentDir(dir);

    //Комитет
    bool isFileComitet = dt.read(dir + "\\" + name_comitet);
    if(isFileComitet) {this->loadECSV(dt, parent);}

    //Задача
    this->construct->load_zad();
    this->init_examples();

    if(dir[dir.length()-1] != '/') {dir += "/";}

    for(int ind = 0; ind < this->getLength(); ind++)
    {
        this->construct->finit();
        this->construct->setCurrentDir(dir + this->names[ind]);

        //Нейросеть
        this->construct->net = nullptr;
        this->construct->load_net(isTrain);
        this->nets.push(this->construct->net);

        //Обучение
        this->construct->bp = nullptr;
        this->construct->load_bp(isTrain);
        this->trains.push(this->construct->bp);
    }
    this->update_examples();

    this->construct->finit();
    this->construct->setCurrentDir(path);
}

template <typename NType>
void NeuroComitet<NType>::nc_save(const string& path, bool isTrain)
{
    string dir = path;

    this->construct->finit();
    this->construct->setCurrentDir(dir);

    //Задача
    this->construct->zad = this->zad;
    this->construct->save_zad();

    if(dir[dir.length()-1] != '/') {dir += "/";}

    for(int ind = 0; ind < this->getLength(); ind++)
    {
        this->construct->finit();
        this->construct->setCurrentDir(dir + this->names[ind]);

        //Задача
        this->construct->zad = this->tmp_examples[ind];
        this->construct->save_zad();

        //Нейросеть
        this->construct->net = this->nets[ind];
        if(!isTrain || this->trains[ind]->getCountTime() == 1) {this->construct->save_net();}

        //Обучение
        this->construct->bp = this->trains[ind];
        if(!isTrain || this->trains[ind]->getCountTime() == 1) {this->construct->save_bp();}
    }

    this->construct->finit();
    this->construct->setCurrentDir(path);
}

template <typename NType>
void NeuroComitet<NType>::updateCurtime()
{
    char buffer[80];
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    this->curtime = buffer;
}

template <typename NType>
void NeuroComitet<NType>::outdata_saveECSV(string& parent)
{
    vector<string> str_vec;
    string str_val;
    vector<float> colsec_time;
    clock_t period_train;
    DataECSV& dt = this->construct->getDT();
    string field_net = parent + ".net";
    string field_bp = parent + ".bp";
    string field_zad = parent + ".zad";

    //Действия перед сохранением отчёта
    this->updateCurtime();
    this->timer.collapsesec(colsec_time);
    //period_train = (clock_t)(this->timer.deltasec() / this->periodsec) + 1;
    period_train = (clock_t)(this->timer.deltasec());

    //Шапка
    dt.addGroup(parent, "");
    dt.addString(parent, "curtime", this->curtime);
    dt.addString(parent, "names", this->names);
    to_array_string(str_vec, colsec_time); dt.addElement(parent, "timer", str_vec, typeid(float).name());
    str_val = to_string(period_train); dt.addElement(parent, "period_train", str_val, typeid(clock_t).name());

    //Тело
    for(int ind = 0; ind < this->getLength(); ind++)
    {
        this->construct->zad = this->tmp_examples[ind];
        this->construct->net = this->nets[ind];
        this->construct->bp = this->trains[ind];
        this->construct->outdata_net_saveECSV(field_net); //Нейросеть
        this->construct->outdata_bp_saveECSV(field_bp); //Обучение
        this->construct->outdata_zad_saveECSV(field_zad); //Задача
    }
}

template <typename NType>
void NeuroComitet<NType>::outdata_save(const string& path)
{
    //this->construct->setCurrentDir(path);
    string parent = "comitet";
    DataECSV& dt = this->construct->getDT();

    dt.clear();
    this->outdata_saveECSV(parent);
    if(this->blOutdataAppend)
    {
        dt.appendObj(path + "\\" + this->construct->name_outdata);
    }
    else
    {
        dt.write(path + "\\" + this->construct->name_outdata);
    }
}

template <typename NType>
bool NeuroComitet<NType>::outdata_create(const string& path)
{
    DataECSV& dt = this->construct->getDT();
    dt.clear();
    return dt.create(path + "\\" + this->construct->name_outdata);
}

template <typename NType>
void NeuroComitet<NType>::init_examples()
{
    NExamples<NType>* exms;

    this->emptyExamples();
    for(int ind = 0; ind < this->getLength(); ind++)
    {
        exms = this->zad->create();
        this->tmp_examples.push(exms);
    }
}

template <typename NType>
void NeuroComitet<NType>::update_examples()
{
    for(int ind = 0; ind < this->tmp_examples.getLength(); ind++)
    {
        this->tmp_examples[ind]->copyTmpExamples(this->zad);
        this->nets[ind]->tmp_examples = this->tmp_examples[ind];
    }
}

template <typename NType>
void NeuroComitet<NType>::init_train()
{
    for(int ind = 0; ind < this->trains.getLength(); ind++)
    {
        this->trains[ind]->init_train();
    }
}

template <typename NType>
void NeuroComitet<NType>::initProccess()
{
    this->nthreads.init(this->count_parallel, nullptr);
    this->nthreads_work.init(this->count_parallel, false);
    this->task_status.init(this->getLength(), NStatusTask::NStatusNone);
    this->nthreads_task.init(this->count_parallel, ID_NONE);
}

template <typename NType>
void NeuroComitet<NType>::deinit()
{
    INeuroNet<NType>* net;
    TrainBP<NType>* train;

    int len = this->getLength();
    if(this->threadManager != nullptr)
    {
        delete this->threadManager;
        this->threadManager = nullptr;
    }

    //Задачи
    this->emptyExamples();

    //Нейросети
    for(int i = 0; i < len; i++)
    {
        net = this->nets.pop();
        if(net != nullptr) {delete net;}
    }

    //Обучение
    for(int i = 0; i < len; i++)
    {
        train = this->trains.pop();
        if(train != nullptr) {delete train;}
    }

    this->construct->bp = nullptr;
    this->construct->net = nullptr;
    this->construct->zad = nullptr;
    //this->construct = nullptr;
    //this->zad = nullptr;

    this->threadManager = nullptr;
    this->threadManager_work = false;
}

template <typename NType>
void NeuroComitet<NType>::emptyExamples()
{
    NExamples<NType>* exm;
    int len = this->tmp_examples.getLength();

    for(int ind = 0; ind < len; ind++)
    {
        exm = this->tmp_examples.pop();
        if(exm != nullptr) {delete exm;}
        this->nets[ind]->tmp_examples = nullptr;
    }
}

template <typename NType>
bool NeuroComitet<NType>::check()
{
    bool bl = true;
    this->erCheck.clear();
    for(int ind = 0; ind < this->nets.getLength(); ind++)
    {
        if(nets[ind]->check())
        {
            this->erCheck.push(NError::NErrorNone);
        }
        else
        {
            bl = false;
            this->erCheck.push(nets[ind]->getCheck());
        }
    }
    return bl;
}

template <typename NType>
void NeuroComitet<NType>::runFunc(int indTask, NSetType st, bool post)
{
    INeuroNet<NType>* net = this->nets[indTask];
    net->tmp_examples->initTypeSet();
    net->runExamples(st);
    if(post) {net->tmp_examples->postrun(net->getShiftout());}
}

template <typename NType>
void NeuroComitet<NType>::runFuncName(const string& name, NSetType st, bool post)
{
    this->runFunc(this->names.getIndex(name), st, post);
}

template <typename NType>
void NeuroComitet<NType>::runFunc(NSetType st, bool post)
{
    for(int indTask = 0; indTask < this->getLength(); indTask++)
    {
        this->runFunc(indTask, st, post);
    }
}

template <typename NType>
void NeuroComitet<NType>::runTrain(int indTask, int indParallel)
{
    //INeuroNet<NType>* net = this->nets[indTask];
    TrainBP<NType>* train = this->trains[indTask];

    //Процесс обучения
    if(train->getCountTime() > 1)
    {
        train->trainTime();
    }
    else
    {
        train->pretrain();
        train->train();
        train->posttrain();
    }

    this->nthreads_work[indParallel] = false;
    //this->nthreads_task[indParallel] = ID_NONE;
}

template <typename NType>
void NeuroComitet<NType>::runTrainName(const string& name, int indParallel)
{
    this->runTrain(this->names.getIndex(name), indParallel);
}

template <typename NType>
int NeuroComitet<NType>::thread_train(int indTask)
{
    int indParallel = this->nthreads.getLength();
    if(indParallel < this->count_parallel)
    {
        this->nthreads_work.push(true);
        this->nthreads_task.push(indTask);
        this->task_status[indTask] = NStatusTask::NStatusRun;
        thread* pnthread = new thread(&NeuroComitet::runTrain, this, indTask, indParallel);
        this->nthreads.push(pnthread);
        return indParallel;
    }
    else
    {
        return ID_NONE;
    }
}

template <typename NType>
int NeuroComitet<NType>::thread_train(const string& name)
{
    return this->thread_train(this->names.getIndex(name));
}

template <typename NType>
bool NeuroComitet<NType>::thread_train(int indTask, int indParallel)
{
    if(this->nthreads[indParallel] != nullptr) {return false;}
    this->nthreads_work[indParallel] = true;
    this->nthreads_task[indParallel] = indTask;
    this->task_status[indTask] = NStatusTask::NStatusRun;
    thread* pnthread = new thread(&NeuroComitet::runTrain, this, indTask, indParallel);
    this->nthreads[indParallel] = pnthread;
    return true;
}

template <typename NType>
bool NeuroComitet<NType>::thread_train(const string& name, int indParallel)
{
    return this->thread_train(this->names.getIndex(name), indParallel);
}

template <typename NType>
void NeuroComitet<NType>::thread_train()
{
    //if(this->nthreads.getLength() > 0) {throw "Non-deleted threads are present in the pool!";}
    if(this->threadManager != nullptr) {throw "There is a failed Manager thread in the pool!";}
    //this->initProccess();

    for(int indTask = 0; indTask < this->getLength(); indTask++)
    {
        if(this->nets[indTask]->getStatus() < NStatusNet::NStatusInit) {throw "Invalid status net!";}
        this->nets[indTask]->setStatus(NStatusNet::NStatusInit);
    }

    this->timer.start();
    for(int indTask = 0; indTask < this->count_parallel && indTask < this->getLength(); indTask++)
    {
        if(!this->thread_train(indTask, indTask)) {throw "Non-deleted threads are present in the pool!";}
    }

    this->threadManager_work = true;
    this->threadManager = new thread(&NeuroComitet::thread_trainManager, this);
}

template <typename NType>
void NeuroComitet<NType>::thread_trainManager()
{
    int indParallel, indTask;
    bool control = true;

    while(control)
    {
        control = false;
        for(indParallel = 0; indParallel < this->count_parallel; indParallel++)
        {
            if(!this->nthreads_work[indParallel] && this->nthreads[indParallel] != nullptr)
            {
                //Завершение потока
                indTask = this->nthreads_task[indParallel];
                if(this->nthreads[indParallel]->joinable()) {this->nthreads[indParallel]->join();}
                delete this->nthreads[indParallel];
                this->nthreads[indParallel] = nullptr;
                this->task_status[indTask] = NStatusTask::NStatusSuccess;
                this->timer.stop(this->nthreads_task[indParallel]);

                //Создание потока
                indTask = 0;
                while(indTask < this->getLength() && this->task_status[indTask] != NStatusTask::NStatusNone) {indTask++;}
                if(indTask < this->getLength()) {this->thread_train(indTask, indParallel);}
            }

            //Контроль потоков
            if(this->nthreads[indParallel] != nullptr) {control = true;}
        }
        this_thread::sleep_for(chrono::milliseconds(this->msec));
    }

    this->threadManager_work = false;
}

template <typename NType>
void NeuroComitet<NType>::waitManager()
{
    if(this->threadManager->joinable()) {this->threadManager->join();}
    delete this->threadManager;
    this->threadManager = nullptr;
    this->timer.stop();
}

template <typename NType>
void NeuroComitet<NType>::waitProccess(int indParallel)
{
    thread* pnthread = this->nthreads[indParallel];
    if(pnthread != nullptr)
    {
        if(pnthread->joinable()) {pnthread->join();}
        delete pnthread;
        this->nthreads[indParallel] = nullptr;
    }
}

template <typename NType>
void NeuroComitet<NType>::waitProccess()
{
    for(int indParallel = 0; indParallel < this->nthreads.getLength(); indParallel++)
    {
        this->waitProccess(indParallel);
    }
}


template <typename NType>
void NeuroComitet<NType>::saveECSV(DataECSV& dt, string& parent)
{
    int ind;
    vector<string> str_vec;
    string str_val;
    string fieldTrain = parent + ".train";
    string fieldNet = parent + ".net";

    dt.addGroup(parent, "");
    dt.addString(parent, "names", this->names);
    str_val = to_string(this->count_parallel); dt.addElement(parent, "count_parallel", str_val, typeid(int).name());
    str_val = to_string(this->msec); dt.addElement(parent, "msec", str_val, typeid(int).name());
    str_val = to_vstring(this->blShiftEnd); dt.addElement(parent, "blShiftEnd", str_val, typeid(bool).name());
    str_val = to_vstring(this->blOutdataAppend); dt.addElement(parent, "blOutdataAppend", str_val, typeid(bool).name());
    str_val = to_vstring(this->blNCSave); dt.addElement(parent, "blNCSave", str_val, typeid(bool).name());
    to_array_string(str_vec, this->trains_type); dt.addElement(parent, "trains_type", str_vec, typeid(NType).name());

    for(ind = 0; ind < this->trains.getLength(); ind++)
    {
        trains[ind]->saveECSV(dt, fieldTrain);
    }

    for(ind = 0; ind < this->nets.getLength(); ind++)
    {
        nets[ind]->saveECSV(dt, fieldNet);
    }
}

template <typename NType>
void NeuroComitet<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    INeuroNet<NType>* net;
    TrainBP<NType>* train;
    vector<string> str_vec;
    string str_val;
    string fieldTrain = parent + ".train";
    string fieldNet = parent + ".net";
    int indTrain = 0;

    this->deinit();

    size_t ind = dt.getShift();
    iter = dt.modules[ind];
    if(iter->cmpPath(parent, true)) {ind++;}

    while(dt.isNextBlock(ind, parent))
    {
        iter = dt.modules[ind];

             if(iter->getFieldValue(parent, "names", this->names)) {ind++;}
        else if(iter->getFieldValue(parent, "count_parallel", str_val)) {to_value(this->count_parallel, str_val); ind++;}
        else if(iter->getFieldValue(parent, "msec", str_val)) {to_value(this->msec, str_val); ind++;}
        else if(iter->getFieldValue(parent, "blShiftEnd", str_val)) {to_value(this->blShiftEnd, str_val); ind++;}
        else if(iter->getFieldValue(parent, "blOutdataAppend", str_val)) {to_value(this->blOutdataAppend, str_val); ind++;}
        else if(iter->getFieldValue(parent, "blNCSave", str_val)) {to_value(this->blNCSave, str_val); ind++;}
        else if(iter->getFieldValue(parent, "trains_type", str_vec)) {to_array_value(this->trains_type, str_vec); ind++;}
        else if(iter->cmpPath(fieldTrain, true))
        {
            dt.setShift(ind);
            if(this->trains_type.getLength() > 0)
            {
                this->construct->train_type = this->trains_type[indTrain];
                train = this->construct->createTrain();
                indTrain++;
            }
            else
            {
                train = this->construct->createTrain();
            }
            train->loadECSV(dt, fieldTrain);
            this->trains.push(train);
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
