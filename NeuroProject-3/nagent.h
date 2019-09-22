#ifndef NAGENT_H
#define NAGENT_H

#include "trainbp.h"

template <typename NType>
class NAgent: public TrainBP<NType>
{
public:
    NAgent();

public:
    NType epsilon; //Вероятность случайных действий
    NType epsilon_init;
    NType epsilon_decay; //Коэффициент уменьшения вероятности случайных действий
    NType epsilon_min; //Минимальный порог вероятности случайных действий
    NType gamma; //Коэффициент расределения награды в уравнении Беллмана
    NType alfa; //Коэффициент обновления цели
    NArray<int> actions; //Выбранное действие под каждый пример
    //NArray<NExample<NType>*> memoryExm; //Последовательность примеров для действий
    NArray<NType> rewards; //Соответствующие вознаграждения агента
    NArray<NType> targets; //Распределённое вознаграждение (1-alfa) * state + alfa * (rewards + gamma * max(next_state))
    NArray<bool> dones; //Конечные примеры (неудачи)?
    NArray<NType> angles; //Агрегируемые значения входа
    NArray<NType> scores; //Количество итераций в игре
    int iterScore;
    bool end_done; //Заканчивать эпоху при первой неудаче?
    bool blOutput; //Использовать эталонный вектор для обучения

protected:
    bool isOneOut; //В нейросети 1 выход

protected:
    virtual void makePoint(); //Добавить отладочную информацию
    NType agentTarget(NExample<NType>* state, int action, NType reward, NExample<NType>* next_state, bool done, int pos); //Функция вознаграждения агента
    virtual int act(NExample<NType>* exm); //Функция выбора действия
    NType calcReward(int action, bool& done); //Подсчёт вознаграждения
    virtual void train_epoch(int beginset, int testset); //1 эпоха обучения
    int train_calcstate(int beginset, int testset, bool is_end);
    void train_calcbath(int beginset, int shiftBath, int pos);
    NType train_calctarget(int beginset, int shiftBath, int pos);
    bool end_epoch(); //Условие на окончание эпохи
    virtual void calcKoefsAgent(); //Пересчёт коэффициентов агента
    virtual void train_initparam(NType& curEnrg); //Инициализация параметров обучения
    void addAngle();

public:
    void createTestFile(const string& name);
    void appendTestFile(const string& name);
    void saveTest(DataECSV& dt, string& parent);
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};

template <typename NType>
NAgent<NType>::NAgent()
{
    this->epsilon = 0;
    this->epsilon_init = 0;
    this->epsilon_decay = 0;
    this->epsilon_min = 0;
    this->gamma = 0;
    this->alfa = 1;
    this->isOneOut = false;
    this->end_done = false;
    this->blOutput = false;
}

template <typename NType>
void NAgent<NType>::makePoint()
{
    string testFileName = this->path.current_dir + "\\" + "nagent.csv"; //Тест
    bool isTest = this->net->tmp_examples->getIsTest();

    TrainBP<NType>::makePoint();

    if(isTest && this->epoches > 0) {this->appendTestFile(testFileName);} //Тест
}

template <typename NType>
NType NAgent<NType>::agentTarget(NExample<NType>* state, int action, NType reward, NExample<NType>* next_state, bool done, int pos)
{
    NType target = 0;
    action = this->net->getShiftout() + (isOneOut ? 0 : action);
    //int next_action = this->net->getShiftout() + (isOneOut ? 0 : this->actions[pos+1]);
    if(!done) {target = this->net->maxOut(next_state->outrun);}
    //if(!done) {target = next_state->outrun[next_action];}
    //if(!done) {target = this->targets[pos+1];}
    //target = abs(state->input[0]);
    target = (1 - this->alfa) * state->outrun[action] + this->alfa * (reward + this->gamma * target);
    this->targets[pos] = target;
    return target;
}

template <typename NType>
int NAgent<NType>::act(NExample<NType>* exm)
{
    int action;
    uniform_real_distribution<> distribution(0, 1);

    if(distribution(NRandGenerator) < this->epsilon)
    {
        uniform_int_distribution<> distributionAct(0, (isOneOut ? 1 : this->net->getLenOut()-1));
        action = distributionAct(NRandGenerator);
    }
    else
    {
        this->net->runExample(exm);
        action = this->net->actNet(exm->outrun);
    }

    this->actions.push(action);
    //this->memoryExm.push(exm);
    return action;
}

