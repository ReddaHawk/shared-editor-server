#include "servereditor.h"

ServerEditor::ServerEditor() {
    this->_siteId = -1;
    Symbol newSym = generateSymbol(QChar::ParagraphSeparator, QTextCharFormat(), QTextBlockFormat(), 0);
    auto it = _symbols.begin();
    _symbols.insert(it, newSym);
}

ServerEditor::ServerEditor(QVector<Symbol> &symbols) :
    _siteId(-1),
    _symbols(symbols)
{
}

int ServerEditor::getSiteId() {
    return _siteId;
}

int ServerEditor::getSymbolSiteId(int index) {
    auto it = _symbols.begin();
    it+=index;
    return it->getSiteId();
}

QTextCharFormat ServerEditor::getSymbolFormat(int index) {
    auto it = _symbols.begin();
    it+=index;
    return it->getCharFormat();
}

Symbol ServerEditor::generateSymbol(QChar value, QTextCharFormat charFormat, QTextBlockFormat blockFormat, int index) {
    QVector<int> newFractIndex;
    if (index == 0) {
        if (_symbols.empty()) {
            newFractIndex.push_back(1);
        } else {
            Symbol sym2 = _symbols[index];
            QVector<int> startFractIndex;
            int size = sym2.fractIndexSize();
            for (int i = 0; i < size - 1; ++i) {
                startFractIndex.push_back(0);
            }
            Symbol sym1('$', QTextCharFormat(), QTextBlockFormat(), this->_siteId, 0, startFractIndex);
            generateIndexBetween(sym1, 0, sym2, 0, newFractIndex);
        }
    } else if (_symbols.size() == index) {
        Symbol sym = _symbols[index - 1];
        newFractIndex.push_back(sym.getFractIndexDigit(0) + 1);
    } else {
        Symbol sym2 = _symbols[index];
        Symbol sym1 = _symbols[index - 1];
        generateIndexBetween(sym1, 0, sym2, 0, newFractIndex);
    }

    return Symbol(value, charFormat, blockFormat, this->_siteId, this->_counter++, newFractIndex);
}

void ServerEditor::generateIndexBetween(Symbol &sym1, int pos1, Symbol &sym2, int pos2, QVector<int> &newFractIndex) {
    int digit1 = sym1.getFractIndexDigit(pos1);
    int digit2 = sym2.getFractIndexDigit(pos2);

    if (digit1 == -1 && digit2 == -1) {
        // [ x , ... , y ]       [ x , ... , z ]       =>   [ x , ... , y , 1 ]
        newFractIndex.push_back(1);

    } else if (digit1 == -1) {
        // [ x , ... , y ]       [ x , ... , y , z ]   =>   [ x , ... , y , 0 , 1 ]
        newFractIndex.push_back(0);
        newFractIndex.push_back(1);

    } else if (digit2 == -1) {
        // [ x , ... , y , 2 ]   [ x , ... , y ]       =>   [ x , ... , y , 3 ]
        newFractIndex.push_back(digit1+1);

    } else if (digit2 - digit1 > 1) {
        // [ x , ... , y , 1 ]   [ x , ... , y , 4 ]   =>   [ x , ... , y , 2 ]
        newFractIndex.push_back(digit1+1);

    } else if (digit2 - digit1 == 1) {
        // [ x , ... , y , 3 ]   [ x , ... , y , 4 ]   =>   [ x , ... , y , 3 , ? ] + recursion
        newFractIndex.push_back(digit1);
        generateIndexBetween(sym1, pos1+1, sym2, pos2+1, newFractIndex);

    } else if (digit2 - digit1 == 0) {
        // [ x , ... , y , 2 ]   [ x , ... , y , 2 ]   =>   [ x , ... , y , 2 ] + recursion
        newFractIndex.push_back(digit1);
        generateIndexBetween(sym1, pos1+1, sym2, pos2+1, newFractIndex);
    }
}

auto ServerEditor::findInsertIndex(const Symbol &sym) {
    auto it = _symbols.begin();

    while (it != _symbols.end() && it->getFractIndex() < sym.getFractIndex()){
        it++;
    }

    return it;
}

auto ServerEditor::findIndexByPos(const Symbol &sym) {
    auto it = _symbols.begin();

    while (it != _symbols.end() && it->getFractIndex() != sym.getFractIndex()) {
        it++;
    }

    return it;
}


QString ServerEditor::to_string() {
    QString str;
    for (auto it = _symbols.begin(); it != _symbols.end(); it++) {
        str.push_back(it->getValue());
    }
    return str;
}

void ServerEditor::remoteInsert(Symbol sym) {
    auto index = findInsertIndex(sym);
    remoteCharInserted(sym.getSiteId(), sym.getValue(), sym.getCharFormat(), sym.getBlockFormat(), index - _symbols.begin());
    _symbols.insert(index, sym);
}

void ServerEditor::remoteDelete(Symbol sym) {
    auto index = findIndexByPos(sym);
    remoteCharDeleted(sym.getSiteId(), index - _symbols.begin());
    _symbols.erase(index);
}

void ServerEditor::process(const EditingMessage &m) {
    if (m.getOperation() == MSG_INSERT) {
        remoteInsert(m.getSymbol());
    } else if (m.getOperation() == MSG_ERASE) {
        remoteDelete(m.getSymbol());
    }
}

int ServerEditor::symbolCount() {
    return _symbols.size();
}
