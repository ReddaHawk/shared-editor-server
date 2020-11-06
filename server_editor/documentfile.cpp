#include "documentfile.h"
#include <QThread>

DocumentFile::DocumentFile(QFile *file) :file(file)
{
}

DocumentFile::~DocumentFile()
{
}

void DocumentFile::saveChanges(QVector<Symbol> symbols)
{
    qDebug() << this << QThread::currentThread() << "Saving changes to" << file->fileName();
    if(!file->open(QIODevice::WriteOnly))
        return;

    QDataStream out(file);

    foreach(Symbol symbol , symbols){
        out << symbol;
    }
    file->close();
}
