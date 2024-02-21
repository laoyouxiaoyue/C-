#ifndef DAY_H
#define DAY_H
#include <QString>
#include <QLabel>
class Day
{
public:
    Day();
    Day(QString shidu,QString quality, QString wendu, QString ganmao,QString date, QString high,QString low,QString week,   QString fx, QString fl,   QString type,   QString notice);
public:
    void init();
    QString shidu;
    QString quality;
    QString wendu;
    QString ganmao;
    QString date;
    QString high;
    QString low;
    QString week;
    QString fx;
    QString fl;
    QString type;
    QString kqzl;
    int aqi;
    QString notice;
    int pm25;
    QLabel *d;
    QLabel *dc;
    QLabel *dp;
    QLabel *df;
    QString city;
};

#endif // DAY_H
