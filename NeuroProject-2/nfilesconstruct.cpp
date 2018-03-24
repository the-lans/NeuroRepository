
#include "nfilesconstruct.h"

NFilesConstruct::NFilesConstruct()
{
    this->current_dir = "";
    this->prefix_tmp = "tmp_";

    this->name_examples = "";
    this->name_neuronet = "";
    this->name_trainbp = "";

    this->isFileExamples = false;
    this->isFileNeuronet = false;
    this->isFileTrainBP = false;
}

NFilesConstruct::NFilesConstruct(NFilesConstruct& obj)
{
    this->current_dir = obj.current_dir;
    this->prefix_tmp = obj.prefix_tmp;

    this->name_examples = obj.name_examples;
    this->name_neuronet = obj.name_neuronet;
    this->name_trainbp = obj.name_trainbp;

    this->isFileExamples = obj.isFileExamples;
    this->isFileNeuronet = obj.isFileNeuronet;
    this->isFileTrainBP = obj.isFileTrainBP;
}

NFilesConstruct& NFilesConstruct::operator=(NFilesConstruct& obj)
{
    this->current_dir = obj.current_dir;
    this->prefix_tmp = obj.prefix_tmp;

    this->name_examples = obj.name_examples;
    this->name_neuronet = obj.name_neuronet;
    this->name_trainbp = obj.name_trainbp;

    this->isFileExamples = obj.isFileExamples;
    this->isFileNeuronet = obj.isFileNeuronet;
    this->isFileTrainBP = obj.isFileTrainBP;

    return *this;
}


void NFilesConstruct::init()
{
    this->isFileExamples = false;
    this->isFileNeuronet = false;
    this->isFileTrainBP = false;

    this->dt_examples.clear();
    this->dt_neuronet.clear();
    this->dt_trainbp.clear();
}

void NFilesConstruct::fopen(ios_base::openmode mode)
{
    fexamples.open(current_dir + "\\" + name_examples, mode);
    fneuronet.open(current_dir + "\\" + name_neuronet, mode);
    ftrainbp.open(current_dir + "\\" + name_trainbp, mode);

    isFileExamples = fexamples.is_open();
    isFileNeuronet = fneuronet.is_open();
    isFileTrainBP = ftrainbp.is_open();
}

void NFilesConstruct::tmp_fopen(ios_base::openmode mode)
{
    tmp_fexamples.open(current_dir + "\\" + prefix_tmp + name_examples, mode);
    tmp_fneuronet.open(current_dir + "\\" + prefix_tmp + name_neuronet, mode);
    tmp_ftrainbp.open(current_dir + "\\" + prefix_tmp + name_trainbp, mode);
}

void NFilesConstruct::fclose()
{
    if(fexamples != nullptr && isFileExamples)
    {
        fexamples.close();
    }
    if(fneuronet != nullptr && isFileNeuronet)
    {
        fneuronet.close();
    }
    if(ftrainbp != nullptr && isFileTrainBP)
    {
        ftrainbp.close();
    }
}

void NFilesConstruct::tmp_fclose()
{
    if(tmp_fexamples != nullptr)
    {
        tmp_fexamples.close();
    }
    if(tmp_fneuronet != nullptr)
    {
        tmp_fneuronet.close();
    }
    if(tmp_ftrainbp != nullptr)
    {
        tmp_ftrainbp.close();
    }
}

void NFilesConstruct::fremove()
{
    string str;

    str = current_dir + "\\" + name_examples;
    remove(str.c_str());
    str = current_dir + "\\" + name_neuronet;
    remove(str.c_str());
    str = current_dir + "\\" + name_trainbp;
    remove(str.c_str());
}

void NFilesConstruct::tmp_fremove()
{
    string str;

    str = current_dir + "\\" + prefix_tmp + name_examples;
    remove(str.c_str());
    str = current_dir + "\\" + prefix_tmp + name_neuronet;
    remove(str.c_str());
    str = current_dir + "\\" + prefix_tmp + name_trainbp;
    remove(str.c_str());
}

bool NFilesConstruct::is_open()
{
    return (fexamples.is_open() && fneuronet.is_open() && ftrainbp.is_open());
}

bool NFilesConstruct::is_tmp_open()
{
    return (tmp_fexamples.is_open() && tmp_fneuronet.is_open() && tmp_ftrainbp.is_open());
}

bool NFilesConstruct::unionHeadBody()
{
    bool bl = true;

    bl = bl && dt_examples.unionHeadBody(current_dir + "\\" + name_examples, current_dir + "\\" + prefix_tmp + name_examples);
    bl = bl && dt_neuronet.unionHeadBody(current_dir + "\\" + name_neuronet, current_dir + "\\" + prefix_tmp + name_neuronet);
    bl = bl && dt_trainbp.unionHeadBody(current_dir + "\\" + name_trainbp, current_dir + "\\" + prefix_tmp + name_trainbp);

    return bl;
}
