#ifndef TRAINBP_H
#define TRAINBP_H

#include "ineuronet.h"
#include "iobjectecsv.h"
#include "nfilesconstruct.h"

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

protected:
    NType speedLearningInit; //Обучение: первоначальная скорость обучения
    NType speedLearningMax; //Обучение: максимальная скорость обучения
    NType speedRegularization; //Обучение: скорость регуляризации
    NType inertia; //Обучение: инерция
    NType koefIncEnergy; //Коэффициент допустимого прироста погрешности изменения энергии
    NType decSpeedLearning; //Коэффициент уменьшения скорости обучения
    NType incSpeedLearning; //Коэффициент увеличения скорости обучения
    int maxEpoches; //Обучение: максимальное количество эпох
    NType minError; //Обучение: минимальная ошибка обучающего множества
    NType minErrorChange; //Обучение: минимальное изменение энергии
    int epoches; //Количество эпох
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

    int stepExmTime; //Обучение Time: Шаг взятия примеров
    int countTime; //Обучение Time: Количество обучений нейросети Time
    bool clearNet; //Обучение Time: Каждый раз очищать нейросеть
    bool blShift; //Обучение Time: Будут ли смещаться тестовые/обучающие типы примеров вместе с окном при множественном обучении?

private:
    NArray<NType> backOut; //Производная целевой функции или вектор, рассчитанный по дельта-правилу
    NMatrix<NType> changeWeight1, changeWeight2;
    NArray<NType> changeBias1, changeBias2;
    NType speedLearning; //Скорость обучения
    NType koefTrain; //Коэффициент обучения, агрегирующий другие коэффициенты
    NType koefInert; //Коэффициент инерции
    NType koefRegular; //Коэффициент регуляризации
    NType koefDropout; //Коэффициент dropout
    INLayer<NType>** pLay; //net.lay
    NExample<NType>** pExam; //net.examples
    NArray<NType>** pMas; //errorNet
    INLayer<NType>** pChangeLay; //changeNet.lay
    NType* pOutrun; //exm.outrun
    int lenLay; //net.lay - Количество слоёв
    int pos; //Счётчик примеров

public:
    void setSpeedLearningInit(NType param);
    void setKoefIncEnegry(NType param);
    void setDecSpeedLearning(NType param);
    void setIncSpeedLearning(NType param);
    void setSpeedRegularization(NType param);
    void setInertia(NType param);
    void setMaxEpoches(NType param);
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
    virtual void train(); //Процесс обучения
    virtual void trainTime();
    virtual void calculateError(NExample<NType>* exm); //Расчёт ошибок обучения
    virtual void calculateLock(); //Блокировка нейронов от изменения
    virtual void change(NExample<NType>* exm); //Расчёт изменений весов
    virtual void doMaskLock(); //Блокировка нейронов от изменения
    virtual void update(); //Принятие изменений
    void createDB(); //Создать БД
    void openDB(); //Открыть файлы БД
    void appendDB(); //Дописать в конец БД
    void closeDB(); //Закрыть файлы БД
    void fieldsDB(); //Выводимые поля БД

protected:
    void makePoint(); //Добавить отладочную информацию

public:
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
    this->minError = 0;
    this->minErrorChange = 0;
    this->epoches = 0;
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
    this->stepExmTime = 1;
    this->countTime = 1;
    this->clearNet = true;
    this->blShift = true;
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
void TrainBP<NType>::setMaxEpoches(NType param)
{
    this->maxEpoches = param;
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
    this->net->examples->srand();
    this->path.init();

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
}

