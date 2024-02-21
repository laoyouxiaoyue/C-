#include "widget.h"
#include "ui_widget.h"
#include <QMenu>
#include <QAction>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QPainter>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    initMenu(); //初始哈右键退出菜单
    initLabel();
    initCity();
    completer=new QCompleter(citynameList);
    completer->setFilterMode(Qt::MatchContains);
    ui->cityname->setCompleter(completer);
    this->setStyleSheet("#Widget{border-image:url(:/new/prefix1/res/background.png);}QLabel{border-radius:4px;color:rgb(255,255,255);}");
    qam=new QNetworkAccessManager();
    connect(qam,&QNetworkAccessManager::finished,this,&Widget::onReplied);
    getWeatherInfo("101010100");
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->lblHighCurve&&event->type()==QEvent::Paint)
    {
        paintHighCurve();
    }
    if(watched==ui->lblLowCurve&&event->type()==QEvent::Paint)
    {
        paintLowCurve();
    }
    return QWidget::eventFilter(watched,event);
}

void Widget::paintHighCurve()
{
     QPainter painter(ui->lblHighCurve);
     painter.setRenderHint(QPainter::Antialiasing,true);

    int pointX[6]={0};
    int pointY[6]={0};
    int tempSum=0;
    for(int i=0;i<5;i++)
    {
        tempSum+=days[i].high.split("℃")[0].toInt();
       pointX[i]=ff[i]->pos().x()+(ff[i]->width()/2);
        //qDebug()<<ff[i]->pos().x()<<'\n';
    }
   tempSum/=5;

    int yCenter=ui->lblHighCurve->height()/2;
    for(int i=0;i<5;i++)
    {
        pointY[i]=yCenter-(days[i].high.split("℃")[0].toInt()-tempSum)*3;
        qDebug()<<yCenter<<" "<<days[i].high.split("℃")[0].toInt()<<"\n";
        qDebug()<<pointY[i]<<'\n';
    }

    QPen pen=painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(255,170,0));
    painter.setPen(pen);
    painter.setBrush(QColor(255,170,0));
    for(int i=0;i<5;i++)
    {
        QString s=days[i].high;
        painter.drawText(pointX[i],pointY[i],s);
        painter.drawEllipse(QPoint(pointX[i],pointY[i]),3,3);
    }
    for(int i=0;i<4;i++)
    {
        if(i==0)
        {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        }
        else{
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
    }
}

