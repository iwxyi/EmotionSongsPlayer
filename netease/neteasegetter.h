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

    void setUseFixed(bool enable);
    void setUseTypeFixed(bool enable);
    void addFixedSongList(QString id);
    void removeFixedSongList(QString id);
    void addTypeFixedSongList(QString type, QString id);
    void removeTypeFixedSongList(QString type, QString id);
    void addBlackList(QString b);
    void removeBlackList(QString b);

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

    bool use_fixed, use_type_fixed; // 开启则使用固定歌单、类型固定歌单
    QStringList fixed_songList; // 固定歌单列表（无视类别，只播放这些）
    QMap<QString, QStringList> type_fixed_songList_map; // 类型固定歌单
    QStringList black_list; // 黑名单
};

#endif // NETEASEGETTER_H
