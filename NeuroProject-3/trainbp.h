#ifndef TRAINBP_H
#define TRAINBP_H

#include "ineuronet.h"
#include "iobjectecsv.h"
#include "nfilesconstruct.h"

#include "inoptimizer.h"
#include "noptimizeradam.h"

template <typename NType>
class TrainBP: public IObjectECSV
{
public:
    TrainBP();
    virtual ~TrainBP();

public:
    INeuroNet<NType>* net; //Обучаемая нейронная сеть
    INeuroNet<NType>* changeNet; //Копия нейросети для изменений весов
    NArray< NArray<NType>* > errorNet; //Ошибки слоёв
    NGroupEnergy<NType> geTrain; //Групповая энергия обучающего множества
    NGroupEnergy<NType> geTest; //Групповая энергия тестового множества
    NArray<NType> energyAver; //Средняя ошибка обучения
    NArray<NType> energyMax; //Максимальная ошибка обучения
    NArray<NType> energyTestAver; //Средняя ошибка обучения
    NArray<NType> energyTestMax; //Максимальная ошибка обучения
    NArray<NType> speedLearningHistory; //Изменение скорости обучения
    NMatrix<NType> state; //Матрица состояний
    NFilesConstruct path; //Обучение Time: путь к файлу сериализации
    NArray<NType> optParam; //Параметры алгоритма оптимизации

protected:
    NType speedLearningInit; //Обучение: первоначальная скорость обучения
    NType speedLearningMax; //Обучение: максимальная скорость обучения
    NType speedRegularization; //Обучение: скорость регуляризации
    NType inertia; //Обучение: инерция
    NType koefIncEnergy; //Коэффициент допустимого прироста погрешности изменения энергии
    NType decSpeedLearning; //Коэффициент уменьшения скорости обучения
    NType incSpeedLearning; //Коэффициент увеличения скорости обучения
    int maxEpoches; //Обучение: максимальное количество эпох
    int maxGame; //Максимальное количество игр
    int countBath; //Обучение: размер мини-выборки
    int stepExmBath; //Обучение: смещение примеров каждую эпоху
    NType minError; //Обучение: минимальная ошибка обучающего множества
    NType minErrorChange; //Обучение: минимальное изменение энергии
    int epoches; //Количество эпох
    int iterGame; //Количество игр
    int stabEnergy; //Количество стабилизаций энергии
    NType changeEnergy; //Изменение энергии
    int stepReg; //Обучение: шаг сохранения энергии
    int stepDropout; //Шаг изменения dropout
    bool mixtrain; //Обучение: перемешивать примеры в обучающем множестве?
    bool mixtest; //Обучение: перемешивание тестовых примеров с обучающими
    bool blState; //Обучение: механизм сохранения нейросети в матрицу состояний
    NType minErrorTest; //Обучение: минимальная ошибка тестового множества
    bool blErrorTest; //Обучение: использование тестового множества для оценки
    bool blDropout; //Обучение: использование при обучении dropout
    bool blDropoutExm; //Использовать dropout для каждого примера
    bool isGame; //Отслеживание игр, а не эпох
    NOptimizerType optType; //Тип алгоритма оптимизации

    int stepExmTime; //Обучение Time: Шаг взятия примеров
    int countTime; //Обучение Time: Количество обучений нейросети Time
    bool clearNet; //Обучение Time: Каждый раз очищать нейросеть
    bool blShift; //Обучение Time: Будут ли смещаться тестовые/обучающие типы примеров вместе с окном при множественном обучении?

protected:
    NArray<NType> backOut; //Производная целевой функции или вектор, рассчитанный по дельта-правилу
    NMatrix<NType> changeWeight1, changeWeight2;
    NArray<NType> changeBias1, changeBias2;
    NType speedLearning; //Скорость обучения
    NType koefTrain; //Коэффициент обучения, агрегирующий другие коэффициенты
    NType koefInert; //Коэффициент инерции
    NType koefRegular; //Коэффициент регуляризации
    NType koefDropout; //Коэффициент dropout
    INLayer<NType>** pLay; //net.lay
    NExample<NType>** pExam; //net.tmp_examples
    NArray<NType>** pMas; //errorNet
    INLayer<NType>** pChangeLay; //changeNet.lay
    //NType* pOutrun; //exm.outrun
    int lenLay; //net.lay - Количество слоёв
    //int pos; //Счётчик примеров
    INOptimizer<NType>* optimizer; //Алгоритм оптимизации

public:
    void setSpeedLearningInit(NType param);
    void setKoefIncEnegry(NType param);
    void setDecSpeedLearning(NType param);
    void setIncSpeedLearning(NType param);
    void setSpeedRegularization(NType param);
    void setInertia(NType param);
    void setMaxEpoches(int param);
    void setMaxGame(int value);
    void setCountBath(int param);
    void setStepExmBath(int param);
    void setMinError(NType param);
    void setMinErrorChange(NType param);
    void setStepReg(int param);
    void setStepDropout(int param);
    NType getSpeedLearningInit();
    NType getKoefIncEnegry();
    NType getDecSpeedLearning();
    NType getIncSpeedLearning();
    NType getSpeedRegularization();
    NType getInertia();
    int getEpoches();
    int getIterGame();
    int getCountBath();
    int getStepExmBath();
    int getStabEnergy();
    NType getChangeEnergy();
    int getStepReg();
    int getStepDropout();
    void setMixTrain(bool param);
    bool getMixTrain();
    void setMixTest(bool param);
    bool getMixTest();
    void setBlState(bool param);
    void setMinErrorTest(NType param);
    void setBlErrorTest(bool param);

