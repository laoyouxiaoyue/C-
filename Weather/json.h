#ifndef JSON_H
#define JSON_H
#include <QNetWorkAccessManager>
#include <QNetworkReply>
#include <QWidget>
#include <QMouseEvent>
class JSON
{
public:
    JSON();
public:
    QNetworkAccessManager *mnetworkaccess;
private:
    void onReplied(QNetworkReply* reply);
};

#endif // JSON_H
