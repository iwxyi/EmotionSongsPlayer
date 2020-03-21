#ifndef NETEASEPLAYER_H
#define NETEASEPLAYER_H

#include <QObject>
#include "neteasegetter.h"

class NeteasePlayer : public QObject
{
    Q_OBJECT
public:
    explicit NeteasePlayer(QObject *parent = nullptr);

signals:

public slots:
    void randomPlay(QString type);
    void play();
    void stop();
    void next();
    void block();
    QString information();

private:
    NeteaseGetter* musics;

    QString current_type; // 当前类型，如果和 music 的 type 不一样，则重新获取（提前）
};

#endif // NETEASEPLAYER_H