    void setStepExmTime(int param);
    void setCountTime(int param);
    int getStepExmTime();
    int getCountTime();

public:
    virtual void reset(); //Сброс параметров обучения
    virtual void init(); //Инициализация
    virtual void deinit(); //Деинициализация
    virtual void init_train(); //Действия перед циклом обучения
    virtual void pretrain(); //Действия перед обучением
    virtual void posttrain(); //Действия после обучения
    virtual void train(); //Процесс обучения
    virtual void trainTime();
    virtual void calculateError(NExample<NType>* tmp_exm); //Расчёт ошибок обучения
    virtual void calculateLock(); //Блокировка нейронов от изменения
    virtual void change(NExample<NType>* tmp_exm); //Расчёт изменений весов
    virtual void doMaskLock(); //Блокировка нейронов от изменения
    virtual void update(); //Принятие изменений
    void createDB(); //Создать БД
    void openDB(); //Открыть файлы БД
    void appendDB(); //Дописать в конец БД
    void closeDB(); //Закрыть файлы БД
    void fieldsDB(); //Выводимые поля БД
    INOptimizer<NType>* newOptimizer(NOptimizerType optType); //Создание оптимизатора
    INOptimizer<NType>* newOptimizer(NOptimizerType optType, NArray<NType>& param);

protected:
    virtual void makePoint(); //Добавить отладочную информацию
    virtual int act(NExample<NType>* exm); //Функция выбора действия
    virtual void train_epoch(int beginset, int testset); //1 эпоха обучения
    virtual void calcKoefsInitTrain(); //Инициализация коэффициентов обучения
    virtual void calcKoefsTrain(NType& curEnrg, NType& prevEnrg); //Пересчёт коэффициентов обучения
    NType calcErrorTrain(bool blEnrg); //Ошибка обучающего множества
    NType calcErrorTest(bool blEnrg); //Учёт ошибки тестового множества
    NType calcErrorValidate(bool blEnrg); //Ошибка валидационного множества
    NType calcError(bool blEnrg); //Общее множество
    virtual void train_initparam(NType& curEnrg); //Инициализация параметров обучения
    virtual void train_drop(bool isPoint); //Применение dropout
    NExample<NType>* nextUsageExm(int& pos); //Найти следующий действующий пример

public:
    virtual void outdata_saveECSV(DataECSV& dt, string& parent);
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
TrainBP<NType>::TrainBP()
{
    this->net = nullptr;
    this->changeNet = nullptr;
    this->speedLearningInit = 1;
    this->speedLearningMax = 1;
    this->speedLearning = 1;
    this->koefIncEnergy = 1;
    this->decSpeedLearning = 1;
    this->incSpeedLearning = 1;
    this->speedRegularization = 0;
    this->inertia = 0;
    this->maxEpoches = 0;
    this->maxGame = 0;
    this->countBath = 0;
    this->stepExmBath = 0;
    this->minError = 0;
    this->minErrorChange = 0;
    this->epoches = 0;
    this->iterGame = 0;
    this->changeEnergy = 0;
    this->stabEnergy = 0;
    this->stepReg = 0;
    this->stepDropout = 0;
    this->mixtrain = false;
    this->mixtest = false;
    this->blState = false;
    this->minErrorTest = 0;
    this->blErrorTest = false;
    this->blDropout = false;
    this->blDropoutExm = false;
    this->stepExmTime = 1;
    this->countTime = 1;
    this->clearNet = true;
    this->blShift = true;
    this->isGame = false;
    this->optimizer = nullptr;
}

template <typename NType>
TrainBP<NType>::~TrainBP()
{
    this->deinit();
}


template <typename NType>
void TrainBP<NType>::setSpeedLearningInit(NType param)
{
    this->speedLearningInit = param;
}

template <typename NType>
void TrainBP<NType>::setKoefIncEnegry(NType param)
{
    this->koefIncEnergy = param;
}

template <typename NType>
void TrainBP<NType>::setDecSpeedLearning(NType param)
{
    this->decSpeedLearning = param;
}

template <typename NType>
void TrainBP<NType>::setIncSpeedLearning(NType param)
{
    this->incSpeedLearning = param;
}

template <typename NType>
void TrainBP<NType>::setSpeedRegularization(NType param)
{
    this->speedRegularization = param;
}

template <typename NType>
void TrainBP<NType>::setInertia(NType param)
{
    this->inertia = param;
}

template <typename NType>
void TrainBP<NType>::setMaxEpoches(int param)
{
    this->maxEpoches = param;
}

template <typename NType>
void TrainBP<NType>::setMaxGame(int value)
{
    this->maxGame = value;
}

template <typename NType>
void TrainBP<NType>::setCountBath(int param)
{
    this->countBath = param;
}

template <typename NType>
void TrainBP<NType>::setStepExmBath(int param)
{
    this->stepExmBath = param;
}

template <typename NType>
void TrainBP<NType>::setMinError(NType param)
{
    this->minError = param;
}

template <typename NType>
void TrainBP<NType>::setMinErrorTest(NType param)
{
    this->minErrorTest = param;
}

template <typename NType>
void TrainBP<NType>::setMinErrorChange(NType param)
{
    this->minErrorChange = param;
}

template <typename NType>
void TrainBP<NType>::setStepReg(int param)
{
    this->stepReg = param;
}

template <typename NType>
void TrainBP<NType>::setStepDropout(int param)
{
    this->stepDropout = param;
}

template <typename NType>
NType TrainBP<NType>::getSpeedLearningInit()
{
     return this->speedLearningInit;
}

template <typename NType>
NType TrainBP<NType>::getKoefIncEnegry()
{
     return this->koefIncEnergy;
}

template <typename NType>
NType TrainBP<NType>::getDecSpeedLearning()
{
     return this->decSpeedLearning;
}

template <typename NType>
NType TrainBP<NType>::getIncSpeedLearning()
{
     return this->incSpeedLearning;
}

template <typename NType>
NType TrainBP<NType>::getSpeedRegularization()
{
    return this->speedRegularization;
}

template <typename NType>
NType TrainBP<NType>::getInertia()
{
    return this->inertia;
}

template <typename NType>
int TrainBP<NType>::getEpoches()
{
    return this->epoches;
}

template <typename NType>
int TrainBP<NType>::getIterGame()
{
    return this->iterGame;
}

template <typename NType>
int TrainBP<NType>::getCountBath()
{
    return this->countBath;
}

template <typename NType>
int TrainBP<NType>::getStepExmBath()
{
    return this->stepExmBath;
}

template <typename NType>
int TrainBP<NType>::getStabEnergy()
{
    return this->stabEnergy;
}

template <typename NType>
NType TrainBP<NType>::getChangeEnergy()
{
    return this->changeEnergy;
}

template <typename NType>
int TrainBP<NType>::getStepReg()
{
    return this->stepReg;
}

template <typename NType>
int TrainBP<NType>::getStepDropout()
{
    return this->stepDropout;
}

template <typename NType>
void TrainBP<NType>::setMixTrain(bool param)
{
    this->mixtrain = param;
}

template <typename NType>
bool TrainBP<NType>::getMixTrain()
{
    return this->mixtrain;
}

template <typename NType>
void TrainBP<NType>::setMixTest(bool param)
{
    this->mixtest = param;
}

template <typename NType>
bool TrainBP<NType>::getMixTest()
{
    return this->mixtest;
}

template <typename NType>
void TrainBP<NType>::setBlState(bool param)
{
    this->blState = param;
}

template <typename NType>
void TrainBP<NType>::setBlErrorTest(bool param)
{
    this->blErrorTest = param;
}

template <typename NType>
void TrainBP<NType>::setStepExmTime(int param)
{
    this->stepExmTime = param;
}

template <typename NType>
void TrainBP<NType>::setCountTime(int param)
{
    this->countTime = param;
}

template <typename NType>
int TrainBP<NType>::getStepExmTime()
{
    return this->stepExmTime;
}

template <typename NType>
int TrainBP<NType>::getCountTime()
{
    return this->countTime;
}


template <typename NType>
void TrainBP<NType>::reset()
{
    this->changeEnergy = 0;
    this->epoches = 0;
    this->iterGame = 0;
    this->stabEnergy = 0;

    this->geTrain.clear();
    this->geTest.clear();
    this->energyAver.clear();
    this->energyMax.clear();
    this->energyTestAver.clear();
    this->energyTestMax.clear();
    this->speedLearningHistory.clear();
    this->state.clear();

    this->changeNet->init();
}

template <typename NType>
void TrainBP<NType>::init()
{
    this->deinit();
    time_srand();
    this->path.init();

    //Мини-батчи
    if(this->countBath == 0) {this->countBath = this->net->examples->getTestset() - this->net->examples->getBeginset();}
    if(this->stepExmBath == 0) {this->stepExmBath = (this->countBath > 0 ? this->countBath : 1);}

    //Инициализация нейросети для изменения весов
    if(this->changeNet == nullptr)
    {
        this->changeNet = new INeuroNet<NType>;
    }
    this->changeNet->setExtern(false);
    this->changeNet->copyNet(this->net);
    this->changeNet->setValueWeight(0);

    //Инициализация массива ошибок слоёв
    NArray<NType>* mas;
    INLayer<NType>* layer;
    for(int i = 0; i < this->net->lay.getLength(); i++)
    {
        layer = this->net->lay[i];
        mas = new NArray<NType>();
        mas->init(layer->getLenNeurons(), 0);
        this->errorNet.push(mas);
    }

    //Инициализация алгоритма оптимизации
    this->optimizer = (this->optParam.empty() ? this->newOptimizer(this->optType) : this->newOptimizer(this->optType, this->optParam));

    this->reset();
}

template <typename NType>
void TrainBP<NType>::deinit()
{
    if(this->changeNet != nullptr)
    {
        this->changeNet->emtyLayer();
        delete this->changeNet;
        this->changeNet = nullptr;
    }

    NArray<NType>* mas;
    int len = this->errorNet.getLength();
    for(int i = 0; i < len; i++)
    {
        mas = this->errorNet.pop();
        delete mas;
    }

    //Алгоритм оптимизации
    if(this->optimizer != nullptr)
    {
        delete this->optimizer;
        this->optimizer = nullptr;
    }
}

template <typename NType>
void TrainBP<NType>::init_train()
{
    if(this->blShift)
    {
        if(this->mixtest) {this->net->tmp_examples->mixTest();}
        this->net->tmp_examples->initTypeSet();
    }
    if(this->net->tmp_examples == nullptr) {this->net->tmp_examples = this->net->examples;}
}

template <typename NType>
void TrainBP<NType>::pretrain()
{
    if(this->blShift)
    {
        this->net->tmp_examples->mixTypeSet();
    }
    else
    {
        if(this->mixtest) {this->net->tmp_examples->mixTest();}
        this->net->tmp_examples->initTypeSet();
    }

    if(this->clearNet)
    {
        if(this->net->getTypeInit() == NInitType::NInitEmpty) {this->net->init();}
        else if(this->net->getTypeInit() == NInitType::NInitG) {this->net->ginit();}
    }

    this->reset();
}

template <typename NType>
void TrainBP<NType>::posttrain()
{
    this->net->runExamples(NSetType::NSetValidate);
    this->net->tmp_examples->postrun(this->net->getShiftout());
    this->net->tmp_examples->runEnergyClass(NSetType::NSetTrain);
    this->net->funcRegularization();
}

template <typename NType>
void TrainBP<NType>::train()
{
    NType prevEnrg = 0, curEnrg = 0, curEnrgTest = 0;
    bool isPoint = true;
    int beginset = this->net->tmp_examples->getBeginset();
    int testset = this->net->tmp_examples->getTestset();
    bool isOnline = this->net->tmp_examples->getIsOnline();

    //Прореживание примеров
    this->net->tmp_examples->initUsage(NSetType::NSetTrain);
    this->net->tmp_examples->thinnigExamples(this->net->getIndexOutNumClass());

    //Начальные данные
    this->net->setStatus(NStatusNet::NStatusInit);
    this->train_initparam(curEnrg);
    curEnrg = this->calcErrorTrain(false);
    this->calcKoefsInitTrain();
    prevEnrg = curEnrg;
    if(this->blErrorTest) {curEnrgTest = this->calcErrorTest(false);}

    //Точка отладки
    if(this->stepReg != 0) {this->makePoint();}

    //Дропаут
    this->train_drop(true);
    if(blDropout) {this->net->initKoefDropout();}

    //Цикл обучения
    while(this->epoches < this->maxEpoches
          && (!this->isGame || this->iterGame < this->maxGame)
          && curEnrg > this->minError
          && (!this->blErrorTest || curEnrgTest > this->minErrorTest)
          && this->changeEnergy >= this->minErrorChange)
    {
        while(this->epoches < this->maxEpoches
            && (!this->isGame || this->iterGame < this->maxGame)
            && curEnrg > this->minError
            && (!this->blErrorTest || curEnrgTest > this->minErrorTest)
            && this->changeEnergy >= this->minErrorChange)
        {
            //Прореживание примеров
            if(this->mixtrain)
            {
                this->net->tmp_examples->mixTrain();
                this->net->tmp_examples->initUsage(NSetType::NSetTrain);
                this->net->tmp_examples->thinnigExamples(this->net->getIndexOutNumClass());
            }

            //1 эпоха обучения
            if(isOnline) {this->net->tmp_examples->initState();}
            train_epoch(beginset, testset);
            isPoint = (this->stepReg != 0)&&(this->isGame ? true : this->epoches % this->stepReg == 0);
            curEnrg = this->calcErrorTrain(false);

            //Алгоритм изменения скорости обучения
            this->calcKoefsTrain(curEnrg, prevEnrg);

            //Изменение условий цикла
            this->changeEnergy = prevEnrg - curEnrg;
            if(this->changeEnergy < 0) {this->changeEnergy = -this->changeEnergy;}
            prevEnrg = curEnrg;
            if(this->blErrorTest) {curEnrgTest = this->calcErrorTest(false);}

            //Точка отладки
            if(isPoint) {this->makePoint();}
            if(isOnline) {this->net->tmp_examples->deinitState();}

            //Дропаут
            this->train_drop(stepDropout > 0 && (this->isGame ? true : this->epoches % stepDropout == 0));
        }

        //Стабилизация
        this->stabEnergy++;
        curEnrg = this->calcErrorTrain(false);

        //Изменение условий цикла
        this->changeEnergy = prevEnrg - curEnrg;
        if(this->changeEnergy < 0) {this->changeEnergy = -this->changeEnergy;}
        prevEnrg = curEnrg;
    }

    //Тестовая выборка
    if(isOnline) {this->calcErrorTrain(false);}
    this->net->tmp_examples->getGroupEnergy(this->geTrain);
    this->calcErrorTest(this->blErrorTest);
    this->net->tmp_examples->setGroupEnergy(this->geTrain);

    //Точка отладки
    if(this->stepReg != 0) {this->makePoint();}

    //Конечные данные
    this->net->setStatus(NStatusNet::NStatusTrain);
}

template <typename NType>
void TrainBP<NType>::trainTime()
{
    this->createDB();
    this->openDB();

    int beginset = this->net->tmp_examples->getBeginset();
    //this->net->tmp_examples->update();
    //this->init_train();
    this->fieldsDB();

    for(int tm = 0; tm < this->countTime; tm++)
    {
        this->pretrain();
        this->train();
        this->posttrain();
        //if(tm == 1) {this->fieldsDB();}
        this->appendDB();
        this->net->tmp_examples->doShift(this->stepExmTime, this->blShift);
    }

    this->net->tmp_examples->portion(beginset);
    this->closeDB();
}

template <typename NType>
void TrainBP<NType>::calculateError(NExample<NType>* tmp_exm)
{
    //Тест
    /*NArray<NType>* outtst;
    outtst = &this->net->lay[0]->output; cout << "outrun=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[1]->output; cout << "outrun=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[2]->output; cout << "outrun=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[0]->sum; cout << "sum=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[1]->sum; cout << "sum=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[2]->sum; cout << "sum=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";*/
    //cout << "outrun=" << tmp_exm->outrun.get(0) << ", " << tmp_exm->outrun.get(1) << ", " << tmp_exm->outrun.get(2) << "\n";}

    // Выходной слой
    int k;
    NArray<NType>& er = this->net->derivEnergy(tmp_exm);
    NArray<NType>* outlay = this->net->derivActivLay(tmp_exm);

    k = lenLay-1;
    pLay[k]->derivRun(pMas[k]);
    //if(blDropout) {pMas[k]->mul(pLay[k]->slay);}
    if(outlay != nullptr) {pMas[k]->mul(*outlay);}
    pMas[k]->mul(er);
    //cout << "d=" << pMas[k]->get(0) << ", " << pMas[k]->get(1) << ", " << pMas[k]->get(2) << "\n"; //Тест

    // Скрытый слой
    if(blDropout)
    {
        for(k = lenLay-2; k >= 0; k--)
        {
            pLay[k]->derivRun(pMas[k]);
            //pMas[k]->mul(pLay[k]->slay);
            if(pLay[k+1]->getDropout() > 0)
            {
                this->backOut.mul(*pMas[k+1], pLay[k+1]->slay, false);
            }
            else
            {
                this->backOut.mul(*pMas[k+1], pLay[k+1]->weigth, false);
            }
            pMas[k]->mul(this->backOut);
        }
    }
    else
    {
        for(k = lenLay-2; k >= 0; k--)
        {
            pLay[k]->derivRun(pMas[k]);
            this->backOut.mul(*pMas[k+1], pLay[k+1]->weigth, false);
            pMas[k]->mul(this->backOut);
        }
    }
}

template <typename NType>
void TrainBP<NType>::calculateLock()
{
    int k, ind, ind_shot = 0;
    for(k = 0; k < this->net->lay.getLength(); k++)
    {
        for(ind = 0; ind < pLay[k]->getLenNeurons(); ind++)
        {
            if(net->lock[ind_shot]) {pMas[k]->set(0, ind);}
            ind_shot++;
        }
    }
}

template <typename NType>
void TrainBP<NType>::change(NExample<NType>* tmp_exm)
{
    int k = 0;
    // Входной слой weight
    //koefDropout = (blDropout ? pLay[k]->getKoefDropout() : 1);
    this->changeWeight1.mul(tmp_exm->original->input, *pMas[k]);
    this->optimizer->runWeight(this->changeWeight1, k);
    this->changeWeight1.valmul(koefTrain);

    this->changeWeight2.copyValue(pChangeLay[k]->weigth);
    this->changeWeight2.valmul(koefInert);
    this->changeWeight2.sum(this->changeWeight1);

    this->changeWeight1.copyValue(pLay[k]->weigth);
    this->changeWeight1.valmul(koefRegular);
    pChangeLay[k]->weigth.sum(this->changeWeight1, this->changeWeight2);

    // Скрытый слой weight
    for(k = 1; k < this->net->lay.getLength(); k++)
    {
        //koefDropout = (blDropout ? pLay[k]->getKoefDropout() : 1);
        this->changeWeight1.mul(pLay[k-1]->output, *pMas[k]);
        this->optimizer->runWeight(this->changeWeight1, k);
        this->changeWeight1.valmul(koefTrain);

        this->changeWeight2.copyValue(pChangeLay[k]->weigth);
        this->changeWeight2.valmul(koefInert);
        this->changeWeight2.sum(this->changeWeight1);

        this->changeWeight1.copyValue(pLay[k]->weigth);
        this->changeWeight1.valmul(koefRegular);
        pChangeLay[k]->weigth.sum(this->changeWeight1, this->changeWeight2);
    }

    // Cлой bias
    for(k = 0; k < this->net->lay.getLength(); k++)
    {
        //koefDropout = (blDropout ? pLay[k]->getKoefDropout() : 1);
        this->changeBias1.copyValue(*pMas[k]);
        this->optimizer->runBias(this->changeBias1, k);
        this->changeBias1.valmul(koefTrain);

        this->changeBias2.copyValue(pChangeLay[k]->bias);
        this->changeBias2.valmul(koefInert);
        this->changeBias2.sum(this->changeBias1);

        this->changeBias1.copyValue(pLay[k]->bias);
        this->changeBias1.valmul(koefRegular);
        pChangeLay[k]->bias.sum(this->changeBias1, this->changeBias2);
    }
}

template <typename NType>
void TrainBP<NType>::doMaskLock()
{
    bool* pLock = net->lock.getData();
    for(int k = 0; k < this->net->lay.getLength(); k++)
    {
        pChangeLay[k]->weigth.doMaskRow(pLock);
        pChangeLay[k]->bias.doMask(pLock);
        pLock += pLay[k]->getLenNeurons();
    }
}

template <typename NType>
void TrainBP<NType>::update()
{
    for(int k = 0; k < this->net->lay.getLength(); k++)
    {
        pLay[k]->weigth.sum(pChangeLay[k]->weigth);
        pLay[k]->bias.sum(pChangeLay[k]->bias);

        //Тест
        /*NType valuew, valueb;
        for(int j = 0; j < pChangeLay[k]->weigth.getLenColumn(); j++)
        {
            for(int i = 0; i < pChangeLay[k]->weigth.getLenRow(); i++)
            {
                valuew = pChangeLay[k]->weigth.get(i, j);
                if(isnan(valuew)) {cout << i << ", " << j << " = " << valuew << "\n";}
            }
            valueb = pChangeLay[k]->bias.get(j);
            if(isnan(valueb)) {cout << j << " = " << valueb << "\n";}
        }*/
    }
}

template <typename NType>
void TrainBP<NType>::createDB()
{
    //this->path.fremove();
    this->path.init();
    this->path.fopen(ios_base::out | ios_base::trunc);
    if(!this->path.is_open())
    {
        throw "Not file";
    }
    this->path.fclose();
}

template <typename NType>
void TrainBP<NType>::openDB()
{
    this->path.init();
    this->path.fopen(ios_base::in | ios_base::out | ios_base::binary);
    this->path.tmp_fopen(ios_base::out | ios_base::trunc | ios_base::ate | ios_base::binary);
    if(!this->path.is_open() || !this->path.is_tmp_open())
    {
        throw "Not file";
    }
}

template <typename NType>
void TrainBP<NType>::appendDB()
{
    string parentTrain = "train";
    string parentNet = "net";
    string parentExamples = "exms";

    path.fclose();
    path.fopen(ios_base::in | ios_base::out | ios_base::trunc | ios_base::binary);

    path.dt_trainbp.clearData();
    this->saveECSV(path.dt_trainbp, parentTrain);
    path.dt_trainbp.appendObj(path.tmp_ftrainbp);
    path.dt_trainbp.writeHead(path.ftrainbp);

    path.dt_neuronet.clearData();
    this->net->saveECSV(path.dt_neuronet, parentNet);
    path.dt_neuronet.appendObj(path.tmp_fneuronet);
    path.dt_neuronet.writeHead(path.fneuronet);

    path.dt_examples.clearData();
    this->net->tmp_examples->saveECSV(path.dt_examples, parentExamples);
    path.dt_examples.appendObj(path.tmp_fexamples);
    path.dt_examples.writeHead(path.fexamples);
}

template <typename NType>
void TrainBP<NType>::closeDB()
{
    this->path.fclose();
    this->path.tmp_fclose();

    if(!this->path.unionHeadBody())
    {
        throw "Not union files";
    }

    this->path.tmp_fremove();
    this->path.init();
}

template <typename NType>
void TrainBP<NType>::fieldsDB()
{
    //string parentTrain = "train";
    //string parentNet = "net";
    string parentExamples = "exms";

    this->pushFieldNotSave(parentExamples, "examples.data");

    path.dt_examples.fieldsNotSave = &fieldsNotSave;
}

template <typename NType>
INOptimizer<NType>* TrainBP<NType>::newOptimizer(NOptimizerType optType)
{
    INOptimizer<NType>* opt = nullptr;

         if(optType == NOptimizerType::NOptimizerSGD)   {opt = new INOptimizer<NType>(this);}
    else if(optType == NOptimizerType::NOptimizerAdam)  {opt = new NOptimizerAdam<NType>(this);}

    opt->setType(optType);

    return opt;
}

template <typename NType>
INOptimizer<NType>* TrainBP<NType>::newOptimizer(NOptimizerType optType, NArray<NType>& param)
{
    INOptimizer<NType>* opt = this->newOptimizer(optType);
    opt->init(param);
    return opt;
}

template <typename NType>
void TrainBP<NType>::makePoint()
{
    //this->energyAver.push(this->net->tmp_examples->getEnergyAver());
    //this->energyMax.push(this->net->tmp_examples->getEnergyMax());
    this->energyAver.push(this->geTrain.energyAver);
    this->energyMax.push(this->geTrain.energyMax);
    this->speedLearningHistory.push(this->speedLearning);

    if(this->blState)
    {
        if(!this->net->isShotData()) {this->net->makeShot();}
        this->state.pushRow(this->net->shot);
    }

    if(this->blErrorTest)
    {
        //this->energyTestAver.push(this->net->tmp_examples->getEnergyAver());
        //this->energyTestMax.push(this->net->tmp_examples->getEnergyMax());
        this->energyTestAver.push(this->geTest.energyAver);
        this->energyTestMax.push(this->geTest.energyMax);
    }
}

template <typename NType>
int TrainBP<NType>::act(NExample<NType>* /*exm*/)
{
    int action;

    if(this->net->getLenOut() == 1)
    {
        uniform_int_distribution<> distributionAct(0, 1);
        action = distributionAct(NRandGenerator);
    }
    else
    {
        uniform_int_distribution<> distributionAct(0, this->net->tmp_examples->getLenClass()-1);
        action = distributionAct(NRandGenerator);
    }

    return action;
}

template <typename NType>
void TrainBP<NType>::train_epoch(int beginset, int testset)
{
    int pos;
    NExample<NType>* tmp_exm;
    bool blLock = this->net->blLock;
    int bathset;
    int action;
    testset -= countBath;

    //Онлайн обучение
    if(this->net->tmp_examples->getIsOnline())
    {
        for(pos = beginset; pos < testset-1; pos++)
        {
            tmp_exm = this->pExam[pos];
            action = this->act(tmp_exm);
            this->net->tmp_examples->nextState(action, pos);
        }
    }

    //Эпоха обучения
    if(blDropout)
    {
        while(beginset <= testset)
        {
            bathset = beginset + countBath;
            for(pos = beginset; pos < bathset; pos++)
            {
                tmp_exm = this->pExam[pos];
                //cout << "Primer: " << pos << "\n"; //Тест
                if(tmp_exm->getUsage())
                {
                    if(blDropoutExm) {this->net->init_slay();}
                    this->net->runExampleDropout(tmp_exm);
                    calculateError(tmp_exm);
                    change(tmp_exm);
                    if(blLock) {doMaskLock();}
                    update();
                }
            }
            beginset += stepExmBath;
        }
    }
    else
    {
        while(beginset <= testset)
        {
            bathset = beginset + countBath;
            for(pos = beginset; pos < bathset; pos++)
            {
                tmp_exm = this->pExam[pos];
                //cout << "Primer: " << pos << "\n"; //Тест
                if(tmp_exm->getUsage())
                {
                    this->net->runExample(tmp_exm);
                    //cout << "Energy: " << tmp_exm->getEnergy() << "\n"; //Тест
                    calculateError(tmp_exm);
                    change(tmp_exm);
                    if(blLock) {doMaskLock();}
                    update();
                }
            }
            beginset += stepExmBath;
        }
    }

    this->epoches++;
    this->iterGame++;
}

template <typename NType>
void TrainBP<NType>::calcKoefsInitTrain()
{
    this->speedLearning = this->speedLearningInit;
    this->koefTrain = this->speedLearning * (1 - this->inertia);
    this->koefInert = this->speedLearning * this->inertia;
    this->koefRegular = - (this->speedRegularization * this->koefTrain) / this->net->tmp_examples->getLenExamples(NSetType::NSetTrain);
}

template <typename NType>
void TrainBP<NType>::calcKoefsTrain(NType& curEnrg, NType& prevEnrg)
{
    this->speedLearning *= (curEnrg > this->koefIncEnergy * prevEnrg ? this->decSpeedLearning : this->incSpeedLearning);
    if(this->speedLearning > this->speedLearningMax) {this->speedLearning = this->speedLearningMax;}
    this->koefTrain = this->speedLearning * (1 - this->inertia);
    this->koefInert = this->speedLearning * this->inertia;
    this->koefRegular = - (this->speedRegularization * this->koefTrain) / this->net->tmp_examples->getLenExamples(NSetType::NSetTrain);
}

template <typename NType>
NType TrainBP<NType>::calcErrorTrain(bool blEnrg)
{
    if(blEnrg) {this->net->runEnergy(NSetType::NSetTrain);}
    else {this->net->runExamples(NSetType::NSetTrain);}
    this->net->tmp_examples->getGroupEnergy(this->geTrain);
    return this->net->tmp_examples->getEnergyAver();
}

template <typename NType>
NType TrainBP<NType>::calcErrorTest(bool blEnrg)
{
    if(blEnrg) {this->net->runEnergy(NSetType::NSetTest);}
    else {this->net->runExamples(NSetType::NSetTest);}
    this->net->tmp_examples->getGroupEnergy(this->geTest);
    return this->net->tmp_examples->getEnergyAver();
}

template <typename NType>
NType TrainBP<NType>::calcErrorValidate(bool blEnrg)
{
    if(blEnrg) {this->net->runEnergy(NSetType::NSetValidate);}
    else {this->net->runExamples(NSetType::NSetValidate);}
    return this->net->tmp_examples->getEnergyAver();
}

template <typename NType>
NType TrainBP<NType>::calcError(bool blEnrg)
{
    if(blEnrg) {this->net->runEnergy(NSetType::NSetNone);}
    else {this->net->runExamples(NSetType::NSetNone);}
    return this->net->tmp_examples->getEnergyAver();
}

template <typename NType>
void TrainBP<NType>::train_initparam(NType& curEnrg)
{
    this->pLay = this->net->lay.getData();
    this->pExam = this->net->tmp_examples->getData();
    this->pMas = this->errorNet.getData();
    this->pChangeLay = this->changeNet->lay.getData();
    this->lenLay = this->net->lay.getLength();
    this->changeEnergy = curEnrg;
    if(blDropout && stepDropout == 0) {stepDropout = (this->isGame ? this->maxGame : this->maxEpoches) + 1;}
}

template <typename NType>
void TrainBP<NType>::train_drop(bool isPoint)
{
    if(blDropout && !blDropoutExm && isPoint) {this->net->init_slay();}
}

template <typename NType>
NExample<NType>* TrainBP<NType>::nextUsageExm(int& pos)
{
    while(!this->pExam[pos]->getUsage()) {pos++;}
    return this->pExam[pos];
}


template <typename NType>
void TrainBP<NType>::outdata_saveECSV(DataECSV& dt, string& parent)
{
    vector<string> str_vec;

    //Построение блоков
    dt.addGroup(parent, "");
    to_array_string(str_vec, this->energyAver); dt.addElement(parent, "energyAver", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyMax); dt.addElement(parent, "energyMax", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyTestAver); dt.addElement(parent, "energyTestAver", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyTestMax); dt.addElement(parent, "energyTestMax", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->speedLearningHistory); dt.addElement(parent, "speedLearningHistory", str_vec, typeid(NType).name());
    //to_array_string(str_vec, this->scores); dt.addElement(parent, "scores", str_vec, typeid(NType).name());
}

template <typename NType>
void TrainBP<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;

