#include "neteasegetter.h"

NeteaseGetter::NeteaseGetter(QObject *parent) : QObject(parent)
{

}

void NeteaseGetter::searchNetListByType(QString type)
{
    NETEASE_DEB "开始搜索："+type;
    this->type = type;
    QString url = NeteaseAPI::getSongListSearchUrl(type);
    connect(new NetUtil(url), &NetUtil::finished, this, [=](QString result) {
        NETEASE_DEB "搜索结果" << QString::number(result.length());
        songList_list = decodeSongListList(result);

        if (songList_list.size() == 0)
        {
            qDebug() << "未搜索到分类";
            return ;
        }

        // 随机获取一个歌单（下一个准备播放的）
        int index = rand() % songList_list.size();
        current_songList = songList_list.at(index);
        NETEASE_DEB "下载随机歌单" << current_songList.name << current_songList.id;
        getNetList(current_songList.id);
    });
}

void NeteaseGetter::getNetList(QString id)
{
    NETEASE_DEB "获取歌单："+id;
    QString url = NeteaseAPI::getSongListUrl(id);
    connect(new NetUtil(url), &NetUtil::finished, this, [=](QString result) {
        NETEASE_DEB "搜索结果" << QString::number(result.length());
        current_songList = decodeSongList(result);
    });
}

void NeteaseGetter::downloadNetSong(QString id)
{
    QString url = NeteaseAPI::getSongDownloadUrl(id);
    NETEASE_DEB "开始下载歌曲："+id << url;

    // 判断文件是否已存在
//    if (isFileExist("music/" + id + ".mp3") || id.isEmpty())
//        return ;
    ensureDirExist("musics");

    NetUtil* net = new NetUtil;
    net->getRedirection(url);
    connect(net, &NetUtil::redirected, this, [=](QString url) {
        {
            NetUtil* net = new NetUtil;
            net->download(url, "musics/" + id + ".mp3");
            connect(net, &NetUtil::finished, this, [=](QString result) {
                qDebug() << "下载完毕" << result;
            });
        }
        net->deleteLater();
    });
}

QString NeteaseGetter::downloadNextRandom()
{
    if (current_songList.songs.size() == 0)
    {
        qDebug() << "没有歌单";
        return "";
    }

    int index = rand() % songList_list.size();
    next_song = current_songList.songs.at(index);
    NETEASE_DEB "下载随机歌曲" << next_song.name << next_song.id;
    downloadNetSong(next_song.id);
    return next_song.id;
}

QList<SongList> NeteaseGetter::decodeSongListList(QString result)
{
    QList<SongList> songList_list;

    // 解析搜索结果
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

SongList NeteaseGetter::decodeSongList(QString result)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(result.toUtf8(), &error);
    SongList songList;
    if (doc.isNull() || error.error != QJsonParseError::NoError)
    {
        qDebug() << "搜索结果解析错误" << error.error << error.errorString();
        return songList;
    }
    if (!doc.isObject())
    {
        qDebug() << "搜索结果不是对象";
        return songList;
    }
    QJsonObject object = doc.object();
    if (!object.contains("data"))
    {
        qDebug() << "搜索结果没有 data";
        return songList;
    }
    QJsonObject data = object.value("data").toObject();

    songList.id = QString::number(object.value("id").toInt());
    songList.name = data.value("name").toString();
    songList.description = data.value("description").toString();
    songList.creator_nickname = data.value("creator").toObject().value("nickname").toString();
    QJsonArray tracks = data.value("tracks").toArray();
    foreach (QJsonValue value, tracks)
    {
        songList.songs.append(decodeSong(value.toObject()));
    }
    return songList;
}

Song NeteaseGetter::decodeSong(QJsonObject object)
{
    Song song;
    song.id = QString::number(object.value("id").toInt());
    song.name = object.value("name").toString();
    song.ar_name = object.value("artists").toObject().value("name").toString();

    NETEASE_DEB song.name << song.id << song.ar_name;

    return song;
}
