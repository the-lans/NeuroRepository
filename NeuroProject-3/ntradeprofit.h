#ifndef NTRADEPROFIT_H
#define NTRADEPROFIT_H

#include "nexamples.h"
#include "ntradeclasstab.h"

template <typename NType>
class NTradeProfit: public NExamples<NType>
{
public:
    NTradeProfit();
    NTradeProfit(NTradeProfit<NType>& obj);
    virtual ~NTradeProfit();

protected:
    int period; //Период агрегации баров
    NType koefTg; //Количество пунктов в цене
    NType koefPrice; //Коэффициент преобразования цены
    NType koefVolume; //Коэффициент преобразования объёма
    int lenInBar; //Количество входных баров
    NPriceDelta typeDelta; //Тип расчёта изменения цены
    int periodBig; //Большой период для отслеживания изменений цены
    int vStopLoss; //Размер Stop Loss
    NType koefTakeProfit; //Коэффициент для расчёта Take Profit
    //bool profitClass; //Преобразовать выход в номер класса? 1 - buy; 2 - sell; 3 - none
    bool blOnePrice; //Операции с одной ценой Close

public:
    NMatrix<NType> dataTg; //Преобразованная таблица данных
    NTradeClassTab<NType> dataTab; //Таблица для обработки
    NTradeClassTab<NType> dataZad; //Бары, преобразованные по периодам

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
    int getPeriodBig();
    int getStopLoss();
    NType getKoefTakeProfit();
    bool getOnePrice();

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
    void funcOutClassPrice(int ind, int jnd);

public:
    virtual void outdata_saveECSV(DataECSV& dt, string& parent);
    virtual void saveECSV(DataECSV& dt, string& parent);
    virtual void loadECSV(DataECSV& dt, string& parent);
};


template <typename NType>
NTradeProfit<NType>::NTradeProfit():
    NExamples<NType>()
{
    this->koefTg = 10000;
    this->koefPrice = 1;
    this->koefVolume = 1;
    this->period = 1;
    this->lenInBar = 0;
    this->typeDelta = NPriceDelta::PricePrev;
    this->periodBig = 1;
    this->vStopLoss = 0;
    this->koefTakeProfit = 1;
    this->blOnePrice = false;
}

template <typename NType>
NTradeProfit<NType>::NTradeProfit(NTradeProfit<NType>& obj):
    NExamples<NType>(obj)
{
    this->koefTg = obj.getKoefTg();
    this->koefPrice = obj.getKoefPrice();
    this->koefVolume = obj.getKoefVolume();
    this->period = obj.getPeriod();
    this->lenInBar = obj.getLenInBar();
    this->typeDelta = obj.getTypeDelta();
    this->periodBig = obj.getPeriodBig();
    this->vStopLoss = obj.getStopLoss();
    this->koefTakeProfit = obj.getKoefTakeProfit();
    this->blOnePrice = obj.getOnePrice();
}

template <typename NType>
NTradeProfit<NType>::~NTradeProfit()
{
    this->deinit();
}

template <typename NType>
void NTradeProfit<NType>::setPeriod(int val)
{
    this->period = val;
}

template <typename NType>
int NTradeProfit<NType>::getPeriod()
{
    return this->period;
}

template <typename NType>
void NTradeProfit<NType>::setKoefTg(NType val)
{
    this->koefTg = val;
}

template <typename NType>
NType NTradeProfit<NType>::getKoefTg()
{
    return this->koefTg;
}

template <typename NType>
void NTradeProfit<NType>::setKoefPrice(NType val)
{
    this->koefPrice = val;
}

template <typename NType>
NType NTradeProfit<NType>::getKoefPrice()
{
    return this->koefPrice;
}

template <typename NType>
void NTradeProfit<NType>::setKoefVolume(NType val)
{
    this->koefVolume = val;
}

template <typename NType>
NType NTradeProfit<NType>::getKoefVolume()
{
    return this->koefVolume;
}

template <typename NType>
void NTradeProfit<NType>::setLenInBar(int val)
{
    this->lenInBar = val;
}

template <typename NType>
int NTradeProfit<NType>::getLenInBar()
{
    return this->lenInBar;
}

template <typename NType>
NPriceDelta NTradeProfit<NType>::getTypeDelta()
{
    return this->typeDelta;
}

template <typename NType>
int NTradeProfit<NType>::getPeriodBig()
{
    return this->periodBig;
}

template <typename NType>
int NTradeProfit<NType>::getStopLoss()
{
    return this->vStopLoss;
}

template <typename NType>
NType NTradeProfit<NType>::getKoefTakeProfit()
{
    return this->koefTakeProfit;
}

