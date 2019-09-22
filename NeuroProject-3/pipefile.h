#ifndef PIPEFILE_H
#define PIPEFILE_H

#include "qfuncsmart.h"
#include "pipemessage.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>

//#include <QString>
#include <QQueue>
#include <QLockFile>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QThread>

using namespace std;

class PipeFile
{
public:
    PipeFile(const QString& name);
    ~PipeFile();

public:
    PipeMessage mes; //Текущее сообщение

protected:
    QQueue<PipeMessage> rbuf; //Очередь буфера сообщений (чтение)
    QQueue<PipeMessage> wbuf; //Очередь буфера сообщений (запись)
    QString name; //Имя канала
    QString path; //Рабочая папка
    QString ext; //Расширение файла
    QFile fpipe; //Файл-поток
    QTextStream stream; //Текстовый поток
    QLockFile* fblock; //Файл для блокировки
    QString delim_data; //Разделитель данных
    unsigned long msec; //Ожидание в миллисекундах
    unsigned long size_file; //Размер файла для чтения
    //unsigned long lenBuffer; //Размер буфера для чтения
    bool blLoop; //Продолжение цикла ожидания

public:
    void setPath(const QString& path);
    void setDelimData(const QString& delim);
    QString getDelimData();
    void setMsec(unsigned long msec);

public:
    bool create();
    bool exists();
    bool open();
    void close();
    bool isOpen();
    QString getPathLockName();
    bool read();
    bool write();
    bool readRBuf(); //Чтение буфера (чтение)
    void writeWBuf(); //Запись в буфер (запись)
    void writeWBufFormat(const QString& command);
    void writeWBufFormat(const QString& command, QStringList& param);
    bool eof();
    void curtimeToMessage();
    bool isEmptyCommand();
    void waitForRead();
    void waitForRead(unsigned long wmsec);
    bool waitForLoop(unsigned long num);
    unsigned long updateSizeWait();
    void parsing(QString& str);
    QString toString();
    string toStdString();

protected:
    bool readFileInRBuf(); //Чтение файла в буфер
    bool writeWBufInFile(); //Запись буфера в файл
    bool getline(QString& str);
    bool setline(const QString& str);
    bool flush();
    bool lockFile();
    bool unlockFile();
    unsigned long updateSize();
    void setFilePointer(unsigned long tell);
    unsigned long getFilePointer();
};

#endif // PIPEFILE_H
