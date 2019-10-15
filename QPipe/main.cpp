#include <QCoreApplication>
#include <iostream>
#include "pipefile.h"

using namespace std;

int main(int argc, char *argv[])
{
    QString number;
    QCoreApplication a(argc, argv);
    PipeFile ps("chanel");
    ps.setPath("C:\\Ilya\\Flash\\!ProgramProjects\\NeuroRepository\\QPipe\\");

    cout << "Begin" << endl;
    if(!ps.exists()) {ps.create();}
    cout << "Create" << endl;
    ps.open();
    cout << "Open " << ps.getPathLockName().toStdString() << endl;

    for(int ind = 1; ind <= 10; ind++)
    {
        ps.curtimeToMessage();
        number.setNum(ind);
        ps.mes.command = "Command " + number;
        ps.writeWBuf();
        ps.mes.command += "_t";
        ps.writeWBuf();
        ps.write();
        cout << "Write command" << endl;
        ps.waitForRead();

        while(ps.read())
        {
            cout << ">> " << ps.toStdString() << endl;
        }
    }

    ps.close();
    cout << "Close" << endl;
    cout << "End" << endl;

    return a.exec();
}
