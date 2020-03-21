#ifndef NETEASEGETTER_H
#define NETEASEGETTER_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "neteaseapi.h"
#include "netutil.h"
#include "fileutil.h"

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
    friend class NeteasePlayer;
public:
    NeteaseGetter(QObject *parent = nullptr);

    void setDataDir(QString path);
    QString getType();

signals:
    void signalNoSongs(QString type);
    void signalDownloadFinished(QString id);

public slots:
    void searchNetListByType(QString type);
    void getRandomSongListInResult();
    void getNetList(QString id);
    void downloadNetSong(QString id);
    QString prepareNextSong();
    QString getNextSong();
    void isCurrentOrNext(QString id);

private:
    QList<SongList> decodeSongListList(QString result);
    SongList decodeSongList(QJsonObject object);
    SongList decodeSongList(QString result);
    Song decodeSong(QJsonObject object);

private:
    QString type;
    QList<SongList> songList_list; // 歌单列表
    SongList current_songList; // 歌单（歌曲列表）
    Song current_song, next_song;
    QString data_dir;
};

#endif // NETEASEGETTER_H
