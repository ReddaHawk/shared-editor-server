#include "documentfile.h"

DocumentFile::DocumentFile(QFile *file) :file(file)
{

}

DocumentFile::~DocumentFile()
{
    delete file;
}

void DocumentFile::saveChanges(QVector<Symbol> symbols)
{
    if(!file->open(QIODevice::WriteOnly))
        return;

    // QTextStream is better?

    QDataStream out(file);

    foreach(Symbol symbol , symbols){
        out << symbol;
    }
    file->close();
}
