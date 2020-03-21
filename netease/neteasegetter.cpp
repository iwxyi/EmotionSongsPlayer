#include "neteasegetter.h"

NeteaseGetter::NeteaseGetter(QObject *parent) : QObject(parent)
{

}

void NeteaseGetter::searchListByType(QString type)
{
    NETEASE_DEB "开始搜索"+type;
    QString url = NeteaseAPI::getSongListSearchUrl(type);
    connect(new NetUtil(url), &NetUtil::finished, this, [=](QString result) {
        NETEASE_DEB "搜索结果" << QString::number(result.length());
        // 解析搜索结果（歌单里面随机取一个）
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(result.toLatin1(), &error);
        if (doc.isNull() || error.error == QJsonParseError::NoError)
        {
            qDebug() << "搜索结果解析错误";
            return ;
        }
        if (!doc.isObject())
        {
            qDebug() << "搜索结果不是对象";
            return ;
        }
        QJsonObject object = doc.object();
        QStringList list = object.keys();
        if (!list.contains("data"))
        {
            qDebug() << "未找到 data";
            return ;
        }
    });
}

void NeteaseGetter::getList(QString id)
{

}

void NeteaseGetter::getSong(QString id)
{

}

