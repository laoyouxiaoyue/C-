#ifndef WIDGET_H
#define WIDGET_H
#include <QTextToSpeech>
#include <QWidget>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include "day.h"
#include <vector>
#include <QPainter>
#include <QCompleter>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void mousePressEvent(QMouseEvent* ev)override
    {
        if(ev->button()==Qt::LeftButton)//这里判断，是否是鼠标左键按下，只有左键按下窗口才能移动
        {
            pos=ev->pos();//用成员变量记录下按下左键时鼠标的坐标
        }
    }
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent*ev)override
    {
        if(ev->buttons()==Qt::LeftButton)//注意，这里用的是buttons，因为buttons里存储了之前的按键信息
        {
            int x,y;
            x=ev->pos().x()-pos.x();
            y=ev->pos().y()-pos.y();
            this->move(this->x()+x,this->y()+y);
        }
    }
protected:
    QPoint pos;
    bool eventFilter(QObject* watched,QEvent* event);
    void paintHighCurve();
    void paintLowCurve();
private:
    Ui::Widget *ui;
    QMenu *ExitMenu;//右键菜单
    QAction *ExitAct;//右键退出
    void initMenu();//右键退出菜单
    vector<QLabel*>time;
    vector<QLabel*>quilaty;
    vector<QLabel*>ff;
    vector<QLabel*>types;
    vector<QLabel*>typest;
    QCompleter *completer;
    void initLabel();
    QNetworkAccessManager *qam;
    void onReplied(QNetworkReply* reply);
    void initCity();
public:
    void contextMenuEvent(QContextMenuEvent *event);
    void getWeatherInfo(QString cityCode);
    void parseJson(QByteArray &bytearray);
    void updateUI();
    QStringList citynameList;
    Day days[10];
    map<QString,QString>city;
    map<QString,QString>weather_type;
    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);  //构造画家
        p.drawPixmap(rect(),QPixmap(":/new/prefix1/res/background.png"));//修改图片地址和名称即可
    }
private slots:
    void on_pushButton_clicked();
};

#endif // WIDGET_H
