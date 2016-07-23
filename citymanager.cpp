#include "citymanager.h"

#include <QtNetwork>
#include <QTextCodec>
#include <QXmlStreamReader>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>

// 获取天气信息接口：http://www.webxml.com.cn/zh_cn/weather_icon.aspx
CityManager::CityManager(QObject *parent)
    : QObject(parent)
{
    net_ = new QNetworkAccessManager(this);
    QObject::connect(net_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

}

void CityManager::getSupportProvince(const OnMessage &msg)
{
    onMessage_ = msg;
    QUrl url("http://www.webxml.com.cn/WebServices/WeatherWebService.asmx/getSupportProvince?");
    QNetworkRequest request(url);
    net_->get(request);
}

void CityManager::getSupportCity(const QString &provinceName, const OnMessage &msg)
{
    onMessage_ = msg;
    QString str("http://www.webxml.com.cn/WebServices/WeatherWebService.asmx/getSupportCity?byProvinceName=");
    str.append(provinceName);
    QUrl url(str);
    QNetworkRequest request(url);
    net_->get(request);
}

void CityManager::getWeatherbyCityName(const QString &cityName, const OnMessage &msg)
{
    onMessage_ = msg;
    QString str("http://www.webxml.com.cn/WebServices/WeatherWebService.asmx/getWeatherbyCityName?theCityName=");
    str.append(cityName);
    QUrl url(str);
    QNetworkRequest request(url);
    net_->get(request);
}

void CityManager::replyFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString all = codec->toUnicode(reply->readAll());

    QStringList list;
    QXmlStreamReader reader(all);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            if (reader.name() == "string") {
                list.append(reader.readElementText());
            }
        }
    }

    if (onMessage_) {
        onMessage_(list);
    }

    reply->deleteLater();
}
