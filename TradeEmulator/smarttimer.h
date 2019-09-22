#ifndef SMARTTIMER_H
#define SMARTTIMER_H

#include "defsmart.h"
#include "funcsmart.h"
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>

class SmartTimer
{
public:
    SmartTimer();
    SmartTimer(SmartTimer& obj);

public:
    std::vector<clock_t> arr_time; //Массив времени
    std::vector<number_id> arr_num; //Массив идентификаторов

protected:
    clock_t start_time; //Начальное время
    clock_t end_time; //Конечное время

public:
    clock_t getStartTime();
    clock_t getEndTime();
    size_t getIndex(number_id num);

public:
    void start(); //Запуск таймера
    void stop(); //Остановка таймера
    void stop(number_id num);
    void clear(); //Очистка таймера
    void addBeg(clock_t val_time, number_id num = ID_NONE); //Добавление точки в начало
    void addEnd(clock_t val_time, number_id num = ID_NONE); //Добавление точки в конец
    void collapse(std::vector<clock_t>& result); //Свернуть время: arr_time[0] = 0
    void collapsesec(std::vector<float>& result);
    void collapsemin(std::vector<float>& result);
    clock_t deltatime(); //Разница во времени
    clock_t deltatime(int ind);
    clock_t deltatime(int ind1, int ind2);
    float deltasec(); //Разница в секундах
    float deltasec(int ind);
    float deltasec(int ind1, int ind2);
    float deltamin(); //Разница в минутах
    float deltamin(int ind);
    float deltamin(int ind1, int ind2);
    std::string toString();
};

#endif // SMARTTIMER_H