    dt.addGroup(parent, "");
    str_val = to_string(this->speedLearningInit); dt.addElement(parent, "speedLearningInit", str_val, typeid(NType).name());
    str_val = to_string(this->speedLearningMax); dt.addElement(parent, "speedLearningMax", str_val, typeid(NType).name());
    str_val = to_string(this->koefIncEnergy); dt.addElement(parent, "koefIncEnergy", str_val, typeid(NType).name());
    str_val = to_string(this->decSpeedLearning); dt.addElement(parent, "decSpeedLearning", str_val, typeid(NType).name());
    str_val = to_string(this->incSpeedLearning); dt.addElement(parent, "incSpeedLearning", str_val, typeid(NType).name());
    str_val = to_string(this->speedRegularization); dt.addElement(parent, "speedRegularization", str_val, typeid(NType).name());
    str_val = to_string(this->inertia); dt.addElement(parent, "inertia", str_val, typeid(NType).name());
    str_val = to_string(this->maxEpoches); dt.addElement(parent, "maxEpoches", str_val, typeid(int).name());
    str_val = to_string(this->maxGame); dt.addElement(parent, "maxGame", str_val, typeid(int).name());
    str_val = to_string(this->countBath); dt.addElement(parent, "countBath", str_val, typeid(int).name());
    str_val = to_string(this->stepExmBath); dt.addElement(parent, "stepExmBath", str_val, typeid(int).name());
    str_val = to_string(this->minError); dt.addElement(parent, "minError", str_val, typeid(NType).name());
    str_val = to_string(this->minErrorChange); dt.addElement(parent, "minErrorChange", str_val, typeid(NType).name());
    str_val = to_string(this->stepReg); dt.addElement(parent, "stepReg", str_val, typeid(int).name());
    str_val = to_string(this->stepDropout); dt.addElement(parent, "stepDropout", str_val, typeid(int).name());
    str_val = to_vstring(this->mixtrain); dt.addElement(parent, "mixtrain", str_val, typeid(bool).name());
    str_val = to_vstring(this->mixtest); dt.addElement(parent, "mixtest", str_val, typeid(bool).name());
    str_val = to_vstring(this->blState); dt.addElement(parent, "blState", str_val, typeid(bool).name());
    str_val = to_string(this->minErrorTest); dt.addElement(parent, "minErrorTest", str_val, typeid(NType).name());
    str_val = to_vstring(this->blErrorTest); dt.addElement(parent, "blErrorTest", str_val, typeid(bool).name());
    str_val = to_vstring(this->blDropout); dt.addElement(parent, "blDropout", str_val, typeid(bool).name());
    str_val = to_vstring(this->blDropoutExm); dt.addElement(parent, "blDropoutExm", str_val, typeid(bool).name());
    str_val = to_vstring(this->isGame); dt.addElement(parent, "isGame", str_val, typeid(bool).name());
    str_val = to_string(this->optType); dt.addElement(parent, "optType", str_val, typeid(NType).name());

