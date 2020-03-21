#include "neteaseplayer.h"

NeteasePlayer::NeteasePlayer(QObject *parent) : QObject(parent)
{
    musics = new NeteaseGetter(this);
}

void NeteasePlayer::randomPlay(QString type)
{
    musics->searchNetListByType(type);
}

void NeteasePlayer::play()
{
    musics->downloadNextRandom();
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
    return musics->current_song.name + "  " + musics->current_song.ar_name;
}
