#include "pipefile.h"

PipeFile::PipeFile(const QString& name)
{
    this->name = name;
    this->path = "";
    this->ext = "txt";
    this->fblock = Q_NULLPTR;
    this->delim_data = ";";
    this->msec = 300;
    this->size_file = 0;
    //this->lenBuffer = 256;
    this->blLoop = true;
}

PipeFile::~PipeFile()
{
    if(this->fpipe.isOpen()) {this->close();}
}

void PipeFile::setPath(const QString& path)
{
    this->path = path;
}

void PipeFile::setDelimData(const QString& delim)
{
    this->delim_data = delim;
}

QString PipeFile::getDelimData()
{
    return this->delim_data;
}

void PipeFile::setMsec(unsigned long msec)
{
    this->msec = msec;
}


bool PipeFile::create()
{
    QString filename = this->path + "\\" + this->name + "." + this->ext;
    this->fpipe.setFileName(filename);
    bool bl = this->fpipe.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(bl) {this->updateSize();}
    if(bl) {this->fpipe.close();}
    return bl;
}

bool PipeFile::exists()
{
    QString filename = this->path + "\\" + this->name + "." + this->ext;
    this->fpipe.setFileName(filename);
    return this->fpipe.exists();
}

bool PipeFile::open()
{
    QString filename = this->path + "\\" + this->name + "." + this->ext;
    QString lockname = QDir::tempPath() + "\\" + this->name + ".lock";
    this->fpipe.setFileName(filename);
    this->fblock = new QLockFile(lockname);
    this->stream.setDevice(&this->fpipe);
    this->blLoop = true;
    bool bl = this->fpipe.open(QIODevice::Append | QIODevice::ReadOnly);
    if(bl) {this->updateSize();}
    return bl;
}

void PipeFile::close()
{
    delete this->fblock;
    this->fblock = Q_NULLPTR;
    this->fpipe.close();
}

bool PipeFile::isOpen()
{
    return this->fpipe.isOpen();
}

QString PipeFile::getPathLockName()
{
    return QDir::tempPath() + "\\" + this->name + ".lock";
}

bool PipeFile::read()
{
    bool bl = true;
    bl = this->lockFile() && bl;
    bl = bl && this->readFileInRBuf();
    bl = this->unlockFile() && bl;

    if(bl) {bl = this->readRBuf();} else {this->mes.deinit();}

    return bl;
}

bool PipeFile::write()
{
    bool bl = true;
    bl = this->lockFile() && bl;
    bl = bl && this->readFileInRBuf();
    bl = bl && this->writeWBufInFile();
    bl = bl && this->flush();
    if(bl) {this->updateSize();}
    bl = this->unlockFile() && bl;

    return bl;
}

bool PipeFile::readRBuf()
{
    if(!this->rbuf.empty())
    {
        this->mes = this->rbuf.dequeue();
        return true;
    }
    return false;
}

void PipeFile::writeWBuf()
{
    this->wbuf.enqueue(this->mes);
}

void PipeFile::writeWBufFormat(const QString& command)
{
    this->curtimeToMessage();
    this->mes.command = command;
    this->writeWBuf();
}

void PipeFile::writeWBufFormat(const QString& command, QStringList& param)
{
    this->curtimeToMessage();
    this->mes.command = command;
    this->mes.param = param;
    this->writeWBuf();
}

bool PipeFile::eof()
{
    return this->stream.atEnd();
}

void PipeFile::curtimeToMessage()
{
    char buffer[80];
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    this->mes.message = buffer;
}

bool PipeFile::isEmptyCommand()
{
    return ((this->mes.message == "")&&(this->mes.command == ""));
}

void PipeFile::waitForRead()
{
    this->waitForRead(this->msec);
}

void PipeFile::waitForRead(unsigned long wmsec)
{
    bool bl;
    unsigned long newfsize;

    this->lockFile();
    newfsize = this->fpipe.size();
    this->unlockFile();

    while(this->size_file == newfsize && this->rbuf.empty())
    {
        bl = this->lockFile();
        if(bl) {newfsize = this->fpipe.size();}
        bl = this->unlockFile();
        QThread::msleep(wmsec);
    }
    this->size_file = newfsize;
}

bool PipeFile::waitForLoop(unsigned long num)
{
    bool bl;
    unsigned long newfsize;
    //unsigned long num = wmsec / this->msec;

    this->lockFile();
    if(this->blLoop)
    {
        newfsize = this->fpipe.size();
        this->blLoop = false;
    }
    else
    {
        newfsize = this->fpipe.size();
    }
    this->unlockFile();

    for(unsigned long ind = 0; ind < num && this->size_file == newfsize && this->rbuf.empty(); ind++)
    {
        bl = this->lockFile();
        if(bl) {newfsize = this->fpipe.size();}
        bl = this->unlockFile();
        QThread::msleep(this->msec);
    }

    if(!(this->size_file == newfsize && this->rbuf.empty()))
    {
        this->size_file = newfsize;
        this->blLoop = true;
    }

    return this->blLoop;
}

unsigned long PipeFile::updateSizeWait()
{
    bool bl;

    bl = this->lockFile();
    if(bl) {this->updateSize();}
    bl = this->unlockFile();

    return this->size_file;
}

void PipeFile::parsing(QString& str)
{
    this->mes.parsing(str, this->delim_data);
}

QString PipeFile::toString()
{
    return this->mes.toString(this->delim_data + " ");
}

string PipeFile::toStdString()
{
    return this->mes.toStdString(this->delim_data + " ");
}


bool PipeFile::readFileInRBuf()
{
    QString str;
    PipeMessage vmes;

    while(this->getline(str))
    {
        vmes.parsing(str, this->delim_data);
        this->rbuf.enqueue(vmes);
    }

    return true;
}

bool PipeFile::writeWBufInFile()
{
    bool bl = true;

    while(!this->wbuf.empty())
    {
        if(bl)
        {
            bl = bl && this->setline(wbuf.dequeue().toString(this->delim_data + " "));
        }
    }

    return bl;
}

bool PipeFile::getline(QString& str)
{
    if(this->eof())
    {
        str = "";
        return false;
    }
    else
    {
        str = this->stream.readLine();
        if(stream.status() == QTextStream::Ok && str != "") {return true;}
        return false;
    }
}

bool PipeFile::setline(const QString& str)
{
    this->stream << str << endl;
    if(stream.status() == QTextStream::Ok) {return true;}
    return false;
}

bool PipeFile::flush()
{
    this->stream.flush();
    return this->fpipe.flush();
}

bool PipeFile::lockFile()
{
    return this->fblock->lock();
}

bool PipeFile::unlockFile()
{
    this->fblock->unlock();
    return true;
}

unsigned long PipeFile::updateSize()
{
    this->size_file = this->fpipe.size();
    return this->size_file;
}

void PipeFile::setFilePointer(unsigned long tell)
{
    this->stream.seek(tell);
}

unsigned long PipeFile::getFilePointer()
{
    return this->stream.pos();
}