    str_val = to_string(this->epoches); dt.addElement(parent, "epoches", str_val, typeid(int).name());
    str_val = to_string(this->iterGame); dt.addElement(parent, "iterGame", str_val, typeid(int).name());
    str_val = to_string(this->stabEnergy); dt.addElement(parent, "stabEnergy", str_val, typeid(int).name());
    str_val = to_string(this->changeEnergy); dt.addElement(parent, "changeEnergy", str_val, typeid(NType).name());
    to_array_string(str_vec, this->energyAver); dt.addElement(parent, "energyAver", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyMax); dt.addElement(parent, "energyMax", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyTestAver); dt.addElement(parent, "energyTestAver", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyTestMax); dt.addElement(parent, "energyTestMax", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->speedLearningHistory); dt.addElement(parent, "speedLearningHistory", str_vec, typeid(NType).name());
    //to_array_string(str_vec, this->scores); dt.addElement(parent, "scores", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->optParam); dt.addElement(parent, "optParam", str_vec, typeid(NType).name());

    to_matrix_string(str_mtrx, this->state); dt.addElement(parent, "state", str_mtrx, typeid(NType).name());

    str_val = to_string(this->stepExmTime); dt.addElement(parent, "stepExmTime", str_val, typeid(int).name());
    str_val = to_string(this->countTime); dt.addElement(parent, "countTime", str_val, typeid(int).name());
    str_val = to_vstring(this->clearNet); dt.addElement(parent, "clearNet", str_val, typeid(bool).name()); 
    str_val = to_vstring(this->blShift); dt.addElement(parent, "blShift", str_val, typeid(bool).name());
}

