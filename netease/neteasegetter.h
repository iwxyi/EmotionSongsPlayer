#ifndef NETEASEGETTER_H
#define NETEASEGETTER_H

#include <QObject>
#include <QDebug>

/**
 * 歌单信息
 */
struct SongList {

};

/**
 * 歌曲信息
 */
struct Song {

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
};

#endif // NETEASEGETTER_H
