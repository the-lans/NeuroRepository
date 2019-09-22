#ifndef TRADEPARAMETRS_H
#define TRADEPARAMETRS_H

//#include <string>
#include "iobjectecsv.h"

//using namespace std;

struct TradeParametrs: IObjectECSV
{
    TradeParametrs();

    int period; //Период агрегации баров
    int timeframe_min; //Размер таймфрейма в минутах
    int lenInBar; //Количество входных баров
    int wind; //Количество баров, нужное для обучения
    float vStopLoss; //Размер Stop Loss
    float vTakeProfit; //Размер Take Profit
    int stepExmTime; //Обучение Time: шаг взятия примеров
    int countIteration; //Количество отсчётов для тестирования
    string name_inarch; //Имя входного файла с архивом данных
    string name_input; //Имя входного файла для задачи
    string name_outdata; //Имя выходного файла по задаче
    string name_outtotal; //Имя общего выходного файла с данными

    virtual void saveECSV(DataECSV& dt, string& parent); //Сохранение объекта в структуру
    virtual void loadECSV(DataECSV& dt, string& parent); //Загрузка объекта из структуры
};

#endif // TRADEPARAMETRS_H