template <typename NType>
bool NTradeProfit<NType>::getOnePrice()
{
    return this->blOnePrice;
}


template <typename NType>
NExamples<NType>* NTradeProfit<NType>::create()
{
    return (new NTradeProfit<NType>());
}

template <typename NType>
void NTradeProfit<NType>::copyTmpExamples(NExamples<NType>* exms)
{
    NTradeProfit<NType>* exmz = (NTradeProfit<NType>*)exms;
    NExamples<NType>::copyTmpExamples(exms);

    this->period = exmz->getPeriod();
    this->koefTg = exmz->getKoefTg();
    this->koefPrice = exmz->getKoefPrice();
    this->koefVolume = exmz->getKoefVolume();
    this->lenInBar = exmz->getLenInBar();
    this->typeDelta = exmz->getTypeDelta();

    this->periodBig = exmz->getPeriodBig();
    this->vStopLoss = exmz->getStopLoss();
    this->koefTakeProfit = exmz->getKoefTakeProfit();
    this->blOnePrice = exmz->getOnePrice();

    //this->dataTg = exmz->dataTg;
    //this->dataTab = exmz->dataTab;
    this->dataZad = exmz->dataZad;
    //cout << "NTradeProfit = " << exmz->dataZad.date.getLength() << endl;
    //cout << " date = " << this->dataZad.date.getLength() << endl;
    //cout << " LenRow = " << this->dataZad.getLenRow() << endl;
}

template <typename NType>
void NTradeProfit<NType>::doShift(int shift, bool blShift)
{
    NExamples<NType>::doShift(shift, blShift);

    //Заполнение итоговой таблицы calc
    if(this->blClearTypeSet)
    {
        for(int ind = this->beginset - shift; ind < this->beginset; ind++)
        {
            this->dataZad.class_run[ind+this->lenInBar+1] = 0;
            this->dataZad.class_calc[ind+this->lenInBar+1] = 0;
            this->dataZad.energy[ind+this->lenInBar+1] = 0;
            this->dataZad.typeExample[ind+this->lenInBar+1] = NSetType::NSetNone;
        }
    }
}

template <typename NType>
void NTradeProfit<NType>::deinit()
{
    this->dataTg.clear();
    this->dataTab.deinit();
    this->dataZad.deinit();
    NExamples<NType>::deinit();
}

template <typename NType>
void NTradeProfit<NType>::prerun()
{
    if(this->limitset == 0 || this->limitset > this->dataTab.getLenRow()) {this->limitset = this->dataTab.getLenRow();}

    this->calcPeriod();
    this->calcTg();
    this->calcExample();

    this->filloutput();
    this->filloutrun();
    this->update();
}

template <typename NType>
void NTradeProfit<NType>::postrun(int shiftout)
{
    int ind, jnd, pos;
    NType value, valrun, valmax;
    NExample<NType>* exm;

    //Заполнение вектора постобработки
    //NExamples<NType>::postrun(shiftout);
    if(!this->blOriginal) {this->recovery();}

    for(pos = this->beginset; pos < this->endset; pos++)
    {
        exm = this->get(pos);
        exm->outpostrun.clear();
        int len = this->getLenOutNumClass(exm);
        for(jnd = 0; jnd < len; jnd++)
        {
            value = 0; valmax = 0;
            for(ind = 0; ind < this->lenOut; ind++)
            {
                valrun = exm->outrun[jnd * this->lenOut + ind];
                valrun = this->funcReversNormOut(valrun);
                if(valrun > valmax) {valmax = valrun; value = ind+1;}
            }
            if(this->blFilter && valmax <= this->valFilter) {value = 0;}
            if(!this->blClass && this->lenOut > 1 && value > 0) {value--;}
            exm->outpostrun.push(value);
        }
    }

    //Заполнение итоговой таблицы calc
    for(pos = this->beginset; pos < this->endset; pos++)
    {
        exm = this->get(pos);
        this->dataZad.class_run[pos+this->lenInBar+1] = exm->outrun[shiftout];
        this->dataZad.class_calc[pos+this->lenInBar+1] = exm->outpostrun[shiftout];
        this->dataZad.energy[pos+this->lenInBar+1] = exm->getEnergy();
        this->dataZad.typeExample[pos+this->lenInBar+1] = exm->getTypeSet();
    }

    //cout << "NTradeProfit = " << this->beginset << "/" << this->endset << endl;
}

