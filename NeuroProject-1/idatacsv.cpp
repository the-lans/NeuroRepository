#include "idatacsv.h"

template <typename NType>
IDataCSV<NType>::IDataCSV()
{
    type_value = NTypeEnum::NTypeFloat; // float
    if(typeid(NType) == typeid(double)) {type_value = NTypeEnum::NTypeDouble;} //double
    else if(typeid(NType) == typeid(int)) {type_value = NTypeEnum::NTypeInt;} //int
}

template <typename NType>
void IDataCSV<NType>::read(QString name, QString delimetr)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream stream(&file);
    QString str = stream.readAll();

    QStringList strings = str.split("\n");
    this->data.clear();
    this->mtrx.init(0, 0, 0);

    for(const QString& curStr: strings){
        if (!curStr.isEmpty()) {
            QStringList items = curStr.split(delimetr);
            for(QString& item: items){
                item = item.trimmed();
            }
            this->data.append(items);
        }
    }

    file.close();
}

template <typename NType>
NMatrix<NType>& IDataCSV<NType>::getMatrix()
{
    return this->mtrx;
}
