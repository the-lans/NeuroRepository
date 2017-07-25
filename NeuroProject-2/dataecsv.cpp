#include "dataecsv.h"

/*void DataECSV::addElement(string parent, string field, double value, string type)
{
    string str = (string) value;
    str.replace("\"", "\\\"");
    str.replace("\\", "\\\\");
    str.replace(";", "\\;");
    StructECSV element;
    element.mtrx.init(1, 1, 0);
    element.path.push(parent);
    element.path.push(field);
    element.type.push(type);
    element.mtrx.set(0, 0, str);
    element.typeECSV.push(TypeDataECSV::Element);
}

void DataECSV::addString(string parent, string field, string value, string type)
{
    value.replace("\"", "\\\"");
    value.replace("\\", "\\\\");
    value.replace(";", "\\;");
    string str ="\"" + value + "\"";
    StructECSV element;
    element.mtrx.init(1, 1, 0);
    element.path.push(parent);
    element.path.push(field);
    element.type.push(type);
    element.mtrx.set(0, 0, str);
    element.typeECSV.push(TypeDataECSV::String);
}*/
