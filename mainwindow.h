#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QStackedWidget>
#include "db.h"
#include "network/tcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_logindb_clicked();

    void on_pushButton_startServer_clicked();

    void on_pushButton_stopServer_clicked();

private:
    Ui::MainWindow *ui;
    TcpServer m_tcpServer;
    void setStarted(bool started);
};
#endif // MAINWINDOW_H
