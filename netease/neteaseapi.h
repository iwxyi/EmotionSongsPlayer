#ifndef NETEASEAPI_H
#define NETEASEAPI_H

#include <QString>

class NeteaseAPI
{
public:
    /**
     * 搜索所有歌单
     */
    static inline QString getSongListSearchUrl(QString type)
    {
        return "https://v1.itooi.cn/netease/search?keyword="+type+"&type=songList";
    }

    /**
     * 获取歌单地址
     */
    static inline QString getSongListUrl(QString id)
    {
        return "https://v1.itooi.cn/netease/songList?id=" + id;
    }

    /**
     * 获取歌曲详情地址
     * 可以获取多个，id用英文逗号隔开
     */
    static inline QString getSongUrl(QString id)
    {
        return "https://v1.itooi.cn/netease/song?id="+id;
    }

    /**
     * 获取歌曲下载地址
     */
    static inline QString getSongDownloadUrl(QString id)
    {
        return "https://v1.itooi.cn/netease/url?id="+id+"&quality=flac";
    }
};

#endif // NETEASEAPI_H
