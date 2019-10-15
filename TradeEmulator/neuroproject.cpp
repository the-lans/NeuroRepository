#include "neuroproject.h"

default_random_engine NRandGenerator;


void init_random_generator()
{
    //random_device rd;
    NRandGenerator.seed(static_cast<unsigned int>(std::time(nullptr)));
}

float deltatime(clock_t start_time, clock_t end_time)
{
    return static_cast<float>(end_time - start_time)/1000;
}

void time_srand()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void to_value(NLayerType& value, string str_val)
{
    str_to_lower(str_val);
         if(str_val == "tanh")      {value = NLayerType::NFuncTanh;}
    else if(str_val == "softsign")  {value = NLayerType::NFuncSoftsign;}
    else if(str_val == "arctg")     {value = NLayerType::NFuncArctg;}
    else if(str_val == "linear")    {value = NLayerType::NFuncLinear;}
    else if(str_val == "step")      {value = NLayerType::NFuncStep;}
    else if(str_val == "softstep")  {value = NLayerType::NFuncSoftStep;}
    else if(str_val == "relu")      {value = NLayerType::NFuncReLU;}
    else if(str_val == "prelu")     {value = NLayerType::NFuncPReLU;}
    else if(str_val == "elu")       {value = NLayerType::NFuncELU;}
    else if(str_val == "softplus")  {value = NLayerType::NFuncSoftPlus;}
    else if(str_val == "sin")       {value = NLayerType::NFuncSin;}
    else if(str_val == "sinc")      {value = NLayerType::NFuncSinc;}
    else if(str_val == "gaussian")  {value = NLayerType::NFuncGaussian;}
    else if(str_val == "linearint") {value = NLayerType::NFuncLinearInt;}
    else if(str_val == "softmax")   {value = NLayerType::NFuncSoftMax;}
    else {value = NLayerType::NFuncNone;}
}

void to_value(NLayerDerivat& value, string str_val)
{
    str_to_lower(str_val);
    if(str_val == "out") {value = NLayerDerivat::NDerivatOut;}
    else {value = NLayerDerivat::NDerivatSum;}
}

void to_value(NInitType& value, string str_val)
{
    str_to_lower(str_val);
         if(str_val == "initempty") {value = NInitType::NInitEmpty;}
    else if(str_val == "initg") {value = NInitType::NInitG;}
    else {value = NInitType::NInitNone;}
}

void to_value(NSetType& value, string str_val)
{
    str_to_lower(str_val);
         if(str_val == "train") {value = NSetType::NSetTrain;}
    else if(str_val == "test") {value = NSetType::NSetTest;}
    else if(str_val == "validate") {value = NSetType::NSetValidate;}
    else {value = NSetType::NSetNone;}
}

void to_value(NTargetFunc& value, string str_val)
{
    str_to_lower(str_val);
         if(str_val == "mse") {value = NTargetFunc::NTargetMSE;}
    else if(str_val == "cross") {value = NTargetFunc::NTargetCross;}
    else {value = NTargetFunc::NTargetNone;}
}

void to_value(NNormalizationFunc& value, string str_val)
{
    str_to_lower(str_val);
         if(str_val == "linear") {value = NNormalizationFunc::NNormLinear;}
    else if(str_val == "softstep") {value = NNormalizationFunc::NNormSoftStep;}
    else if(str_val == "arctg") {value = NNormalizationFunc::NNormArctg;}
    else if(str_val == "gaussian") {value = NNormalizationFunc::NNormGaussian;}
    else {value = NNormalizationFunc::NNormNone;}
}

void to_value(NCommandNet& value, string str_val)
{
    str_to_lower(str_val);
         if(str_val == "runtrain") {value = NCommandNet::NComRunTrain;}
    else if(str_val == "runfunc") {value = NCommandNet::NComRunFunc;}
    else if(str_val == "updateout") {value = NCommandNet::NComUpdateOut;}
    else if(str_val == "begnpt") {value = NCommandNet::NComBeg;}
    else if(str_val == "endnpt") {value = NCommandNet::NComEnd;}
    else if(str_val == "finishnpt") {value = NCommandNet::NComFinish;}
    else if(str_val == "begextern") {value = NCommandNet::NComBegExtern;}
    else if(str_val == "endextern") {value = NCommandNet::NComEndExtern;}
    else if(str_val == "endtrain") {value = NCommandNet::NComEndTrain;}
    else if(str_val == "ncsave") {value = NCommandNet::NComNCSave;}
    else if(str_val == "issave") {value = NCommandNet::NComIsSave;}
    else {value = NCommandNet::NComNone;}
}

void to_value(NTrainType& value, string str_val)
{
    str_to_lower(str_val);
         if(str_val == "trainbp") {value = NTrainType::NTrainBPType;}
    else if(str_val == "agent") {value = NTrainType::NAgentType;}
    else {value = NTrainType::NTrainNone;}
}

void to_value(NOptimizerType& value, string str_val)
{
    str_to_lower(str_val);
         if(str_val == "sgd") {value = NOptimizerType::NOptimizerSGD;}
    else if(str_val == "adam") {value = NOptimizerType::NOptimizerAdam;}
    else {value = NOptimizerType::NOptimizerNone;}
}


