#ifndef STRUCTECSV_H
#define STRUCTECSV_H

//#include "narray.h"
#include "nmatrix.h"
#include <string>

using namespace std;

enum class TypeDataECSV {Element, String};

struct StructECSV
{
    NArray<string> path;
    NArray<string> type;
    NMatrix<string> mtrx;
    NArray<TypeDataECSV> typeECSV;
};

#endif // STRUCTECSV_H
