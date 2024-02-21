#include "day.h"

Day::Day() {}

Day::Day(QString shidu, QString quality, QString wendu, QString ganmao, QString date, QString high, QString low, QString week, QString fx, QString fl, QString type, QString notice)
{
    this->shidu=shidu;
    this->quality=quality;
    this->wendu=wendu;
    this->ganmao=ganmao;
    this->date=date;
    this->high=high;
    this->low=low;
    this->week=week;
    this->fx=fx;
    this->fl=fl;
    this->type=type;
    this->notice=notice;
}

void Day::init()
{
    qDebug()<<date;
    //d->setText(date);
}
