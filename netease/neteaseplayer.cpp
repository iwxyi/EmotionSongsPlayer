#include "neteaseplayer.h"

NeteasePlayer::NeteasePlayer(QObject *parent) : QObject(parent)
{
    musics = new NeteaseGetter(this);
}

void NeteasePlayer::randomPlay()
{

}

void NeteasePlayer::stop()
{

}

void NeteasePlayer::next()
{

}

void NeteasePlayer::block()
{

}

QString NeteasePlayer::information()
{

}
