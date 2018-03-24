#include "iobjectecsv.h"

void IObjectECSV::pushFieldNotSave(string& parent, const string& field)
{
     this->fieldsNotSave.push_back(getpath(parent, field));
}
