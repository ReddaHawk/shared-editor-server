#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qDebug()<<"Prima del show";
    w.show();
    qDebug()<<"dopo del show";

    return a.exec();
}
