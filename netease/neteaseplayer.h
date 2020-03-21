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
    void randomPlay();
    void stop();
    void next();
    void block();
    QString information();

private:
    NeteaseGetter* musics;
};

#endif // NETEASEPLAYER_H