template <typename NType>
NType NAgent<NType>::calcReward(int action, bool& done)
{
    NType reward = this->net->tmp_examples->stateReward(this->net->getShiftout(), (isOneOut ? 0 : action), done);

    this->rewards.push(reward);
    this->dones.push(done);
    return reward;
}

//template <typename NType>
//void NAgent<NType>::train_epoch(int beginset, int testset)
//{
//    int pos = 0;
//    NExample<NType>* tmp_exm;
//    NExample<NType>* next_exm;
//    bool blLock = this->net->blLock;
//    int shift = this->net->getShiftout();
//    int action;
//    NType target;
//    int lenBath;
//    bool isTest = this->net->tmp_examples->getIsTest();

//    testset -= this->countBath;
//    this->actions.clear();
//    //this->memoryExm.clear();
//    this->rewards.clear();
//    this->targets.clear();
//    this->dones.clear();
//    this->angles.clear();

//    for(pos = 0; pos < this->countBath + 1 && beginset < testset; pos++)
//    {
//        action = this->act(this->pExam[beginset]);
//        this->calcReward(action, beginset >= (testset-1));
//        if(isTest) {this->addAngle();} //Тест
//        this->net->tmp_examples->nextState(action, pos);
//        beginset++;
//    }

//    while(beginset < testset && !end_epoch())
//    {
//        lenBath = actions.getLength() - 1;

//        for(pos = lenBath - this->countBath; pos < lenBath; pos++)
//        {
//            tmp_exm = this->pExam[pos];
//            next_exm = this->pExam[pos+1];
//            this->net->runExample(next_exm);
//            if(this->blDropout)
//            {
//                if(this->blDropoutExm) {this->net->init_slay();}
//                this->net->runExampleDropout(tmp_exm);
//            }
//            else
//            {
//                this->net->runExample(tmp_exm);
//            }
//            action = shift + (isOneOut ? 0 : actions[pos]);
//            target = agentTarget(tmp_exm, action, rewards[pos], next_exm, dones[pos], pos);
//            this->targets.push(target);
//            if(!this->blOutput)
//            {
//                tmp_exm->output = tmp_exm->outrun;
//                tmp_exm->output[action] = target;
//            }
//            this->calculateError(tmp_exm);
//            this->change(tmp_exm);
//            if(blLock) {this->doMaskLock();}
//            this->update();
//        }

//        this->calcKoefsAgent();

//        for(pos = lenBath; pos < lenBath + this->stepExmBath && beginset < testset; pos++)
//        {
//            action = this->act(this->pExam[beginset]);
//            this->calcReward(action, beginset >= (testset-1));
//            if(isTest) {this->addAngle();} //Тест
//            this->net->tmp_examples->nextState(action, pos);
//            beginset++;
//        }
//    }

//    this->epoches++;
//}

template <typename NType>
void NAgent<NType>::train_epoch(int beginset, int testset)
{
    int pos = 0;
    int bathset;
    int bathcount = this->countBath;
    bool isOnline = this->net->tmp_examples->getIsOnline();
    int shiftBath = 0;

    this->iterScore = 0;
    testset -= this->countBath;
    this->actions.clear();
    //this->memoryExm.clear();
    this->rewards.clear();
    this->targets.clear();
    this->dones.clear();
    this->angles.clear();
    this->scores.clear();

    while(beginset < testset && !end_epoch() && bathcount >= this->stepExmBath && (!this->isGame || this->iterGame < this->maxGame))
    {
        //Онлайн обучение
        if(isOnline)
        {
            bathcount = this->train_calcstate(beginset, testset, false);
        }

        //Мини-батч
        //for(pos = bathcount-1; pos >= 0; pos--)
        for(pos = 0; pos < bathcount; pos++)
        {
            this->train_calcbath(beginset, shiftBath, pos);
        }

        beginset += this->stepExmBath;
        shiftBath += this->stepExmBath;

        //Онлайн обучение (обновление)
        if(isOnline)
        {
            this->calcKoefsAgent();
            bathset = bathcount - this->stepExmBath;
            if(bathset > 0)
            {
                this->actions.clip(bathset);
                this->rewards.clip(bathset);
                this->targets.clip(bathset);
                this->dones.clip(bathset);
                this->angles.clip(bathset);
            }
            else if(bathset < 0) {break;}
        }
    }

    //Онлайн обучение
    if(isOnline)
    {
        bathcount = this->train_calcstate(beginset, testset, true);

        //Мини-батч
        //for(pos = bathcount-1; pos >= 0; pos--)
        for(pos = 0; pos < bathcount; pos++)
        {
            this->train_calctarget(beginset, shiftBath, pos);
        }
    }

    this->epoches++;
}

