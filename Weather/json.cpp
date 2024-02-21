#include "json.h"

JSON::JSON() {
    mnetworkaccess=new QNetworkAccessManager();
    connect(mnetworkaccess,&QNetworkAccessManager::finished,this,&JSON::onReplied);
}
