#include <iostream>
#include "dataecsv.h"
#include "tempclass.h"

using namespace std;

int main()
{
    //Доступ к данным
    DataECSV dt;
    //string path = "E:\\!ProgramProjects\\NeuroProject\\ProjectECSV\\files\\";
    string path = "C:\\Ilya\\Flash\\!ProgramProjects\\NeuroRepository\\ProjectECSV\\files\\";
    string parent = ".base";

    //Объекты
    TempClass obj;
    //SubClass sb;
    //obj.color.push_back(EnumColor::ColorBlue);
    //obj.color.push_back(EnumColor::ColorRed);
    //sb.vec.push_back(123); sb.vec.push_back(753);
    //obj.sub.push_back(sb);

    //Чтение объекта
    cout << "Read..." << endl;
    dt.read(path + "input.txt");
    obj.loadECSV(dt, parent);
    dt.clear();

    //Запись объекта
    cout << "Write..." << endl;
    obj.saveECSV(dt, parent);
    dt.write(path + "output.txt");
    cout << "Complit!" << endl;

    return 0;
}