template <typename NType>
void TrainBP<NType>::train()
{
    NExample<NType>* exm;
    NType prevEnrg = 0, curEnrg = 0, curEnrgTest = 0;
    bool isPoint;
    bool blLock = this->net->blLock;
    int beginset = this->net->examples->getBeginset();
    int testset = this->net->examples->getTestset();

    //Прореживание примеров
    this->net->examples->initUsage(NSetType::NSetTrain);
    this->net->examples->thinnigExamples(this->net->getShiftout());

    //Начальные данные
    this->speedLearning = this->speedLearningInit;
    this->koefTrain = this->speedLearning * (1 - this->inertia);
    this->koefInert = this->speedLearning * this->inertia;
    this->koefRegular = - (this->speedRegularization * this->koefTrain) / this->net->examples->getLenExamples(NSetType::NSetTrain);
    this->pLay = this->net->lay.getData();
    this->pExam = this->net->examples->getData();
    this->pMas = this->errorNet.getData();
    this->pChangeLay = this->changeNet->lay.getData();
    this->lenLay = this->net->lay.getLength();

    this->net->runExamples(NSetType::NSetTrain);
    this->net->examples->getGroupEnergy(this->geTrain);
    prevEnrg = this->net->examples->getEnergyAver();
    curEnrg = this->net->examples->getEnergyAver();
    this->changeEnergy = prevEnrg;

    if(this->blErrorTest)
    {
        this->net->runExamples(NSetType::NSetTest);
        this->net->examples->getGroupEnergy(this->geTest);
        curEnrgTest = this->net->examples->getEnergyAver();
    }

    if(this->stepReg != 0) {this->makePoint();}
    if(blDropout)
    {
        if(this->stepDropout == 0) {this->stepDropout = this->maxEpoches + 1;}
        this->net->init_slay();
    }

    //Цикл обучения
    while(this->epoches < this->maxEpoches
          && curEnrg > this->minError
          && (!this->blErrorTest || curEnrgTest > this->minErrorTest)
          && this->changeEnergy >= this->minErrorChange)
    {
        while(this->epoches < this->maxEpoches
            && curEnrg > this->minError
            && (!this->blErrorTest || curEnrgTest > this->minErrorTest)
            && this->changeEnergy >= this->minErrorChange)
        {
            if(this->mixtrain)
            {
                this->net->examples->mixTrain();
                this->net->examples->initUsage(NSetType::NSetTrain);
                this->net->examples->thinnigExamples(this->net->getShiftout()); //Прореживание примеров
            }

            if(blDropout)
            {
                for(pos = beginset; pos < testset; pos++)
                {
                    exm = this->pExam[pos];
                    //cout << "Primer: " << pos << "\n"; //Тест
                    if(exm->getUsage())
                    {
                        this->net->runExampleDropout(exm);
                        calculateError(exm);
                        change(exm);
                        if(blLock) {doMaskLock();}
                        update();
                    }
                }
            }
            else
            {
                for(pos = beginset; pos < testset; pos++)
                {
                    exm = this->pExam[pos];
                    //cout << "Primer: " << pos << "\n"; //Тест
                    if(exm->getUsage())
                    {
                        this->net->runExample(exm);
                        //cout << "Energy: " << exm->getEnergy() << "\n"; //Тест
                        calculateError(exm);
                        change(exm);
                        if(blLock) {doMaskLock();}
                        update();
                    }
                }
            }

            this->epoches++;
            isPoint = (this->stepReg != 0)&&(this->epoches % this->stepReg == 0);

            this->net->runEnergy(NSetType::NSetTrain);
            if(isPoint) {this->net->examples->getGroupEnergy(this->geTrain);}
            curEnrg = this->net->examples->getEnergyAver();
            this->changeEnergy = prevEnrg - curEnrg;
            if(this->changeEnergy < 0) {this->changeEnergy = -this->changeEnergy;}

            //Алгоритм изменения скорости обучения
            this->speedLearning *= (curEnrg > this->koefIncEnergy * prevEnrg ? this->decSpeedLearning : this->incSpeedLearning);
            if(this->speedLearning > this->speedLearningMax) {this->speedLearning = this->speedLearningMax;}
            this->koefTrain = this->speedLearning * (1 - this->inertia);
            this->koefInert = this->speedLearning * this->inertia;
            this->koefRegular = - (this->speedRegularization * this->koefTrain) / this->net->examples->getLenExamples(NSetType::NSetTrain);
            prevEnrg = curEnrg;

            if(this->blErrorTest)
            {
                this->net->runExamples(NSetType::NSetTest);
                if(isPoint) {this->net->examples->getGroupEnergy(this->geTest);}
                curEnrgTest = this->net->examples->getEnergyAver();
            }

            if(isPoint) {this->makePoint();}
            if(blDropout && (epoches % stepDropout == 0)) {this->net->init_slay();}
        }

        this->stabEnergy++;

        this->net->runExamples(NSetType::NSetTrain);
        curEnrg = this->net->examples->getEnergyAver();
        this->changeEnergy = prevEnrg - curEnrg;
        if(this->changeEnergy < 0) {this->changeEnergy = -this->changeEnergy;}
        prevEnrg = curEnrg;
    }

    this->net->examples->getGroupEnergy(this->geTrain);
    if(this->blErrorTest)
    {
        this->net->runEnergy(NSetType::NSetTest);
    }
    else
    {
        this->net->runExamples(NSetType::NSetTest);
    }
    this->net->examples->getGroupEnergy(this->geTest);
    this->net->examples->setGroupEnergy(this->geTrain);

    if(this->stepReg != 0) {this->makePoint();}
}

