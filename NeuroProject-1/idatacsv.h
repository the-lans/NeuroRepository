#ifndef IDATACSV_H
#define IDATACSV_H

#include <QList>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <typeinfo>
#include "nmatrix.h"
#include "neuroproject.h"

template <typename NType>
class IDataCSV
{
public:
    IDataCSV();
protected:
    QList<QStringList> data;
    NMatrix<NType> mtrx;
    NTypeEnum type_value;
public:
    void read(QString name, QString delimetr);
    virtual void calculate() = 0;
    NMatrix<NType>& getMatrix();
};

#endif // IDATACSV_H
