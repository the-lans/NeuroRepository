#ifndef NCARTPOLE_H
#define NCARTPOLE_H

#include "nexamples.h"

#define CP_GRAVITY 9.8
#define CP_MASSCART 1.0
#define CP_MASSPOLE 0.1
#define CP_LENGTH 0.5 //Actually half the pole's length
#define CP_FORCE_MAG 10.0
#define CP_TAU 0.02 //Seconds between state updates
#define CP_ANGLE_THRESHOLD (12 * 2 * M_PI / 360) //Angle at which to fail the episode
#define CP_X_THRESHOLD 2.4
#define CP_TOTAL_MASS (CP_MASSPOLE + CP_MASSCART)
#define CP_POLEMASS_LENGTH (CP_MASSPOLE * CP_LENGTH)

template <typename NType>
class NCartPole: public NExamples<NType>
{
public:
    NCartPole();
    NCartPole(NCartPole<NType>& obj);
    virtual ~NCartPole();

protected:
    NType koef_a; //Коэффициент падения
    NType koef_b; //Коэффициент передвижения
    NType angle_over; //Угол конца игры
    NType x; //Позиция тележки
    NType x_dot; //Скорость тележки
    NType angle; //Угол полюса
    NType angle_dot; //Скорость полюса

public:
    NType getKoef_a() const;
    void setKoef_a(const NType &value);
    NType getKoef_b() const;
    void setKoef_b(const NType &value);
    NType getAngle_over() const;
    void setAngle_over(const NType &value);
    NType getX() const;
    NType getX_dot() const;
    NType getAngle() const;
    NType getAngle_dot() const;

public:
    virtual NExamples<NType>* create(); //Создание объекта
    virtual void copyTmpExamples(NExamples<NType>* exms); //Копирование примеров для временного хранилища
    virtual void postrun(int shiftout); //Постобработка данных
    virtual void initState(); //Начальная инициализация примеров
    virtual void beginState(); //Начальное состояние-пример
    virtual void beginState(int pos);
    void paramToState(); //Преобразование параметров в состояние
    void paramToState(int pos);
    virtual void resetState(int pos); //Новая игра
    virtual void nextState(int action, int pos); //Следующее состояние-пример
    virtual NType stateReward(int shiftout, int action, bool& done); //Вознаграждение в текущем состоянии
    virtual bool isOver(int shiftout, int action); //Это конец игры?
    virtual void deinitState(); //Деинициализация состояний
    virtual NType getAngleState(); //Агрегирующее значение входа

public:
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NCartPole<NType>::NCartPole():
    NExamples<NType>()
{
    this->isOnline = true;
    this->koef_a = 1.2;
    this->koef_b = 0.05;
    this->angle_over = 1;
    this->x = 0;
    this->x_dot = 0;
    this->angle = 0;
    this->angle_dot = 0;
}

template <typename NType>
NCartPole<NType>::NCartPole(NCartPole<NType>& obj):
    NExamples<NType>(obj)
{
    this->isOnline = true;
    this->koef_a = obj.getKoef_a();
    this->koef_b = obj.getKoef_b();
    this->angle_over = obj.getAngle_over();
    this->x = 0;
    this->x_dot = 0;
    this->angle = 0;
    this->angle_dot = 0;
}

template <typename NType>
NCartPole<NType>::~NCartPole()
{
    this->deinit();
}


template <typename NType>
NType NCartPole<NType>::getKoef_a() const
{
    return koef_a;
}

template <typename NType>
void NCartPole<NType>::setKoef_a(const NType &value)
{
    koef_a = value;
}

template <typename NType>
NType NCartPole<NType>::getKoef_b() const
{
    return koef_b;
}

template <typename NType>
void NCartPole<NType>::setKoef_b(const NType &value)
{
    koef_b = value;
}

template <typename NType>
NType NCartPole<NType>::getAngle_over() const
{
    return this->angle_over;
}

template <typename NType>
void NCartPole<NType>::setAngle_over(const NType &value)
{
    this->angle_over = value;
}

template <typename NType>
NType NCartPole<NType>::getX() const
{
    return this->x;
}

template <typename NType>
NType NCartPole<NType>::getX_dot() const
{
    return this->x_dot;
}

template <typename NType>
NType NCartPole<NType>::getAngle() const
{
    return this->angle;
}

template <typename NType>
NType NCartPole<NType>::getAngle_dot() const
{
    return this->angle_dot;
}


template <typename NType>
NExamples<NType>* NCartPole<NType>::create()
{
    return (new NCartPole<NType>());
}

template <typename NType>
void NCartPole<NType>::copyTmpExamples(NExamples<NType>* exms)
{
    //NCartPole<NType>* exmz = (NCartPole<NType>*)exms;
    NCartPole<NType>* exmz = dynamic_cast<NCartPole*>(exms);
    NExamples<NType>::copyTmpExamples(exms);

    this->koef_a = exmz->getKoef_a();
    this->koef_b = exmz->getKoef_b();
    this->angle_over = exmz->getAngle_over();
}

template <typename NType>
void NCartPole<NType>::postrun(int /*shiftout*/)
{
    int pos;
    NType value;
    NExample<NType>* exm;

    //Заполнение вектора постобработки
    if(!this->blOriginal) {this->recovery();}

    for(pos = this->beginset; pos < this->endset; pos++)
    {
        exm = this->get(pos);
        exm->outpostrun.clear();
        value = exm->outrun.maxArg();
        exm->outpostrun.push(value);
    }
}

template <typename NType>
void NCartPole<NType>::initState()
{
    //this->prerun();
    this->resetState(this->beginset);
    this->filloutput();
    this->filloutrun();
    this->update();
}

template <typename NType>
void NCartPole<NType>::beginState()
{
    this->beginState(this->beginset);
}

template <typename NType>
void NCartPole<NType>::beginState(int pos)
{
    this->state = this->get(pos);
    this->x = this->state->input[0];
    this->x_dot = this->state->input[1];
    this->angle = this->state->input[2];
    this->angle_dot = this->state->input[3];
}

template <typename NType>
void NCartPole<NType>::paramToState()
{
    //Вход
    this->state->input.init(this->lenIn, 0);
    this->state->input[0] = this->x;
    this->state->input[1] = this->x_dot;
    this->state->input[2] = this->angle;
    this->state->input[3] = this->angle_dot;

    //Выход
    this->state->output.init(this->lenOut, 0);
    this->state->output[0] = (this->angle > 0 ? this->angle : 0);
    this->state->output[1] = (this->angle < 0 ? -this->angle : 0);
}

template <typename NType>
void NCartPole<NType>::paramToState(int pos)
{
    this->state = this->get(pos);
    this->paramToState();
}

template <typename NType>
void NCartPole<NType>::resetState(int pos)
{
    //Рандомные значения
    uniform_real_distribution<> distribution(this->inMin, this->inMax);
    this->x = this->funcNormIn(distribution(NRandGenerator));
    this->x_dot = this->funcNormIn(distribution(NRandGenerator));
    this->angle = this->funcNormIn(distribution(NRandGenerator));
    this->angle_dot = this->funcNormIn(distribution(NRandGenerator));

    this->paramToState(pos);
}

template <typename NType>
void NCartPole<NType>::nextState(int action, int pos)
{
    if(pos >= this->length - 1) {this->state = nullptr; return;}
    if((action != 0)&&(action != 1)) {throw("Not action!");}

    //if(this->dones.endElement())
    if(this->isOver(0, action))
    {
        this->resetState(pos+1);
    }
    else
    {
        this->beginState(pos);

        NType force = (action == 0 ? CP_FORCE_MAG : -CP_FORCE_MAG);
        NType cosangle = cos(this->angle);
        NType sinangle = sin(this->angle);
        NType temp = (force + CP_POLEMASS_LENGTH * this->angle_dot * this->angle_dot * sinangle) / CP_TOTAL_MASS;
        NType angle_acc = (CP_GRAVITY * sinangle - cosangle * temp) / (CP_LENGTH * (4.0/3.0 - CP_MASSPOLE * cosangle * cosangle / CP_TOTAL_MASS));
        NType x_acc = temp - CP_POLEMASS_LENGTH * angle_acc * cosangle / CP_TOTAL_MASS;

        this->x += CP_TAU * x_dot;
        this->x_dot += CP_TAU * x_acc;
        this->angle += CP_TAU * angle_dot;
        this->angle_dot += CP_TAU * angle_acc;

        this->paramToState(pos+1);
    }
}

template <typename NType>
NType NCartPole<NType>::stateReward(int shiftout, int action, bool& done)
{
    if(this->isOver(shiftout, action))
    {
        done = true;
        return -1; //Игру проиграли
    }
    if(done) {return +1;} //Игру выиграли
    return +1; //Каждый шаг игры
}

template <typename NType>
bool NCartPole<NType>::isOver(int /*shiftout*/, int /*action*/)
{
    return (this->angle > CP_ANGLE_THRESHOLD || this->angle < -CP_ANGLE_THRESHOLD
            || this->x > CP_X_THRESHOLD || this->x < -CP_X_THRESHOLD);
}

template <typename NType>
void NCartPole<NType>::deinitState()
{
    //this->emptyExamples();
}

template <typename NType>
NType NCartPole<NType>::getAngleState()
{
    return this->angle;
}


template <typename NType>
void NCartPole<NType>::saveECSV(DataECSV& dt, string& parent)
{
    //NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;
    string fieldExamples = parent + ".examples";

    dt.addGroup(parent, "");
    str_val = to_string(this->koef_a); dt.addElement(parent, "koef_a", str_val, typeid(NType).name());
    str_val = to_string(this->koef_b); dt.addElement(parent, "koef_b", str_val, typeid(NType).name());
    str_val = to_string(this->angle_over); dt.addElement(parent, "angle_over", str_val, typeid(NType).name());

    NExamples<NType>::saveECSV(dt, fieldExamples);
}

template <typename NType>
void NCartPole<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    //NExample<NType>* exm;
    //NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;
    //vector<string> subpath;
    string fieldExamples = parent + ".examples";

    //Выполнение операций перед загрузкой
    if(!dt.isOneMatrix()) {NCartPole<NType>::deinit();}

    if(dt.isOneMatrix())
    {
    }
    else
    {
        //Поля класса
        size_t ind = dt.getShift();
        iter = dt.modules[ind];
        if(iter->cmpPath(parent, true)) {ind++;}

        while(dt.isNextBlock(ind, parent))
        {
            iter = dt.modules[ind];

            if(iter->getFieldValue(parent, "koef_a", str_val)) {to_value(this->koef_a, str_val); ind++;}
            else if(iter->getFieldValue(parent, "koef_b", str_val)) {to_value(this->koef_b, str_val); ind++;}
            else if(iter->getFieldValue(parent, "angle_over", str_val)) {to_value(this->angle_over, str_val); ind++;}
            else if(iter->cmpPath(fieldExamples, false)) //Субполя класса
            {
                dt.setShift(ind);
                NExamples<NType>::loadECSV(dt, fieldExamples);
                ind = dt.getShift();
            }
            else
            {
                ind++;
            }
        }

        dt.setShift(ind);
    }
}

#endif // NCARTPOLE_H