template <typename NType>
void NTradeProfit<NType>::calcPeriod()
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
void NTradeProfit<NType>::calcTg()
{
    int len = this->dataZad.getLenRow() - this->periodBig;
    this->dataTg.init(len-1, 6, 0);

    if(this->typeDelta == NPriceDelta::PricePrev)
    {
        for(int ind = 1; ind < len; ind++)
        {
            funcInTgPrice(this->dataZad.open_real, this->dataZad.open_real, ind, 0);
            funcInTgPrice(this->dataZad.high_real, this->dataZad.high_real, ind, 1);
            funcInTgPrice(this->dataZad.low_real, this->dataZad.low_real, ind, 2);
            funcInTgPrice(this->dataZad.close_real, this->dataZad.close_real, ind, 3);
            funcInTgVolume(this->dataZad.volume_real, ind, 4);
            funcOutClassPrice(ind, 5);
        }
    }
    else if(this->typeDelta == NPriceDelta::PriceClose)
    {
        for(int ind = 1; ind < len; ind++)
        {
            funcInTgPrice(this->dataZad.close_real, this->dataZad.open_real, ind, 0);
            funcInTgPrice(this->dataZad.close_real, this->dataZad.high_real, ind, 1);
            funcInTgPrice(this->dataZad.close_real, this->dataZad.low_real, ind, 2);
            funcInTgPrice(this->dataZad.close_real, this->dataZad.close_real, ind, 3);
            funcInTgVolume(this->dataZad.volume_real, ind, 4);
            funcOutClassPrice(ind, 5);
        }
    }
}

template <typename NType>
void NTradeProfit<NType>::calcExample()
{
    int ind, jnd, knd;
    int len = this->dataTg.getLenRow() - this->lenInBar;
    NType value_class, value;
    this->emptyExamples();

    for(ind = 0; ind < len; ind++)
    {
        NExample<NType>* pExam = new NExample<NType>();

        //Вход
        for(knd = 0; knd < this->lenInBar; knd++)
        {
            if(this->blOnePrice)
            {
                value = this->dataTg[ind + knd][3];
                value = this->funcNormIn(value);
                pExam->input.push(value);
            }
            else
            {
                for(int jnd = 1; jnd < 5; jnd++)
                {
                    value = this->dataTg[ind + knd][jnd];
                    value = this->funcNormIn(value);
                    pExam->input.push(value);
                }
            }
        }

        //Выход
        value_class = this->dataTg[ind + this->lenInBar][5];
        if(this->blClass)
        {
            for(jnd = 1; jnd <= this->lenClass; jnd++)
            {
                value = (isEqual(value_class, (NType)jnd) ? 1.0 : 0.0);
                value = this->funcNormOut(value);
                pExam->output.push(value);
            }

            if(this->lenClass == this->lenOut)
            {
                pExam->outputNumClass.push(value_class);
            }
            else
            {
                for(jnd = 0; jnd < pExam->output.getLength(); jnd++)
                {
                    value_class = (isEqual(pExam->output[jnd], (NType)1.0) ? 1 : 0);
                    pExam->outputNumClass.push(value_class);
                }
            }
        }
        else
        {
            if(this->lenClass <= 1)
            {
                pExam->output.push(this->funcNormOut(value_class));
                value_class = (isEqual(value_class, (NType)1.0) ? 1 : (int)value_class+3);
                pExam->outputNumClass.push(value_class);
            }
            else
            {
                pExam->output.push((value_class > 0 ? this->funcNormOut(value_class) : 0.0));
                pExam->output.push((value_class < 0 ? this->funcNormOut(-value_class) : 0.0));
                pExam->outputNumClass.push(isEqual(value_class, (NType)1.0) ? 1 : 0);
                pExam->outputNumClass.push(isEqual(value_class, (NType)-1.0) ? 1 : 0);
            }
        }

        this->push(pExam);
    }
}

template <typename NType>
void NTradeProfit<NType>::funcInTgPrice(NArray<NType>& vec1, NArray<NType>& vec2, int ind, int jnd)
{
    NType dValue = vec2[ind] - vec1[ind-1];
    dValue = NFUNC_ATAN(this->koefTg * this->koefPrice * dValue);
    this->dataTg.set(dValue, ind-1, jnd);
}

template <typename NType>
void NTradeProfit<NType>::funcInTgVolume(NArray<NType>& vec, int ind, int jnd)
{
    NType dValue = vec[ind] - vec[ind-1];
    dValue = NFUNC_ATAN(this->koefVolume * dValue);
    this->dataTg.set(dValue, ind-1, jnd);
}

template <typename NType>
NType NTradeProfit<NType>::funcOutTgPrice(const NType& val)
{
    return NFUNC_TAN(val) / (this->koefTg * this->koefPrice);
}

