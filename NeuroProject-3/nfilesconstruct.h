#ifndef NFILESCONSTRUCT_H
#define NFILESCONSTRUCT_H

#include <string>
#include <vector>
#include <fstream>
#include "dataecsv.h"

using namespace std;

class NFilesConstruct
{
public:
    string current_dir; //Текущая директория
    string prefix_tmp; //Префикс для временного файла или файла с данными

    string name_examples;
    string name_neuronet;
    string name_trainbp;

    bool isFileExamples;
    bool isFileNeuronet;
    bool isFileTrainBP;

    fstream fexamples;
    fstream fneuronet;
    fstream ftrainbp;
    fstream tmp_fexamples;
    fstream tmp_fneuronet;
    fstream tmp_ftrainbp;

    DataECSV dt_examples;
    DataECSV dt_neuronet;
    DataECSV dt_trainbp;

public:
    NFilesConstruct();
    NFilesConstruct(NFilesConstruct& obj);
    NFilesConstruct& operator=(NFilesConstruct& obj);

public:
    void init();
    void fopen(ios_base::openmode mode);
    void tmp_fopen(ios_base::openmode mode);
    void fclose();
    void tmp_fclose();
    void fremove();
    void tmp_fremove();
    bool is_open();
    bool is_tmp_open();
    bool unionHeadBody();
};

#endif // NFILESCONSTRUCT_H
