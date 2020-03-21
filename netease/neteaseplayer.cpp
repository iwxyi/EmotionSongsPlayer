#include "neteaseplayer.h"

NeteasePlayer::NeteasePlayer(QObject *parent) : QObject(parent), _flag_nexting(false)
{
    musics = new NeteaseGetter(this);
    player = new QMediaPlayer(this);

    connect(musics, &NeteaseGetter::signalDownloadFinished, this, [=](QString id) {
        qDebug() << "收到下载结束信号";
        if (player->state() != QMediaPlayer::PlayingState) // 没有正在播放
        {
            NETEASE_DEB "当前没有播放，直接开始播放";
            if (_flag_nexting)
                return ;
            QTimer::singleShot(1000, [=]{
                playerPlay(id);
            });
        }
    });
    connect(player, &QMediaPlayer::stateChanged, this, [=](QMediaPlayer::State state) {
        qDebug() << "----StateChanged" << state;
        if (state == QMediaPlayer::StoppedState) // 播放结束
        {
            NETEASE_DEB "播放结束，下一首";
            next();
        }
    });
}

void NeteasePlayer::setDataDir(QString path)
{
    this->data_dir = path;

}

void NeteasePlayer::randomPlay(QString type)
{
    musics->searchNetListByType(type);
}

/**
 * 开始播放/恢复暂停
 * 如果当前没有歌曲，那么调用下载
 * 通过信号 signalDownloadFinished 来开始播放
 */
void NeteasePlayer::play()
{
    if (!musics->current_song.id.isEmpty())
        NETEASE_DEB "开始播放：" << musics->current_song.name << musics->current_song.ar_name;
    if (player->state() == QMediaPlayer::PausedState) // 恢复暂停
        player->play();
    else if (musics->current_song.id.isEmpty()) // 当前没有音乐
        musics->prepareNextSong();
    else
        player->play();
}

void NeteasePlayer::stop()
{
    player->pause();
}

/**
 * 立即播放下一首歌
 * 如果下一首歌没有下载好，自动下载，
 * 之后通过信号 signalDownloadFinished 来开始播放
 */
void NeteasePlayer::next()
{
    QString id = musics->getNextSong();
    NETEASE_DEB "下一首，ID：" << id;
    if (id.isEmpty()) // 没有下载好，前往下载
        return ;

    _flag_nexting = true; // 避免切换media时，当做播放完毕再误触next
    // 已经下载好，直接播放
    playerPlay(id);
    _flag_nexting = false;
}

/**
 * 屏蔽当前一首歌
 */
void NeteasePlayer::block()
{

}

QString NeteasePlayer::information()
{
    return musics->current_song.name + "  " + musics->current_song.ar_name;
}

void NeteasePlayer::playerPlay(QString id)
{
    NETEASE_DEB "playerPlay" << id;
    QString path = data_dir + id + ".mp3";
    if (isFileExist(path))
    {
        player->setMedia(QUrl::fromLocalFile(path));
        player->play();
    }
}
