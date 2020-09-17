#include "mainwindow.h"
#include "db.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (!QSqlDatabase::drivers().contains("QMYSQL"))
    {
            QMessageBox::critical(
                        this,
                        "Unable to load database",
                        "This application needs the QMYSQL driver"
                        );
            this->close();
    }
    else
    {
        //ui->page_3->show();
        ui->stackedWidget->setCurrentIndex(0);
        ui->lineEdit_hostname->setText("127.0.0.1");
        ui->lineEdit_dbname->setText("serverDB");
        ui->lineEdit_port->setText("3306");
        ui->lineEdit_username->setText("root");
        ui->lineEdit_password->setText("mypass");
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);


    }

}

MainWindow::~MainWindow()
{
    closeDb();
    delete ui;
}



void MainWindow::on_pushButton_logindb_clicked()
{
    int indexWindow = ui->stackedWidget->currentIndex();
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString hostname = ui->lineEdit_hostname->text();
    QString dbname = ui->lineEdit_dbname->text();
    QString port = ui->lineEdit_port->text();

    QSqlError err = initDb(hostname,dbname,port,username,password);

    if (err.type() != QSqlError::NoError)
    {
        QMessageBox::critical(this,"dbError",err.text());
        //this->close();

    }
    else
        {
         //Db disponibile
         QMessageBox::information(this, "Login","ok");
         ui->stackedWidget->setCurrentIndex(++indexWindow);
        //db.close();
        }

}


void MainWindow::on_pushButton_startServer_clicked()
{
    if(m_tcpServer.listen(QHostAddress::Any,ui->portNumber->value()))
        {
            qDebug() << this << "server started!";
            setStarted(true);
        }
        else
        {
            qCritical() << m_tcpServer.errorString();
            QMessageBox::critical(this,"Server error","Try to change port");
            setStarted(false);
        }

}

void MainWindow::setStarted(bool started)
{
    ui->pushButton_startServer->setEnabled(!started);
    ui->pushButton_stopServer->setEnabled(started);
}

void MainWindow::on_pushButton_stopServer_clicked()
{
        m_tcpServer.close();
        setStarted(false);
}
