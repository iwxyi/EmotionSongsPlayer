#ifndef NETEASEGETTER_H
#define NETEASEGETTER_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "netutil.h"
#include "neteaseapi.h"

#define NETEASE_DEB if (1) qDebug()<<

/**
 * 歌曲信息
 */
struct Song {
    QString id;
    QString name; // 歌曲名称
    QString ar_name; // 歌手名字
};

/**
 * 歌单信息
 */
struct SongList {
    QString id;
    QString name;
    QString description;
    QList<Song> songs;
    QString creator_nickname;
};

class NeteaseGetter : public QObject
{
    Q_OBJECT
public:
    NeteaseGetter(QObject *parent = nullptr);

signals:
    void signalNoSongs(QString type);

public slots:
    void searchListByType(QString type);
    void getList(QString id);
    void getSong(QString id);

private:
    QList<SongList> decodeSongListList(QString result);
    SongList decodeSongList(QJsonObject object);

private:
    QString type;
    QList<SongList> songList_list; // 歌单列表
    QList<Song> songList; // 歌单（歌曲列表）
    Song song;
};

#endif // NETEASEGETTER_H
