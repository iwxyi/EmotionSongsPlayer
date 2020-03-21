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
        songList_list = decodeSongListList(result);
    });
}

void NeteaseGetter::getList(QString id)
{

}

void NeteaseGetter::getSong(QString id)
{

}

QList<SongList> NeteaseGetter::decodeSongListList(QString result)
{
    QList<SongList> songList_list;

    // 解析搜索结果（歌单里面随机取一个）
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(result.toUtf8(), &error);
    if (doc.isNull() || error.error != QJsonParseError::NoError)
    {
        qDebug() << "搜索结果解析错误" << error.error << error.errorString();
        return songList_list;
    }
    if (!doc.isObject())
    {
        qDebug() << "搜索结果不是对象";
        return songList_list;
    }
    QJsonObject object = doc.object();
    if (!object.contains("data"))
    {
        qDebug() << "搜索结果没有 data";
        return songList_list;
    }
    QJsonObject data = object.value("data").toObject();
    if (!data.contains("playlists") || data.value("playlists").type() != QJsonValue::Array)
    {
        qDebug() << "playlists 出错";
        return songList_list;
    }
    QJsonArray playLists = data.value("playlists").toArray();
    foreach (QJsonValue value, playLists)
    {
        if (value.type() != QJsonValue::Object)
        {
            qDebug() << "歌单不是 Object 类型" << value.toString();
            continue;
        }
        QJsonObject object = value.toObject();
        songList_list.append(decodeSongList(value.toObject()));
    }

    return songList_list;
}

SongList NeteaseGetter::decodeSongList(QJsonObject object)
{
    SongList songList;

    songList.id = QString::number(object.value("id").toInt());
    songList.name = object.value("name").toString();
    songList.description = object.value("description").toString();
    songList.creator_nickname = object.value("creator").toObject().value("nickname").toString();

    NETEASE_DEB songList.name << songList.id << songList.creator_nickname;

    return songList;
}

