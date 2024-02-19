#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QTextCharFormat>
#include <QThread>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fs=new QTextCharFormat();
    ui->pushButton_2->setEnabled(false);
    for(int i=9;i<=22;i++)
    {
        ui->comboBox->addItem(QString::number(i));
    }
    setWindowTitle("Tim-tcp-客户端测试");
    cursor = ui->sendMessage->textCursor();
    srand(time(0));
    QString s=QString::number(rand());
    ui->name->setText("游客"+s);
    tcp=new QTcpSocket(this);
    cursor=QTextCursor(ui->sendMessage->document());
    cursor.select(QTextCursor::Document);
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
    connect(ui->B,&QPushButton::clicked,this,&MainWindow::BIU);
    connect(ui->I,&QPushButton::clicked,this,&MainWindow::BIU);
    connect(ui->U,&QPushButton::clicked,this,&MainWindow::BIU);
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
    QString message=ui->sendMessage->toHtml();
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

void MainWindow::BIU()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button==ui->B)
    {
        if(Bflag) button->setStyleSheet(""),Bflag=0,fs->setFontWeight(QFont::Normal);
        else button->setStyleSheet("background-color:white;"),Bflag=1,fs->setFontWeight(QFont::Bold);
    }
    else if(button==ui->I)
    {
        if(Iflag) button->setStyleSheet(""),Iflag=0,fs->setFontItalic(false);
        else button->setStyleSheet("background-color:white;"),Iflag=1,fs->setFontItalic(true);;
    }
    else if(button==ui->U)
    {
        if(Uflag) button->setStyleSheet(""),Iflag=0,fs->setFontUnderline(false);
        else button->setStyleSheet("background-color:white;"),Iflag=1,fs->setFontUnderline(true);
    }
    cursor.mergeCharFormat(*fs);
    ui->sendMessage->setTextCursor(cursor);
}
void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{

    fs->setFont(f);
    ui->sendMessage->setTextCursor(cursor);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{

}
void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    int num=arg1.toInt();
    fs->setFontPointSize(num);
    cursor.mergeCharFormat(*fs);
    ui->sendMessage->setTextCursor(cursor);
}

