#ifndef NETEASEGETTER_H
#define NETEASEGETTER_H

#include <QObject>
#include <QDebug>
#include "netutil.h"

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
    void getList(QString type);


private:
    QString type;
    QList<SongList> songList_list; // 歌单列表
    QList<Song> songList; // 歌单（歌曲列表）
};

#endif // NETEASEGETTER_H
