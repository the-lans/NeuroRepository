#include "smarttimer.h"

SmartTimer::SmartTimer()
{
    start_time = 0;
    end_time = 0;
}

SmartTimer::SmartTimer(SmartTimer& obj)
{
    this->start_time = obj.getStartTime();
    this->end_time = obj.getEndTime();
    this->arr_time = obj.arr_time;
    this->arr_num = obj.arr_num;
}

clock_t SmartTimer::getStartTime()
{
    return start_time;
}

clock_t SmartTimer::getEndTime()
{
    return end_time;
}

size_t SmartTimer::getIndex(number_id num)
{
    return std::find(arr_num.begin(), arr_num.end(), num) - arr_num.begin();
}

void SmartTimer::start()
{
    arr_time.clear();
    arr_num.clear();
    start_time = clock();
    end_time = start_time;
}

void SmartTimer::stop()
{
    stop(ID_NONE);
}

void SmartTimer::stop(number_id num)
{
    end_time = clock();
    arr_time.push_back(end_time);
    arr_num.push_back(num);
}

void SmartTimer::clear()
{
    arr_time.clear();
    arr_num.clear();
    start_time = 0;
    end_time = 0;
}

void SmartTimer::addBeg(clock_t val_time, number_id num)
{
    arr_time.insert(arr_time.begin(), start_time);
    arr_num.insert(arr_num.begin(), num);
    start_time = val_time;
}

void SmartTimer::addEnd(clock_t val_time, number_id num)
{
    end_time = val_time;
    arr_time.push_back(val_time);
    arr_num.push_back(num);
}

void SmartTimer::collapse(std::vector<clock_t>& result)
{
    result.clear();
    if(arr_time.size() > 0)
    {
        for(size_t ind = 0; ind < arr_time.size(); ind++)
        {
            result.push_back(arr_time[ind] - start_time);
        }
    }
}

void SmartTimer::collapsesec(std::vector<float>& result)
{
    result.clear();
    if(arr_time.size() > 0)
    {
        for(size_t ind = 0; ind < arr_time.size(); ind++)
        {
            result.push_back(((float)(arr_time[ind] - start_time))/1000);
        }
    }
}

void SmartTimer::collapsemin(std::vector<float>& result)
{
    result.clear();
    if(arr_time.size() > 0)
    {
        for(size_t ind = 0; ind < arr_time.size(); ind++)
        {
            result.push_back(((float)(arr_time[ind] - start_time))/60000);
        }
    }
}

clock_t SmartTimer::deltatime()
{
    return (end_time - start_time);
}

clock_t SmartTimer::deltatime(int ind)
{
    return (ind > 0 ? arr_time[ind] - arr_time[ind-1] : 0);
}

clock_t SmartTimer::deltatime(int ind1, int ind2)
{
    return (ind2 > ind1 ? arr_time[ind2] - arr_time[ind1] : 0);
}

float SmartTimer::deltasec()
{
    return ((float)deltatime())/1000;
}

float SmartTimer::deltasec(int ind)
{
    return ((float)deltatime(ind))/1000;
}

float SmartTimer::deltasec(int ind1, int ind2)
{
    return ((float)deltatime(ind1, ind2))/1000;
}

float SmartTimer::deltamin()
{
    return ((float)deltatime())/60000;
}

float SmartTimer::deltamin(int ind)
{
    return ((float)deltatime(ind))/60000;
}

float SmartTimer::deltamin(int ind1, int ind2)
{
    return ((float)deltatime(ind1, ind2))/60000;
}

std::string SmartTimer::toString()
{
    std::string str;
    std::vector<std::string> str_vec;
    std::vector<float> colsec_time;

    this->collapsesec(colsec_time);
    to_array_string(str_vec, colsec_time);
    concat(str, str_vec, "; ");

    return str;
}
