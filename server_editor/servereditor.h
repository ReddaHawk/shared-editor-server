#ifndef SERVEREDITOR_H
#define SERVEREDITOR_H

#include <QObject>
#include <QChar>
#include <QString>
#include <QVector>
#include <QUuid>

#include "editingmessage.h"
#include "symbol.h"

class ServerEditor : public QObject
{
    Q_OBJECT

public:
    ServerEditor();
    QUuid getSiteId();
    QUuid getSymbolSiteId(int index);
    QTextCharFormat getSymbolFormat(int index);
    void process(const EditingMessage& m);
    void remoteInsert(Symbol sym);
    void remoteDelete(Symbol sym);
    QString to_string();
    int symbolCount();
    QVector<Symbol> getSymbols();
    void setSymbols(QVector<Symbol>& symbols);
signals:
    void remoteCharInserted(QUuid remoteSiteId, QChar value, QTextCharFormat charFormat, QTextBlockFormat blockFormat, int index);
    void remoteCharDeleted(QUuid remoteSiteId, int index);

private:
    QUuid _siteId;
    QVector<Symbol> _symbols;
    int _counter=1;

    Symbol generateSymbol(QChar value, QTextCharFormat charFormat, QTextBlockFormat blockFormat, int index);
    void generateIndexBetween(Symbol &sym1, int pos1, Symbol &sym2, int pos2, QVector<int> &newFractIndex);
    auto findInsertIndex(const Symbol &sym);
    auto findIndexByPos(const Symbol &sym);
};


#endif //SERVEREDITOR_H
