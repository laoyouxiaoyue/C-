#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QThread>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
    setWindowTitle("Tim-tcp-客户端测试");
    srand(time(0));
    QString s=QString::number(rand());
    ui->name->setText("游客"+s);
    tcp=new QTcpSocket(this);
    cursor=QTextCursor(ui->record->document());
    connect(tcp,&QTcpSocket::readyRead,[=](){
        QByteArray recvMsg=tcp->readAll();
        QString message=QString(recvMsg);
        if(!recvMsg.contains('@')){
            ui->user->setText(recvMsg);
            return;
        }
        QStringList stringList =message.split("@");
        QString name=stringList[0];
        message=stringList[1];
        ui->record->setTextColor(Qt::blue);
        ui->record->append(name);
        ui->record->setTextColor(Qt::black);
        ui->record->append(message);
    });
    connect(tcp,&QTcpSocket::connected,this,[=](){
        QString names=ui->name->text();
        if(names.contains('@'))
        {
            QMessageBox::warning(nullptr, "警告", "用户名中含有非法字符@！", QMessageBox::Ok);
            return;
        }
        flag=1;
        QString message=names;
        tcp->write(message.toUtf8());
        ui->startButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
        ui->name->setReadOnly(true);
        ui->port->setReadOnly(true);
        ui->ip->setReadOnly(true);
    });
    connect(tcp,&QTcpSocket::disconnected,this,[=](){
        flag=0;
        QMessageBox::warning(nullptr, "提示", "已与服务器连接失败！", QMessageBox::Ok);
        ui->startButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
        return;
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startButton_clicked()
{
    QString ip=ui->ip->text();
    unsigned short port=ui->port->text().toInt();
    tcp->connectToHost(QHostAddress(ip),port);
    if(!tcp->waitForConnected())
    {
        QMessageBox::warning(nullptr, "提示", "与服务器连接失败！", QMessageBox::Ok);
    }
    else{
        QMessageBox::information(nullptr,"提示","与服务器连接成功!",QMessageBox::Ok);
    }
}


void MainWindow::on_sendButton_clicked()
{
    QString message=ui->sendMessage->toPlainText();
    QString str = QDateTime::currentDateTime().toString("MM-dd hh:mm");
    ui->sendMessage->clear();
    QString title=ui->name->text()+" "+str;
    message=message;
    ui->record->setTextColor(Qt::green);
    ui->record->append(title);
    ui->record->setTextColor(Qt::black);
    ui->record->append(message);
    message=title+"@"+message;
    tcp->write(message.toUtf8());
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->user->clear();
    tcp->close();
    ui->startButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->name->setReadOnly(false);
    ui->port->setReadOnly(false);
    ui->ip->setReadOnly(false);
}