template <typename NType>
void TrainBP<NType>::trainTime()
{
    this->createDB();
    this->openDB();

    int begset = this->net->examples->getBeginset();
    //this->net->examples->update();
    if(this->mixtest) {this->net->examples->mixTest();}
    this->net->examples->initTypeSet();
    this->fieldsDB();

    for(int tm = 0; tm < this->countTime; tm++)
    {
        if(this->clearNet)
        {
            if(this->net->getTypeInit() == NInitType::NInitEmpty) {this->net->init();}
            else if(this->net->getTypeInit() == NInitType::NInitG) {this->net->ginit();}
        }

        this->reset();
        if(this->blShift) {this->net->examples->mixTypeSet();}

        this->train();

        this->net->runExamples(NSetType::NSetValidate);
        //if(!this->net->examples->getBlOriginal()) {this->net->examples->recovery();}
        this->net->examples->postrun();
        this->net->examples->runEnergyClass(NSetType::NSetTrain);

        //if(tm == 1) {this->fieldsDB();}
        this->appendDB();

        this->net->examples->doShift(this->stepExmTime, this->blShift);
        if(!this->blShift)
        {
            if(this->mixtest) {this->net->examples->mixTest();}
            this->net->examples->initTypeSet();
        }
    }

    this->net->examples->portion(begset);
    this->closeDB();
}