template <typename NType>
int NAgent<NType>::train_calcstate(int beginset, int testset, bool is_end)
{
    NExample<NType>* tmp_exm;
    int action;
    bool isTest = this->net->tmp_examples->getIsTest();
    //bool done = (this->dones.getLength() > 0 ? this->dones.endElement() : false);
    bool done = false;
    int bathcount = 0;
    int bathset = (is_end ? testset : beginset) + this->countBath;

    this->net->tmp_examples->beginState(beginset);
    for(int pos = beginset; pos < bathset && !end_epoch(); pos++)
    {
        if(is_end) {done = done || pos >= (bathset-1);}
        tmp_exm = this->pExam[pos];
        action = this->act(tmp_exm);
        this->calcReward(action, done);
        if(isTest) {this->addAngle();} //Тест
        this->net->tmp_examples->nextState(action, pos);
        this->iterScore++;
        if(done)
        {
            this->scores.push(this->iterScore);
            this->iterScore = 0;
            this->iterGame++;
            done = false;
            if(this->isGame && this->iterGame >= this->maxGame)
            {
                bathcount++; break;
            }
        }
        bathcount++;
    }
    this->targets.extend(bathcount, 0);
    return bathcount;
}

template <typename NType>
void NAgent<NType>::train_calcbath(int beginset, int shiftBath, int pos)
{
    NExample<NType>* tmp_exm = this->pExam[beginset+pos];
    NExample<NType>* next_exm = this->pExam[beginset+pos+1];

    this->net->runExample(next_exm);
    if(this->blDropout)
    {
        if(this->blDropoutExm) {this->net->init_slay();}
        this->net->runExampleDropout(tmp_exm);
    }
    else
    {
        this->net->runExample(tmp_exm);
    }

    //NType target = this->agentTarget(tmp_exm, action, rewards[shiftBath+pos], next_exm, dones[shiftBath+pos], shiftBath+pos);
    //target = abs(tmp_exm->input[0]);
    NType target = this->train_calctarget(beginset, shiftBath, pos);

    if(!this->blOutput)
    {
        int action = actions[shiftBath+pos];
        //int antiaction = (action == 0 ? 1 : 0);
        tmp_exm->output = tmp_exm->outrun;
        tmp_exm->output[action] = target;
        //tmp_exm->output.init_value(0);
        //if(target > 0)
        //{
        //    tmp_exm->output[action] = target;
        //    tmp_exm->output[antiaction] = 0;
        //}
        //else {tmp_exm->output[antiaction] = target;}
    }

    this->calculateError(tmp_exm);
    this->change(tmp_exm);
    if(this->net->blLock) {this->doMaskLock();}
    this->update();
}

template <typename NType>
NType NAgent<NType>::train_calctarget(int beginset, int shiftBath, int pos)
{
    //NExample<NType>* tmp_exm = this->pExam[beginset+pos];
    //NExample<NType>* next_exm = this->pExam[beginset+pos+1];
    return this->agentTarget(this->pExam[beginset+pos], actions[shiftBath+pos], rewards[shiftBath+pos], this->pExam[beginset+pos+1], dones[shiftBath+pos], shiftBath+pos);
}

template <typename NType>
bool NAgent<NType>::end_epoch()
{
    return this->end_done && this->dones.getLength() > 0 && this->dones.endElement();
}

template <typename NType>
void NAgent<NType>::calcKoefsAgent()
{
    this->epsilon *= this->epsilon_decay;
    if(this->epsilon < this->epsilon_min) {this->epsilon = this->epsilon_min;}
}