void Widget::paintLowCurve()
{
    QPainter painter(ui->lblLowCurve);
    painter.setRenderHint(QPainter::Antialiasing,true);

    int pointX[6]={0};
    int pointY[6]={0};
    int tempSum=0;
    for(int i=0;i<5;i++)
    {
        tempSum+=days[i].low.split("℃")[0].toInt();
        pointX[i]=ff[i]->pos().x()+(ff[i]->width()/2);
            //qDebug()<<ff[i]->pos().x()<<'\n';
    }
    tempSum/=5;

    int yCenter=ui->lblHighCurve->height()/2;
    for(int i=0;i<5;i++)
    {
        pointY[i]=yCenter-(days[i].low.split("℃")[0].toInt()-tempSum)*3;
        qDebug()<<yCenter<<" "<<days[i].low.split("℃")[0].toInt()<<"\n";
        qDebug()<<pointY[i]<<'\n';
    }

    QPen pen=painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(0,255,255));
    painter.setPen(pen);
    painter.setBrush(QColor(0,255,255));
    for(int i=0;i<5;i++)
    {
        QString s=days[i].low;
        painter.drawText(pointX[i],pointY[i],s);
        painter.drawEllipse(QPoint(pointX[i],pointY[i]),3,3);
    }
    for(int i=0;i<4;i++)
    {
        if(i==0)
        {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        }
        else{
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
    }
}
void Widget::initLabel()
{
    weather_type["暴雪"]="BaoXue";
    weather_type["暴雨"]="BaoYu";
    weather_type["暴雨到大暴雨"]="BaoYuDaoDaBaoYu";
    weather_type["大暴雨"]="DaBaoYu";
    weather_type["大到暴雪"]="DaDaoBaoXue";
    weather_type["大暴雨到特大暴雨"]="DaBaoYuDaoTeDaBaoYu";
    weather_type["大雪"]="DaXue";
    weather_type["大到暴雨"]="DaDaoBaoYu";
    weather_type["大雨"]="DaYu";
    weather_type["冻雨"]="DongYu";
    weather_type["多云"]="DuoYun";
    weather_type["浮尘"]="FuChen";
    weather_type["雷阵雨"]="LeiZhenYu";
    weather_type["雷震雨伴有冰雹"]="LeiZhenYuBanYouBingBao";
    weather_type["霾"]="Mai";
    weather_type["强沙尘暴"]="QiangShaChenBao";
    weather_type["晴"]="Qing";
    weather_type["沙尘暴"]="ShaChenBao";
    weather_type["特大暴雨"]="TeDaBaoYu";
    weather_type["雾"]="Wu";
    weather_type["小到中雪"]="XiaoDaoZhongXue";
    weather_type["小到中雨"]="XiaoDaoZhongYu";
    weather_type["小雪"]="XiaoXue";
    weather_type["小雨"]="XiaoYu";
    weather_type["雪"]="Xue";
    weather_type["扬沙"]="YangSha";
    weather_type["阴"]="Yin";
    weather_type["雨"]="Yu";
    weather_type["雨夹雪"]="YuJiaXue";
    weather_type["阵雪"]="ZhenXue";
    weather_type["阵雨"]="ZhenYu";
    weather_type["中到大雪"]="ZhongDaoDaXue";
    weather_type["中到大雨"]="ZhongDaoDaYu";
    weather_type["中雪"]="ZhongXue";
    weather_type["中雨"]="ZhongYu";
    time.push_back(ui->d1);
    time.push_back(ui->d2);
    time.push_back(ui->d3);
    time.push_back(ui->d4);
    time.push_back(ui->d5);
    types.push_back(ui->d1p);
    types.push_back(ui->d2p);
    types.push_back(ui->d3p);
    types.push_back(ui->d4p);
    types.push_back(ui->d5p);
    typest.push_back(ui->d1pc);
    typest.push_back(ui->d2pc);
    typest.push_back(ui->d3pc);
    typest.push_back(ui->d4pc);
    typest.push_back(ui->d5pc);
    quilaty.push_back(ui->d1c);
    quilaty.push_back(ui->d2c);
    quilaty.push_back(ui->d3c);
    quilaty.push_back(ui->d4c);
    quilaty.push_back(ui->d5c);
    ff.push_back(ui->d1f);
    ff.push_back(ui->d2f);
    ff.push_back(ui->d3f);
    ff.push_back(ui->d4f);
    ff.push_back(ui->d5f);
    ui->lblHighCurve->installEventFilter(this);
    ui->lblLowCurve->installEventFilter(this);
}
void Widget::initMenu()
{
    ExitMenu=new QMenu(this);
    ExitAct=new QAction();
    ExitAct->setText("退出");
    ExitAct->setIcon(QIcon(":/new/prefix1/res/close.png"));
    ExitMenu->addAction(ExitAct);
    connect(ExitAct,&QAction::triggered,this,[=](){
        qApp->exit(0);
    });
}

void Widget::onReplied(QNetworkReply *reply)
{
    int status_code=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error()!=QNetworkReply::NoError||status_code!=200)
    {
        QMessageBox::warning(NULL,"警告","提取失败",QMessageBox::Ok);
    }
    else{
        QByteArray json=reply->readAll();
        parseJson(json);
    }

    reply->deleteLater();
}

void Widget::initCity()
{
    QFile qf(":/citycode.json");
    qf.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray qbt=qf.readAll();
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(qbt,&err);
    if(err.error!=QJsonParseError::NoError)
    {
        return;
    }
    QJsonArray obj=doc.array();
    for(int i=0;i<obj.size();i++)
    {
        QJsonObject qobj=obj[i].toObject();
        QString cityname=qobj.value("city_name").toString();
        QString code=qobj.value("city_code").toString();
        if(code!="")
        {
            citynameList<<cityname;
            city[cityname+"市"]=code;
            city[cityname]=code;
        }
    }
   // qDebug()<<obj;
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    ExitMenu->exec(QCursor::pos());
    event->accept();
}

void Widget::getWeatherInfo(QString cityCode)
{
    QString url="http://t.weather.itboy.net/api/weather/city/"+cityCode;
    qam->get(QNetworkRequest(url));
}