template <typename NType>
void TrainBP<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;

    TrainBP<NType>::deinit();

    if(dt.isOneMatrix())
    {
        iter = dt.modules[0];
    }
    else
    {
        size_t ind = dt.getShift();
        iter = dt.modules[ind];
        if(iter->cmpPath(parent, true)) {ind++;}

        while(dt.isNextBlock(ind, parent))
        {
            iter = dt.modules[ind];

                 if(iter->getFieldValue(parent, "speedLearningInit", str_val)) {to_value(this->speedLearningInit, str_val);}
            else if(iter->getFieldValue(parent, "speedLearningMax", str_val)) {to_value(this->speedLearningMax, str_val);}
            else if(iter->getFieldValue(parent, "koefIncEnergy", str_val)) {to_value(this->koefIncEnergy, str_val);}
            else if(iter->getFieldValue(parent, "decSpeedLearning", str_val)) {to_value(this->decSpeedLearning, str_val);}
            else if(iter->getFieldValue(parent, "incSpeedLearning", str_val)) {to_value(this->incSpeedLearning, str_val);}
            else if(iter->getFieldValue(parent, "speedRegularization", str_val)) {to_value(this->speedRegularization, str_val);}
            else if(iter->getFieldValue(parent, "inertia", str_val)) {to_value(this->inertia, str_val);}
            else if(iter->getFieldValue(parent, "maxEpoches", str_val)) {to_value(this->maxEpoches, str_val);}
            else if(iter->getFieldValue(parent, "maxGame", str_val)) {to_value(this->maxGame, str_val);}
            else if(iter->getFieldValue(parent, "countBath", str_val)) {to_value(this->countBath, str_val);}
            else if(iter->getFieldValue(parent, "stepExmBath", str_val)) {to_value(this->stepExmBath, str_val);}
            else if(iter->getFieldValue(parent, "minError", str_val)) {to_value(minError, str_val);}
            else if(iter->getFieldValue(parent, "minErrorChange", str_val)) {to_value(this->minErrorChange, str_val);}
            else if(iter->getFieldValue(parent, "stepReg", str_val)) {to_value(this->stepReg, str_val);}
            else if(iter->getFieldValue(parent, "stepDropout", str_val)) {to_value(this->stepDropout, str_val);}
            else if(iter->getFieldValue(parent, "mixtrain", str_val)) {to_value(this->mixtrain, str_val);}
            else if(iter->getFieldValue(parent, "mixtest", str_val)) {to_value(this->mixtest, str_val);}
            else if(iter->getFieldValue(parent, "blState", str_val)) {to_value(this->blState, str_val);}
            else if(iter->getFieldValue(parent, "minErrorTest", str_val)) {to_value(minErrorTest, str_val);}
            else if(iter->getFieldValue(parent, "blErrorTest", str_val)) {to_value(this->blErrorTest, str_val);}
            else if(iter->getFieldValue(parent, "blDropout", str_val)) {to_value(this->blDropout, str_val);}
            else if(iter->getFieldValue(parent, "blDropoutExm", str_val)) {to_value(this->blDropoutExm, str_val);}
            else if(iter->getFieldValue(parent, "isGame", str_val)) {to_value(this->isGame, str_val);}
            else if(iter->getFieldValue(parent, "optType", str_val)) {to_value(this->optType, str_val);}

            else if(iter->getFieldValue(parent, "epoches", str_val)) {to_value(this->epoches, str_val);}
            else if(iter->getFieldValue(parent, "iterGame", str_val)) {to_value(this->iterGame, str_val);}
            else if(iter->getFieldValue(parent, "stabEnergy", str_val)) {to_value(this->stabEnergy, str_val);}
            else if(iter->getFieldValue(parent, "changeEnergy", str_val)) {to_value(this->changeEnergy, str_val);}
            else if(iter->getFieldValue(parent, "energyAver", str_vec)) {to_array_value(this->energyAver, str_vec);}
            else if(iter->getFieldValue(parent, "energyMax", str_vec)) {to_array_value(this->energyMax, str_vec);}
            else if(iter->getFieldValue(parent, "energyTestAver", str_vec)) {to_array_value(this->energyTestAver, str_vec);}
            else if(iter->getFieldValue(parent, "energyTestMax", str_vec)) {to_array_value(this->energyTestMax, str_vec);}
            else if(iter->getFieldValue(parent, "speedLearningHistory", str_vec)) {to_array_value(this->speedLearningHistory, str_vec);}
            //else if(iter->getFieldValue(parent, "scores", str_vec)) {to_array_value(this->scores, str_vec);}
            else if(iter->getFieldValue(parent, "optParam", str_vec)) {to_array_value(this->optParam, str_vec);}

            else if(iter->getFieldValue(parent, "state", str_mtrx)) {to_matrix_value(this->state, str_mtrx);}

            else if(iter->getFieldValue(parent, "stepExmTime", str_val)) {to_value(this->stepExmTime, str_val);}
            else if(iter->getFieldValue(parent, "countTime", str_val)) {to_value(this->countTime, str_val);}
            else if(iter->getFieldValue(parent, "clearNet", str_val)) {to_value(this->clearNet, str_val);}
            else if(iter->getFieldValue(parent, "blShift", str_val)) {to_value(this->blShift, str_val);}

            ind++;
        }

        dt.setShift(ind);
    }
}

#endif // TRAINBP_H
