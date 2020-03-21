#include "neteaseplayer.h"

NeteasePlayer::NeteasePlayer(QObject *parent) : QObject(parent), _flag_nexting(false), _flag_prepaired_next(false), _prepair_duration(0)
{
    musics = new NeteaseGetter(this);
    player = new QMediaPlayer(this);

    connect(musics, &NeteaseGetter::signalDownloadFinished, this, [=](QString id) {
        NETEASE_DEB "收到下载结束信号";
        if (player->state() != QMediaPlayer::PlayingState) // 没有正在播放
        {
            NETEASE_DEB "当前没有播放，直接开始播放";
            if (_flag_nexting)
                return ;
            QTimer::singleShot(1000, [=]{
                playerPlay(id);
                musics->isCurrentOrNext(id);
            });
        }
    });

    connect(player, &QMediaPlayer::durationChanged, this, [=](qint64 dur) {
        // 音乐时长（毫秒）
        // 由此提前一段时间下载下一首歌曲
        if (dur == 0) // 会遇见这种情况（可能是关闭音乐？）
        {
            _prepair_duration = 0;
            return ;
        }
        _prepair_duration = dur - PREPAIR_NEXT_SONG_AHEAD;
        NETEASE_DEB "durationChanged" << dur << _prepair_duration;
    });
    connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 pos) {
        if (player->duration() == 0) // 不知道是啥原因
            return ;
        if (pos >= _prepair_duration && !_flag_prepaired_next)
        {
            NETEASE_DEB "提前准备" << pos << _prepair_duration;
            musics->prepareNextSong();
            _flag_prepaired_next = true;
        }
        if (pos >= player->duration()) // 播放结束
        {
            NETEASE_DEB "播放结束" << pos << player->duration();
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
        _flag_prepaired_next = false;
        player->play();
    }
}