void Widget::parseJson(QByteArray &bytearray)
{
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(bytearray,&err);
    if(err.error!=QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject obj=doc.object();
    QJsonObject data=obj.value("data").toObject();

    days[1].wendu=data.value("wendu").toString();
    days[1].shidu=data.value("shidu").toString();
    days[1].ganmao=data.value("ganmao").toString();
    days[1].pm25=data.value("pm25").toInt();
    days[1].kqzl=data.value("quality").toString();
    days[1].city=obj.value("cityInfo").toObject().value("city").toString();
    QJsonArray forecast=data.value("forecast").toArray();
    for(int i=1;i<=4;i++)
    {
        QJsonObject day=forecast[i-1].toObject();
        Day *d=new Day();
        days[i].date=day.value("date").toString();
        days[i].type=day.value("type").toString();
        days[i].fx=day.value("fx").toString();
        days[i].fl=day.value("fl").toString();
        days[i].low=day.value("low").toString().split(" ")[1];
        days[i].high=day.value("high").toString().split(" ")[1];
        days[i].aqi=day.value("aqi").toInt();
        days[i].week=day.value("week").toString();
        days[i].notice=day.value("notice").toString();
        if(i==1)
        {
            ui->date->setText(day.value("ymd").toString()+" "+day.value("week").toString());
            d->ganmao=data.value("ganmao").toString();
            d->quality=data.value("quality").toString();
            d->shidu=data.value("shidu").toString();
            d->pm25=data.value("pm25").toInt();
        }
    }
    QJsonObject yst=data.value("yesterday").toObject();

    days[0].week=yst.value("week").toString();
    days[0].date=yst.value("date").toString();
    days[0].low=yst.value("low").toString().split(" ")[1];
    days[0].high=yst.value("high").toString().split(" ")[1];
    days[0].type=yst.value("type").toString();
    days[0].fx=yst.value("fx").toString();
    days[0].fl=yst.value("fl").toString();
    updateUI();
}

void Widget::updateUI()
{
    QString s=days[1].type;
    QString icon=weather_type[s];
    ui->city->setText(days[1].city);
    ui->notice->setText(days[1].notice);
    ui->ganmao->setText(days[1].ganmao);
    ui->temp->setText(days[1].wendu);
    ui->tdc->setPixmap(QPixmap(":/new/prefix1/res/type/"+icon+".png"));
    ui->fst->setText(days[1].fx+"\n"+days[1].fl);
    ui->sdt->setText("湿度\n"+days[1].shidu);
    ui->pm25->setText("PM2.5\n"+QString::number(days[1].pm25));
    ui->kqzl->setText("空气质量\n"+days[1].kqzl);
    ui->sdc->setPixmap(QPixmap(":/new/prefix1/res/humidity.png").scaled(50,50));
    ui->fsc->setPixmap(QPixmap(":/new/prefix1/res/wind.png").scaled(50,50));
    ui->pmc->setPixmap(QPixmap(":/new/prefix1/res/pm25.png").scaled(50,50));
    ui->kqc->setPixmap(QPixmap(":/new/prefix1/res/aqi.png").scaled(50,50));
    for(int i=0;i<=4;i++)
    {

        time[i]->setText(days[i].week+"\n"+days[i].date);
        ff[i]->setText(days[i].fx+"\n"+days[i].fl);
        typest[i]->setText(days[i].type);
        QString s=days[i].type;
        QString icon=weather_type[s];
        QPixmap pixmap(":/new/prefix1/res/type/"+icon+".png"); // 替换为你图片的路径
        // 将图片设置到QLabel中
        types[i]->setPixmap(pixmap);
        int aqi=days[i].aqi;
        if(aqi>=0&&aqi<=50)
        {
            quilaty[i]->setText("优");
            quilaty[i]->setStyleSheet("background-color:rgb(121,184,0);");
        }
        else if(aqi>50&&aqi<=100)
        {
            quilaty[i]->setText("良");
            quilaty[i]->setStyleSheet("background-color:rgb(255,187,23);");
        }
        else if(aqi>100&&aqi<=150)
        {
            quilaty[i]->setText("轻度");
            quilaty[i]->setStyleSheet("background-color:rgb(255,87,97);");
        }else if(aqi>150&&aqi<=200)
        {
            quilaty[i]->setText("中度");
            quilaty[i]->setStyleSheet("background-color:rgb(235,17,27);");
        }
        else if(aqi>200&&aqi<=250)
        {
            quilaty[i]->setText("重度");
            quilaty[i]->setStyleSheet("background-color:rgb(170,0,0);");
        }
        else{
            quilaty[i]->setText("严重");
            quilaty[i]->setStyleSheet("background-color:rgb(110,0,0);");
        }
    }
    ui->lblHighCurve->update();
    ui->lblLowCurve->update();
}

void Widget::on_pushButton_clicked()
{
    QString city_name=ui->cityname->text();
    if(!city.count(city_name))
    {
        QMessageBox::warning(NULL,"警告","请检查市名是否正确,如涟源则应补充完整为涟源市",QMessageBox::Button::Ok);
    }
    else{
         getWeatherInfo(city[city_name]);
    }
}

