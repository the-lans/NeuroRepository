#include "neuroproject.h"

default_random_engine NRandGenerator;


void init_random_generator()
{
    random_device rd;
    NRandGenerator.seed(rd());
}

float deltatime(unsigned int start_time, unsigned int end_time)
{
    return ((float)end_time - (float)start_time)/1000;
}

void time_srand()
{
    std::srand((unsigned)std::time(0));
}

void to_value(NLayerType& value, string& str_val)
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

void to_value(NLayerDerivat& value, string& str_val)
{
    str_to_lower(str_val);
    if(str_val == "out") {value = NLayerDerivat::NDerivatOut;}
    else {value = NLayerDerivat::NDerivatSum;}
}

void to_value(NInitType& value, string& str_val)
{
    str_to_lower(str_val);
         if(str_val == "initempty") {value = NInitType::NInitEmpty;}
    else if(str_val == "initg") {value = NInitType::NInitG;}
    else {value = NInitType::NInitNone;}
}

void to_value(NSetType& value, string& str_val)
{
    str_to_lower(str_val);
         if(str_val == "train") {value = NSetType::NSetTrain;}
    else if(str_val == "test") {value = NSetType::NSetTest;}
    else if(str_val == "validate") {value = NSetType::NSetValidate;}
    else {value = NSetType::NSetNone;}
}

void to_value(NTargetFunc& value, string& str_val)
{
    str_to_lower(str_val);
         if(str_val == "mse") {value = NTargetFunc::NTargetMSE;}
    else if(str_val == "cross") {value = NTargetFunc::NTargetCross;}
    else {value = NTargetFunc::NTargetNone;}
}

void to_value(NNormalizationFunc& value, string& str_val)
{
    str_to_lower(str_val);
         if(str_val == "linear") {value = NNormalizationFunc::NNormLinear;}
    else if(str_val == "softstep") {value = NNormalizationFunc::NNormSoftStep;}
    else if(str_val == "arctg") {value = NNormalizationFunc::NNormArctg;}
    else if(str_val == "gaussian") {value = NNormalizationFunc::NNormGaussian;}
    else {value = NNormalizationFunc::NNormNone;}
}

string to_string(NLayerType& value)
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

string to_string(NLayerDerivat& value)
{
    if(value == NLayerDerivat::NDerivatOut) {return "Out";}
    else {return "Sum";}
}

string to_string(NInitType& value)
{
         if(value == NInitType::NInitEmpty) {return "InitEmpty";}
    else if(value == NInitType::NInitG) {return "InitG";}
    else {return "InitNone";}
}

string to_string(NSetType& value)
{
         if(value == NSetType::NSetTrain) {return "Train";}
    else if(value == NSetType::NSetTest) {return "Test";}
    else if(value == NSetType::NSetValidate) {return "Validate";}
    else {return "None";}
}

string to_string(NTargetFunc& value)
{
         if(value == NTargetFunc::NTargetMSE) {return "MSE";}
    else if(value == NTargetFunc::NTargetCross) {return "Cross";}
    else {return "None";}
}

string to_string(NNormalizationFunc& value)
{
         if(value == NNormalizationFunc::NNormLinear) {return "Linear";}
    else if(value == NNormalizationFunc::NNormSoftStep) {return "SoftStep";}
    else if(value == NNormalizationFunc::NNormArctg) {return "Arctg";}
    else if(value == NNormalizationFunc::NNormGaussian) {return "Gaussian";}
    else {return "None";}
}


bool equalf(float A, float B, unsigned int maxUlps)
{
    int aInt = *(int*)(&A);
    int bInt = *(int*)(&B);
    // Уберем знак в aInt, если есть, чтобы получить правильно упорядоченную последовательность
    if(aInt < 0) {aInt = 0x80000000 - aInt;}
    //aInt &= 0x7fffffff;
    // Аналогично для bInt
    if(bInt < 0) {bInt = 0x80000000 - bInt;}
    /*aInt &= 0x7fffffff;*/
    unsigned int intDiff = abs(aInt - bInt);
    if(intDiff <= maxUlps) {return true;}
    return false;
}