template <typename NType>
void NTradeProfit<NType>::funcOutClassPrice(int ind, int jnd)
{
    int i;
    bool bl = true;
    bool blExit = false;
    NType dValue = (this->blClass ? 3 : 0.0);
    NType price = this->dataZad.close_real.get(ind-1);
    NType SL = vStopLoss / koefTg;
    NType TP = koefTakeProfit * vStopLoss / koefTg;

    //buy
    for(i = ind; i < ind + this->periodBig; i++)
    {
        if(this->dataZad.low_real[i] <= price - SL) {blExit = true; break;}
        if(this->dataZad.high_real[i] >= price + TP) {blExit = true; bl = false; dValue = (this->blClass ? 1 : 1.0); break;}
    }

    //sell
    if(bl)
    {
        for(i = ind; i < ind + this->periodBig; i++)
        {
            if(this->dataZad.high_real[i] >= price + SL) {blExit = true; break;}
            if(this->dataZad.low_real[i] <= price - TP) {blExit = true; bl = false; dValue = (this->blClass ? 2 : -1.0); break;}
        }
    }

    if(!this->blClass && !blExit) {dValue = (this->dataZad.close_real[ind + this->periodBig - 1] - price) / TP;}
    this->dataZad.class_real[ind] = dValue;
    this->dataTg.set(dValue, ind-1, jnd);
}


template <typename NType>
void NTradeProfit<NType>::outdata_saveECSV(DataECSV& dt, string& parent)
{
    NTradeClassTab<NType> dataZadValidate;
    string fieldZad = parent + ".dataZad";
    //string fieldExamples = parent + ".examples";

    //Выполнение операций перед выгрузкой
    dataZadValidate.copyData(this->dataZad, NSetType::NSetValidate);

    //Построение блоков
    dt.addGroup(parent, "");
    dataZadValidate.saveECSV(dt, fieldZad);
    //this->dataZad.saveECSV(dt, fieldZad);
    //NExamples<NType>::outdata_saveECSV(dt, fieldExamples);
}

template <typename NType>
void NTradeProfit<NType>::saveECSV(DataECSV& dt, string& parent)
{
    //NMatrix<string> str_mtrx;
    //vector<string> str_vec;
    string str_val;
    //string fieldTab = parent + ".dataTab";
    string fieldZad = parent + ".dataZad";
    string fieldExamples = parent + ".examples";

    dt.addGroup(parent, "");
    str_val = to_string(this->period); dt.addElement(parent, "period", str_val, typeid(int).name());
    str_val = to_string(this->koefTg); dt.addElement(parent, "koefTg", str_val, typeid(NType).name());
    str_val = to_string(this->koefPrice); dt.addElement(parent, "koefPrice", str_val, typeid(NType).name());
    str_val = to_string(this->koefVolume); dt.addElement(parent, "koefVolume", str_val, typeid(NType).name());
    str_val = to_string(this->lenInBar); dt.addElement(parent, "lenInBar", str_val, typeid(int).name());
    str_val = to_string(this->typeDelta); dt.addElement(parent, "typeDelta", str_val, typeid(int).name());
    str_val = to_string(this->periodBig); dt.addElement(parent, "periodBig", str_val, typeid(int).name());
    str_val = to_string(this->vStopLoss); dt.addElement(parent, "vStopLoss", str_val, typeid(int).name());
    str_val = to_string(this->koefTakeProfit); dt.addElement(parent, "koefTakeProfit", str_val, typeid(NType).name());
    str_val = to_vstring(this->blOnePrice); dt.addElement(parent, "blOnePrice", str_val, typeid(bool).name());

    //to_matrix_string(str_mtrx, this->dataTg); dt.addElement(parent, "dataTg", str_mtrx, typeid(NType).name());
    //this->dataTab.saveECSV(dt, fieldTab);
    this->dataZad.saveECSV(dt, fieldZad);

    NExamples<NType>::saveECSV(dt, fieldExamples);
}

template <typename NType>
void NTradeProfit<NType>::loadECSV(DataECSV& dt, string& parent)
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
    if(!dt.isOneMatrix()) {NTradeProfit<NType>::deinit();}

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
            else if(iter->getFieldValue(parent, "periodBig", str_val)) {to_value(this->periodBig, str_val); ind++;}
            else if(iter->getFieldValue(parent, "vStopLoss", str_val)) {to_value(this->vStopLoss, str_val); ind++;}
            else if(iter->getFieldValue(parent, "koefTakeProfit", str_val)) {to_value(this->koefTakeProfit, str_val); ind++;}
            else if(iter->getFieldValue(parent, "blOnePrice", str_val)) {to_value(this->blOnePrice, str_val);  ind++;}

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

#endif // NTRADEPROFIT_H