template <typename NType>
void NAgent<NType>::train_initparam(NType& curEnrg)
{
    string testFileName = this->path.current_dir + "\\" + "nagent.csv"; //Тест
    bool isTest = this->net->tmp_examples->getIsTest();
    int ntrain = this->net->tmp_examples->getTestset() - this->net->tmp_examples->getBeginset() + 1;

    TrainBP<NType>::train_initparam(curEnrg);

    this->epsilon = this->epsilon_init;
    this->isOneOut = (this->net->getLenOut() == 1);

    this->actions.resize(ntrain);
    this->rewards.resize(ntrain);
    this->targets.resize(ntrain);
    this->dones.resize(ntrain);
    this->angles.resize(ntrain);

    if(isTest) {this->createTestFile(testFileName);} //Тест
}

template <typename NType>
void NAgent<NType>::addAngle()
{
    this->angles.push(this->net->tmp_examples->getAngleState());
}


template <typename NType>
void NAgent<NType>::createTestFile(const string& name)
{
    DataECSV dt;
    dt.create(name);
}

template <typename NType>
void NAgent<NType>::appendTestFile(const string& name)
{
    DataECSV dt;
    string parent = "train_epoch";
    this->saveTest(dt, parent);
    dt.appendObj(name);
}

template <typename NType>
void NAgent<NType>::saveTest(DataECSV& dt, string& parent)
{
    //NMatrix<string> str_mtrx;
    vector<string> str_vec;
    //string str_val;

    dt.addGroup(parent, "");
    to_array_string(str_vec, this->angles); dt.addElement(parent, "angles", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->actions); dt.addElement(parent, "actions", str_vec, typeid(int).name());
    to_array_string(str_vec, this->rewards); dt.addElement(parent, "rewards", str_vec, typeid(NType).name());
    to_array_string(str_vec, this->targets); dt.addElement(parent, "targets", str_vec, typeid(NType).name());
    to_array_vstring(str_vec, this->dones); dt.addElement(parent, "dones", str_vec, typeid(bool).name());
    to_array_string(str_vec, this->scores); dt.addElement(parent, "scores", str_vec, typeid(NType).name());
}

template <typename NType>
void NAgent<NType>::saveECSV(DataECSV& dt, string& parent)
{
    //NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;

    dt.addGroup(parent, "");
    str_val = to_string(this->epsilon_init); dt.addElement(parent, "epsilon_init", str_val, typeid(NType).name());
    str_val = to_string(this->epsilon_decay); dt.addElement(parent, "epsilon_decay", str_val, typeid(NType).name());
    str_val = to_string(this->epsilon_min); dt.addElement(parent, "epsilon_min", str_val, typeid(NType).name());
    str_val = to_string(this->gamma); dt.addElement(parent, "gamma", str_val, typeid(NType).name());
    str_val = to_string(this->alfa); dt.addElement(parent, "alfa", str_val, typeid(NType).name());
    str_val = to_vstring(this->end_done); dt.addElement(parent, "end_done", str_val, typeid(bool).name());
    str_val = to_vstring(this->blOutput); dt.addElement(parent, "blOutput", str_val, typeid(bool).name());

    TrainBP<NType>::saveECSV(dt, parent);
}

template <typename NType>
void NAgent<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    //NExample<NType>* exm;
    //NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;

    if(dt.isOneMatrix())
    {
        iter = dt.modules[0];
    }
    else
    {
        //Поля класса
        size_t ind = dt.getShift();
        size_t beginInd = ind;
        iter = dt.modules[ind];
        if(iter->cmpPath(parent, true)) {ind++;}

        while(dt.isNextBlock(ind, parent))
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "epsilon_init", str_val)) {to_value(this->epsilon_init, str_val);}
            else if(iter->getFieldValue(parent, "epsilon_decay", str_val)) {to_value(this->epsilon_decay, str_val);}
            else if(iter->getFieldValue(parent, "epsilon_min", str_val)) {to_value(this->epsilon_min, str_val);}
            else if(iter->getFieldValue(parent, "gamma", str_val)) {to_value(this->gamma, str_val);}
            else if(iter->getFieldValue(parent, "alfa", str_val)) {to_value(this->alfa, str_val);}
            else if(iter->getFieldValue(parent, "end_done", str_val)) {to_value(this->end_done, str_val);}
            else if(iter->getFieldValue(parent, "blOutput", str_val)) {to_value(this->blOutput, str_val);}

            ind++;
        }

        //Базовый класс
        dt.setShift(beginInd);
        TrainBP<NType>::loadECSV(dt, parent);
    }
}

#endif // NAGENT_H