string to_string(const NLayerType& value)
{
         if(value == NLayerType::NFuncTanh)      {return "Tanh";}
    else if(value == NLayerType::NFuncSoftsign)  {return "Softsign";}
    else if(value == NLayerType::NFuncArctg)     {return "Arctg";}
    else if(value == NLayerType::NFuncLinear)    {return "Linear";}
    else if(value == NLayerType::NFuncStep)      {return "Step";}
    else if(value == NLayerType::NFuncSoftStep)  {return "SoftStep";}
    else if(value == NLayerType::NFuncReLU)      {return "ReLU";}
    else if(value == NLayerType::NFuncPReLU)     {return "PReLU";}
    else if(value == NLayerType::NFuncELU)       {return "ELU";}
    else if(value == NLayerType::NFuncSoftPlus)  {return "SoftPlus";}
    else if(value == NLayerType::NFuncSin)       {return "Sin";}
    else if(value == NLayerType::NFuncSinc)      {return "Sinc";}
    else if(value == NLayerType::NFuncGaussian)  {return "Gaussian";}
    else if(value == NLayerType::NFuncLinearInt) {return "LinearInt";}
    else if(value == NLayerType::NFuncSoftMax)   {return "SoftMax";}
    else return "None";
}

string to_string(const NLayerDerivat& value)
{
    if(value == NLayerDerivat::NDerivatOut) {return "Out";}
    else {return "Sum";}
}

string to_string(const NInitType& value)
{
         if(value == NInitType::NInitEmpty) {return "InitEmpty";}
    else if(value == NInitType::NInitG) {return "InitG";}
    else {return "InitNone";}
}

string to_string(const NSetType& value)
{
         if(value == NSetType::NSetTrain) {return "Train";}
    else if(value == NSetType::NSetTest) {return "Test";}
    else if(value == NSetType::NSetValidate) {return "Validate";}
    else {return "None";}
}

string to_string(const NTargetFunc& value)
{
         if(value == NTargetFunc::NTargetMSE) {return "MSE";}
    else if(value == NTargetFunc::NTargetCross) {return "Cross";}
    else {return "None";}
}

string to_string(const NNormalizationFunc& value)
{
         if(value == NNormalizationFunc::NNormLinear) {return "Linear";}
    else if(value == NNormalizationFunc::NNormSoftStep) {return "SoftStep";}
    else if(value == NNormalizationFunc::NNormArctg) {return "Arctg";}
    else if(value == NNormalizationFunc::NNormGaussian) {return "Gaussian";}
    else {return "None";}
}

string to_string(const NCommandNet& value)
{
         if(value == NCommandNet::NComRunTrain) {return "RunTrain";}
    else if(value == NCommandNet::NComRunFunc) {return "RunFunc";}
    else if(value == NCommandNet::NComUpdateOut) {return "UpdateOut";}
    else if(value == NCommandNet::NComBeg) {return "BegNPT";}
    else if(value == NCommandNet::NComEnd) {return "EndNPT";}
    else if(value == NCommandNet::NComFinish) {return "FinishNPT";}
    else if(value == NCommandNet::NComBegExtern) {return "BegExtern";}
    else if(value == NCommandNet::NComEndExtern) {return "EndExtern";}
    else if(value == NCommandNet::NComEndTrain) {return "EndTrain";}
    else if(value == NCommandNet::NComNCSave) {return "NCSave";}
    else if(value == NCommandNet::NComIsSave) {return "IsSave";}
    else {return "None";}
}

string to_string(const NStatusNet& value)
{
    if(value == NStatusNet::NStatusCreate) {return "Create";}
    else if(value == NStatusNet::NStatusInit) {return "Init";}
    else if(value == NStatusNet::NStatusTrain) {return "Train";}
    else {return "None";}
}

string to_string(const NTrainType& value)
{
    if(value == NTrainType::NTrainBPType) {return "TrainBP";}
    else if(value == NTrainType::NAgentType) {return "Agent";}
    else {return "None";}
}

string to_string(const NOptimizerType& value)
{
    if(value == NOptimizerType::NOptimizerSGD) {return "SGD";}
    else if(value == NOptimizerType::NOptimizerAdam) {return "Adam";}
    else {return "None";}
}


bool equalf(float A, float B, unsigned int maxUlps)
{
    int aInt = *(reinterpret_cast<int*>(&A));
    int bInt = *(reinterpret_cast<int*>(&B));
    int cInt = static_cast<int>(0x80000000);
    // Уберем знак в aInt, если есть, чтобы получить правильно упорядоченную последовательность
    if(aInt < 0) {aInt = cInt - aInt;}
    //aInt &= 0x7fffffff;
    // Аналогично для bInt
    if(bInt < 0) {bInt = cInt - bInt;}
    /*aInt &= 0x7fffffff;*/
    unsigned int intDiff = static_cast<unsigned int>(abs(aInt - bInt));
    if(intDiff <= maxUlps) {return true;}
    return false;
}
