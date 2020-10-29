#ifndef DOCUMENTFILE_H
#define DOCUMENTFILE_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QUuid>
#include "symbol.h"
class DocumentFile : public QObject
{
    Q_OBJECT
private:
    QFile *file;
public:
    DocumentFile(QFile *file);
    ~DocumentFile();
public slots:
    void saveChanges(QVector<Symbol> symbols);
};

#endif // DOCUMENTFILE_H