template <typename NType>
void TrainBP<NType>::calculateError(NExample<NType>* exm)
{
    int k;

    //Тест
    /*NArray<NType>* outtst;
    outtst = &this->net->lay[0]->output; cout << "outrun=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[1]->output; cout << "outrun=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[2]->output; cout << "outrun=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[0]->sum; cout << "sum=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[1]->sum; cout << "sum=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";
    outtst = &this->net->lay[2]->sum; cout << "sum=" << outtst->get(0) << ", " << outtst->get(1) << ", " << outtst->get(2) << "\n";*/
    //cout << "outrun=" << exm->outrun.get(0) << ", " << exm->outrun.get(1) << ", " << exm->outrun.get(2) << "\n";}

    // Выходной слой
    NArray<NType>& er = this->net->derivEnergy(exm);
    NArray<NType>* outlay = this->net->derivActivLay(exm);

    k = lenLay-1;
    pLay[k]->derivRun(pMas[k]);
    if(blDropout) {pMas[k]->mul(pLay[k]->slay);}
    if(outlay != nullptr) {pMas[k]->mul(*outlay);}
    pMas[k]->mul(er);
    //cout << "d=" << pMas[k]->get(0) << ", " << pMas[k]->get(1) << ", " << pMas[k]->get(2) << "\n"; //Тест

    // Скрытый слой
    for(k = lenLay-2; k >= 0; k--)
    {
        pLay[k]->derivRun(pMas[k]);
        if(blDropout) {pMas[k]->mul(pLay[k]->slay);}
        this->backOut.mul(*pMas[k+1], pLay[k+1]->weigth, false);
        pMas[k]->mul(this->backOut);
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
void TrainBP<NType>::change(NExample<NType>* exm)
{
    // Входной слой weight
    koefDropout = (blDropout ? pLay[0]->getKoefDropout() : 1);
    this->changeWeight1.mul(exm->input, *pMas[0]);
    this->changeWeight1.valmul(koefTrain * koefDropout);

    this->changeWeight2.copyValue(pChangeLay[0]->weigth);
    this->changeWeight2.valmul(koefInert * koefDropout);
    this->changeWeight2.sum(this->changeWeight1);

    this->changeWeight1.copyValue(pLay[0]->weigth);
    this->changeWeight1.valmul(koefRegular * koefDropout);
    pChangeLay[0]->weigth.sum(this->changeWeight1, this->changeWeight2);

    // Скрытый слой weight
    for(int k = 1; k < this->net->lay.getLength(); k++)
    {
        koefDropout = (blDropout ? pLay[k]->getKoefDropout() : 1);
        this->changeWeight1.mul(pLay[k-1]->output, *pMas[k]);
        this->changeWeight1.valmul(koefTrain * koefDropout);

        this->changeWeight2.copyValue(pChangeLay[k]->weigth);
        this->changeWeight2.valmul(koefInert * koefDropout);
        this->changeWeight2.sum(this->changeWeight1);

        this->changeWeight1.copyValue(pLay[k]->weigth);
        this->changeWeight1.valmul(koefRegular * koefDropout);
        pChangeLay[k]->weigth.sum(this->changeWeight1, this->changeWeight2);
    }

    // Cлой bias
    for(int k = 0; k < this->net->lay.getLength(); k++)
    {
        koefDropout = (blDropout ? pLay[k]->getKoefDropout() : 1);
        this->changeBias1.copyValue(*pMas[k]);
        this->changeBias1.valmul(koefTrain * koefDropout);

        this->changeBias2.copyValue(pChangeLay[k]->bias);
        this->changeBias2.valmul(koefInert * koefDropout);
        this->changeBias2.sum(this->changeBias1);

        this->changeBias1.copyValue(pLay[k]->bias);
        this->changeBias1.valmul(koefRegular * koefDropout);
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
    this->net->examples->saveECSV(path.dt_examples, parentExamples);
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
void TrainBP<NType>::makePoint()
{
    //this->energyAver.push(this->net->examples->getEnergyAver());
    //this->energyMax.push(this->net->examples->getEnergyMax());
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
        //this->energyTestAver.push(this->net->examples->getEnergyAver());
        //this->energyTestMax.push(this->net->examples->getEnergyMax());
        this->energyTestAver.push(this->geTest.energyAver);
        this->energyTestMax.push(this->geTest.energyMax);
    }
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

    str_val = to_string(this->epoches); dt.addElement(parent, "epoches", str_val, typeid(int).name());
    str_val = to_string(this->stabEnergy); dt.addElement(parent, "stabEnergy", str_val, typeid(int).name());
    str_val = to_string(this->changeEnergy); dt.addElement(parent, "changeEnergy", str_val, typeid(NType).name());
    to_array_string(str_vec, this->energyAver); dt.addElement(parent, "energyAver", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyMax); dt.addElement(parent, "energyMax", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyTestAver); dt.addElement(parent, "energyTestAver", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->energyTestMax); dt.addElement(parent, "energyTestMax", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->speedLearningHistory); dt.addElement(parent, "speedLearningHistory", str_vec, typeid(NType).name());
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

            else if(iter->getFieldValue(parent, "epoches", str_val)) {to_value(this->epoches, str_val);}
            else if(iter->getFieldValue(parent, "stabEnergy", str_val)) {to_value(this->stabEnergy, str_val);}
            else if(iter->getFieldValue(parent, "changeEnergy", str_val)) {to_value(this->changeEnergy, str_val);}
            else if(iter->getFieldValue(parent, "energyAver", str_vec)) {to_array_value(this->energyAver, str_vec);}
            else if(iter->getFieldValue(parent, "energyMax", str_vec)) {to_array_value(this->energyMax, str_vec);}
            else if(iter->getFieldValue(parent, "energyTestAver", str_vec)) {to_array_value(this->energyTestAver, str_vec);}
            else if(iter->getFieldValue(parent, "energyTestMax", str_vec)) {to_array_value(this->energyTestMax, str_vec);}
            else if(iter->getFieldValue(parent, "speedLearningHistory", str_vec)) {to_array_value(this->speedLearningHistory, str_vec);}
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
