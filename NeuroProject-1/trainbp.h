#ifndef TRAINBP_H
#define TRAINBP_H

#include "ineuronet.h"

template <typename NType>
class TrainBP
{
public:
    TrainBP();
    ~TrainBP();
public:
    INeuroNet<NType>* net;
    INeuroNet<NType>* changeNet;
    NArray< NArray<NType>* > errorNet;
    NArray<NType> backOut;
protected:
    NType speedLearning;
    NType speedRegularization;
    NType inertia;
    int maxEpoches;
    NType minError;
    NType minErrorChange;
    int epoches;
    int stabEnergy;
    NType changeEnergy;
protected:
    NMatrix<NType> changeWeight1, changeWeight2;
    NArray<NType> changeBias1, changeBias2;
    NType koefTrain;
    NType koefRegular;
    INLayer<NType>** pLay; //net.lay
    NArray<NType>** pMas; //errorNet
    NExample<NType>** pExam; //net.exam
    INLayer<NType>** pChangeLay; //changeNet.lay
    NType* pOutrun; //exm.outrun
    int lenLay; //net.lay
public:
    void setSpeedLearning(NType param);
    void setSpeedRegularization(NType param);
    void setInertia(NType param);
    void setMaxEpoches(NType param);
    void setMinError(NType param);
    void setMinErrorChange(NType param);
    NType getSpeedLearning();
    NType getSpeedRegularization();
    NType getInertia();
    int getEpoches();
    int getStabEnergy();
    NType getChangeEnergy();
public:
    virtual void init();
    virtual void deinit();
    virtual void train();
    virtual void calculateError(NExample<NType>* exm);
    virtual void change(NExample<NType>* exm);
    virtual void update();
};

#endif // TRAINBP_H
