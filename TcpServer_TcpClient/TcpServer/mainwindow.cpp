#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Tim-TCP-服务器");
    server=new QTcpServer(this);
    connect(server,&QTcpServer::newConnection,this,[=](){
        QTcpSocket *tcp;
        tcp=server->nextPendingConnection();
        QString ip=tcp->peerAddress().toString();
        qint16 port =tcp->peerPort();
        index[tcp]="" ;
        QString message=ui->user->toPlainText();
        tcp->write(message.toUtf8());
        connect(tcp,&QTcpSocket::readyRead,this,[=](){
            QString recvMsg=tcp->readAll();
            qintptr descriptor1 = tcp->socketDescriptor();
            if(!recvMsg.contains('@'))
            {
                index[tcp]=recvMsg;
                for(auto [x,y]:index){
                    qintptr descriptor2 = x->socketDescriptor();
                    x->write(getUser().toUtf8());
                }
                ui->user->setText(getUser());
                return;
            }
            else{
                for(auto [x,y]:index){
                    qintptr descriptor2 = x->socketDescriptor();
                    if(descriptor2!=descriptor1)
                    x->write(recvMsg.toUtf8());
                }
                ui->record->append(recvMsg);
            }

        });
        connect(tcp, &QTcpSocket::disconnected, this, [=](){
            index.erase(tcp);
            ui->record->append("客户端已经断开了连接...");
            tcp->deleteLater();
            for(auto [x,y]:index){
                qintptr descriptor2 = x->socketDescriptor();
                x->write(getUser().toUtf8());
            }
            ui->user->setText(getUser());
        });
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_startButton_clicked()
{
    unsigned short port = ui->port->text().toInt();
    // 设置服务器监听
    server->listen(QHostAddress::Any, port);
    ui->startButton->setEnabled(false);
    ui->record->append("服务器开启成功，端口为:"+ui->port->text());
}
QString MainWindow::getUser()
{
    QString message="";
    for(auto [x,y]:index)
    {
        message=message+y+'\n';
    }
    return message;
}


void MainWindow::on_sendButton_clicked()
{
    QString message=ui->sendEdit->toPlainText();
    ui->sendEdit->clear();
    QString str = QDateTime::currentDateTime().toString("MM-dd hh:mm");
    QString title="Server "+str+"@"+message;
    ui->record->append(title);
    for(auto [x,y]:index)
    {
        x->write(title.toUtf8());
    }
    return;
}

