#ifndef NTRADETG_H
#define NTRADETG_H

#include "nexamples.h"
#include "ntradetgtab.h"
//#include <math.h>

template <typename NType>
class NTradeTg: public NExamples<NType>
{
public:
    NTradeTg();
    NTradeTg(NTradeTg<NType>& obj);
    virtual ~NTradeTg();

protected:
    int period; //Период агрегации баров
    NType koefTg; //Количество пунктов в цене
    NType koefPrice; //Коэффициент преобразования цены
    NType koefVolume; //Коэффициент преобразования объёма
    int lenInBar; //Количество входных баров
    NPriceDelta typeDelta; //Тип расчёта изменения цены

public:
    NMatrix<NType> dataTg; //Преобразованная таблица данных
    NTradeTgTab<NType> dataTab; //Таблица для обработки
    NTradeTgTab<NType> dataZad; //Бары, преобразованные по периодам

public:
    void setPeriod(int val);
    int getPeriod();
    void setKoefTg(NType val);
    NType getKoefTg();
    void setKoefPrice(NType val);
    NType getKoefPrice();
    void setKoefVolume(NType val);
    NType getKoefVolume();
    void setLenInBar(int val);
    int getLenInBar();
    NPriceDelta getTypeDelta();

public:
    virtual NExamples<NType>* create(); //Создание объекта
    virtual void copyTmpExamples(NExamples<NType>* exms); //Копирование примеров для временного хранилища
    virtual void doShift(int shift, bool blShift); //Переместить на величину
    virtual void deinit(); //Деинициализация
    virtual void prerun(); //Предобработка данных
    virtual void postrun(int shiftout); //Постобработка данных
    void calcPeriod(); //Бары, преобразованные по периодам
    void calcTg(); //Преобразованная таблица данных
    void calcExample(); //Формирование примеров
    void funcInTgPrice(NArray<NType>& vec1, NArray<NType>& vec2, int ind, int jnd); //Преобразование
    void funcInTgVolume(NArray<NType>& vec, int ind, int jnd);
    NType funcOutTgPrice(const NType& val);

public:
    virtual void outdata_saveECSV(DataECSV& dt, string& parent);
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NTradeTg<NType>::NTradeTg():
    NExamples<NType>()
{
    this->koefTg = 10000;
    this->koefPrice = 1;
    this->koefVolume = 1;
    this->period = 1;
    this->lenInBar = 0;
    this->typeDelta = NPriceDelta::PricePrev;
}

template <typename NType>
NTradeTg<NType>::NTradeTg(NTradeTg<NType>& obj):
    NExamples<NType>(obj)
{
    this->koefTg = obj.getKoefTg();
    this->koefPrice = obj.getKoefPrice();
    this->koefVolume = obj.getKoefVolume();
    this->period = obj.getPeriod();
    this->lenInBar = obj.getLenInBar();
    this->typeDelta = obj.getTypeDelta();
}

template <typename NType>
NTradeTg<NType>::~NTradeTg()
{
    this->deinit();
}

template <typename NType>
void NTradeTg<NType>::setPeriod(int val)
{
    this->period = val;
}

template <typename NType>
int NTradeTg<NType>::getPeriod()
{
    return this->period;
}

template <typename NType>
void NTradeTg<NType>::setKoefTg(NType val)
{
    this->koefTg = val;
}

template <typename NType>
NType NTradeTg<NType>::getKoefTg()
{
    return this->koefTg;
}

template <typename NType>
void NTradeTg<NType>::setKoefPrice(NType val)
{
    this->koefPrice = val;
}

template <typename NType>
NType NTradeTg<NType>::getKoefPrice()
{
    return this->koefPrice;
}

template <typename NType>
void NTradeTg<NType>::setKoefVolume(NType val)
{
    this->koefVolume = val;
}

template <typename NType>
NType NTradeTg<NType>::getKoefVolume()
{
    return this->koefVolume;
}

template <typename NType>
void NTradeTg<NType>::setLenInBar(int val)
{
    this->lenInBar = val;
}

template <typename NType>
int NTradeTg<NType>::getLenInBar()
{
    return this->lenInBar;
}

template <typename NType>
NPriceDelta NTradeTg<NType>::getTypeDelta()
{
    return this->typeDelta;
}


template <typename NType>
NExamples<NType>* NTradeTg<NType>::create()
{
    return (new NTradeTg<NType>());
}

template <typename NType>
void NTradeTg<NType>::copyTmpExamples(NExamples<NType>* exms)
{
    //NTradeTg<NType>* exmz = (NTradeTg<NType>*)exms;
    NTradeTg<NType>* exmz = dynamic_cast<NTradeTg*>(exms);
    NExamples<NType>::copyTmpExamples(exms);

    this->period = exmz->getPeriod();
    this->koefTg = exmz->getKoefTg();
    this->koefPrice = exmz->getKoefPrice();
    this->koefVolume = exmz->getKoefVolume();
    this->lenInBar = exmz->getLenInBar();
    this->typeDelta = exmz->getTypeDelta();

    //this->dataTg = exmz->dataTg;
    //this->dataTab = exmz->dataTab;
    this->dataZad = exmz->dataZad;
}

template <typename NType>
void NTradeTg<NType>::doShift(int shift, bool blShift)
{
    NExamples<NType>::doShift(shift, blShift);

    //Заполнение итоговой таблицы calc
    if(this->blClearTypeSet)
    {
        for(int ind = this->beginset - shift; ind < this->beginset; ind++)
        {
            this->dataZad.high_calc[ind+this->lenInBar+1] = 0;
            this->dataZad.low_calc[ind+this->lenInBar+1] = 0;
            this->dataZad.energy[ind+this->lenInBar+1] = 0;
            this->dataZad.typeExample[ind+this->lenInBar+1] = NSetType::NSetNone;
        }
    }
}

template <typename NType>
void NTradeTg<NType>::deinit()
{
    this->dataTg.clear();
    this->dataTab.deinit();
    this->dataZad.deinit();
    NExamples<NType>::deinit();
}

template <typename NType>
void NTradeTg<NType>::prerun()
{
    if(this->limitset == 0 || this->limitset > this->dataTab.getLenRow()) {this->limitset = this->dataTab.getLenRow();}

    this->calcPeriod();
    this->calcTg();
    this->calcExample();

    this->filloutput();
    this->filloutrun();
    this->update();

    //this->beginset = 0;
    //this->testset = this->getLength();
    //this->validset = this->getLength();
    //this->endset = this->getLength();
}

template <typename NType>
void NTradeTg<NType>::postrun(int /*shiftout*/)
{
    int pos;
    NExample<NType>* exm;

    if(!this->blOriginal) {this->recovery();}

    //Заполнение вектора постобработки
    for(pos = this->beginset; pos < this->endset; pos++)
    {
        exm = this->get(pos);
        exm->outpostrun.clear();
        exm->outpostrun.push(funcOutTgPrice(this->funcReversNormOut(exm->outrun[0])));
        exm->outpostrun.push(funcOutTgPrice(this->funcReversNormOut(exm->outrun[1])));
    }

    //Заполнение итоговой таблицы calc
    if(this->typeDelta == NPriceDelta::PricePrev)
    {
        for(pos = this->beginset; pos < this->endset; pos++)
        {
            exm = this->get(pos);
            this->dataZad.high_calc[pos+this->lenInBar+1] = this->dataZad.high_real[pos+this->lenInBar] + exm->outpostrun[0];
            this->dataZad.low_calc[pos+this->lenInBar+1] = this->dataZad.low_real[pos+this->lenInBar] + exm->outpostrun[1];
            this->dataZad.energy[pos+this->lenInBar+1] = exm->getEnergy();
            this->dataZad.typeExample[pos+this->lenInBar+1] = exm->getTypeSet();
        }
    }
    else if(this->typeDelta == NPriceDelta::PriceClose)
    {
        for(pos = this->beginset; pos < this->endset; pos++)
        {
            exm = this->get(pos);
            this->dataZad.high_calc[pos+this->lenInBar+1] = this->dataZad.close_real[pos+this->lenInBar] + exm->outpostrun[0];
            this->dataZad.low_calc[pos+this->lenInBar+1] = this->dataZad.close_real[pos+this->lenInBar] + exm->outpostrun[1];
            this->dataZad.energy[pos+this->lenInBar+1] = exm->getEnergy();
            this->dataZad.typeExample[pos+this->lenInBar+1] = exm->getTypeSet();
        }
    }
}

template <typename NType>
void NTradeTg<NType>::calcPeriod()
{
    int ind, knd = 0;
    this->dataZad.init(this->limitset / this->period);

    string vDate = this->dataTab.date[0];
    string vTime = this->dataTab.time[0];
    NType vOpen = this->dataTab.open_real[0];
    NType vClose = this->dataTab.close_real[0];
    NType curHigh, vHigh = this->dataTab.high_real[0];
    NType curLow, vLow = this->dataTab.low_real[0];
    NType vVolume = 0;

    for(ind = 1; ind <= this->limitset; ind++)
    {
        vClose = this->dataTab.close_real[ind-1];
        curHigh = this->dataTab.high_real[ind-1];
        curLow = this->dataTab.low_real[ind-1];
        vVolume += this->dataTab.volume_real[ind-1];

        if(vHigh < curHigh) {vHigh = curHigh;}
        if(vLow > curLow) {vLow = curLow;}
        if(ind % this->period == 0)
        {
            this->dataZad.date[knd] = vDate;
            this->dataZad.time[knd] = vTime;
            this->dataZad.open_real[knd] = vOpen;
            this->dataZad.high_real[knd] = vHigh;
            this->dataZad.low_real[knd] = vLow;
            this->dataZad.close_real[knd] = vClose;
            this->dataZad.volume_real[knd] = vVolume;

            if(ind < this->limitset)
            {
                vDate = this->dataTab.date[ind];
                vTime = this->dataTab.time[ind];
                vOpen = this->dataTab.open_real[ind];
                vHigh = this->dataTab.high_real[ind];
                vLow = this->dataTab.low_real[ind];
                vVolume = 0;

                knd++;
            }
        }
    }
}

template <typename NType>
void NTradeTg<NType>::calcTg()
{
    this->dataTg.init(this->dataZad.getLenRow()-1, 5, 0);

    if(this->typeDelta == NPriceDelta::PricePrev)
    {
        for(int ind = 1; ind < this->dataTg.getLenRow(); ind++)
        {
            funcInTgPrice(this->dataZad.open_real, this->dataZad.open_real, ind, 0);
            funcInTgPrice(this->dataZad.high_real, this->dataZad.high_real, ind, 1);
            funcInTgPrice(this->dataZad.low_real, this->dataZad.low_real, ind, 2);
            funcInTgPrice(this->dataZad.close_real, this->dataZad.close_real, ind, 3);
            funcInTgVolume(this->dataZad.volume_real, ind, 4);
        }
    }
    else if(this->typeDelta == NPriceDelta::PriceClose)
    {
        for(int ind = 1; ind < this->dataTg.getLenRow(); ind++)
        {
            funcInTgPrice(this->dataZad.close_real, this->dataZad.open_real, ind, 0);
            funcInTgPrice(this->dataZad.close_real, this->dataZad.high_real, ind, 1);
            funcInTgPrice(this->dataZad.close_real, this->dataZad.low_real, ind, 2);
            funcInTgPrice(this->dataZad.close_real, this->dataZad.close_real, ind, 3);
            funcInTgVolume(this->dataZad.volume_real, ind, 4);
        }
    }
}

template <typename NType>
void NTradeTg<NType>::calcExample()
{
    this->emptyExamples();
    int len = this->dataTg.getLenRow() - this->lenInBar;
    NType value = 0;

    for(int ind = 0; ind < len; ind++)
    {
        NExample<NType>* pExam = new NExample<NType>();

        //Вход
        for(int knd = 0; knd < this->lenInBar; knd++)
        {
            for(int jnd = 1; jnd < this->dataTg.getLenColumn(); jnd++)
            {
                value = this->dataTg[ind + knd][jnd];
                value = this->funcNormIn(value);
                pExam->input.push(value);
            }
        }

        //Выход
        value = this->dataTg[ind + this->lenInBar][1];
        value = this->funcNormOut(value);
        pExam->output.push(value);

        value = this->dataTg[ind + this->lenInBar][2];
        value = this->funcNormOut(value);
        pExam->output.push(value);

        this->push(pExam);
    }
}

template <typename NType>
void NTradeTg<NType>::funcInTgPrice(NArray<NType>& vec1, NArray<NType>& vec2, int ind, int jnd)
{
    NType dValue = vec2[ind] - vec1[ind-1];
    dValue = NFUNC_ATAN(this->koefTg * this->koefPrice * dValue);
    this->dataTg.set(dValue, ind-1, jnd);
}

template <typename NType>
void NTradeTg<NType>::funcInTgVolume(NArray<NType>& vec, int ind, int jnd)
{
    NType dValue = vec[ind] - vec[ind-1];
    dValue = NFUNC_ATAN(this->koefVolume * dValue);
    this->dataTg.set(dValue, ind-1, jnd);
}

template <typename NType>
NType NTradeTg<NType>::funcOutTgPrice(const NType& val)
{
    return NFUNC_TAN(val) / (this->koefTg * this->koefPrice);
}


template <typename NType>
void NTradeTg<NType>::outdata_saveECSV(DataECSV& dt, string& parent)
{
    NTradeTgTab<NType> dataZadValidate;
    string fieldZad = parent + ".dataZad";
    //string fieldExamples = parent + ".examples";

    //Выполнение операций перед выгрузкой
    dataZadValidate.copyData(this->dataZad, NSetType::NSetValidate);

    //Построение блоков
    dt.addGroup(parent, "");
    dataZadValidate.saveECSV(dt, fieldZad);
    //NExamples<NType>::outdata_saveECSV(dt, fieldExamples);
}

template <typename NType>
void NTradeTg<NType>::saveECSV(DataECSV& dt, string& parent)
{
    NMatrix<string> str_mtrx;
    vector<string> str_vec;
    string str_val;
    string fieldTab = parent + ".dataTab";
    string fieldZad = parent + ".dataZad";
    string fieldExamples = parent + ".examples";

    dt.addGroup(parent, "");
    str_val = to_string(this->period); dt.addElement(parent, "period", str_val, typeid(int).name());
    str_val = to_string(this->koefTg); dt.addElement(parent, "koefTg", str_val, typeid(NType).name());
    str_val = to_string(this->koefPrice); dt.addElement(parent, "koefPrice", str_val, typeid(NType).name());
    str_val = to_string(this->koefVolume); dt.addElement(parent, "koefVolume", str_val, typeid(NType).name());
    str_val = to_string(this->lenInBar); dt.addElement(parent, "lenInBar", str_val, typeid(int).name());
    str_val = to_string(this->typeDelta); dt.addElement(parent, "typeDelta", str_val, typeid(int).name());

    //to_matrix_string(str_mtrx, this->dataTg); dt.addElement(parent, "dataTg", str_mtrx, typeid(NType).name());
    //this->dataTab.saveECSV(dt, fieldTab);
    this->dataZad.saveECSV(dt, fieldZad);

    NExamples<NType>::saveECSV(dt, fieldExamples);
}

template <typename NType>
void NTradeTg<NType>::loadECSV(DataECSV& dt, string& parent)
{
    StructECSV* iter;
    //NExample<NType>* exm;
    NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;
    vector<string> subpath;
    string fieldTab = parent + ".dataTab";
    string fieldZad = parent + ".dataZad";
    string fieldExamples = parent + ".examples";

    //Выполнение операций перед загрузкой
    if(!dt.isOneMatrix()) {NTradeTg<NType>::deinit();}

    if(dt.isOneMatrix())
    {
        this->dataTab.loadECSV(dt, parent);
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

            if(iter->getFieldValue(parent, "period", str_val)) {to_value(this->period, str_val); ind++;}
            else if(iter->getFieldValue(parent, "koefTg", str_val)) {to_value(this->koefTg, str_val); ind++;}
            else if(iter->getFieldValue(parent, "koefPrice", str_val)) {to_value(this->koefPrice, str_val); ind++;}
            else if(iter->getFieldValue(parent, "koefVolume", str_val)) {to_value(this->koefVolume, str_val); ind++;}
            else if(iter->getFieldValue(parent, "lenInBar", str_val)) {to_value(this->lenInBar, str_val); ind++;}
            else if(iter->getFieldValue(parent, "typeDelta", str_val)) {to_value(this->typeDelta, str_val); ind++;}

            else if(iter->getFieldValue(parent, "dataTg", str_mtrx)) {to_matrix_value(this->dataTg, str_mtrx); ind++;}
            else if(iter->cmpPath(fieldTab, false)) //Субполя класса
            {
                dt.setShift(ind);
                this->dataTab.loadECSV(dt, fieldTab);
                ind = dt.getShift();
            }
            else if(iter->cmpPath(fieldZad, false)) //Субполя класса
            {
                dt.setShift(ind);
                this->dataZad.loadECSV(dt, fieldZad);
                ind = dt.getShift();
            }
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

#endif // NTRADETG_H
