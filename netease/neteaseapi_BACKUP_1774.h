#ifndef NETEASEAPI_H
#define NETEASEAPI_H

#include <QString>

#define NETEASE_DOMAIN "49.235.46.194:3000/netease/"

class NeteaseAPI
{
public:
    /**
     * 搜索所有歌单
     */
    static inline QString getSongListSearchUrl(QString type)
    {
//        return "https://v1.itooi.cn/netease/search?keyword="+type+"&type=songList";
        return NETEASE_DOMAIN + "search?type=1000&keywords=" + type;
    }

    /**
     * 获取歌单地址
     */
    static inline QString getSongListUrl(QString id)
    {
//        return "https://v1.itooi.cn/netease/songList?id=" + id;
        return NETEASE_DOMAIN + "playlist/detail?id=" + id;
    }

    /**
     * 获取歌曲详情地址
     * 可以获取多个，id用英文逗号隔开
     */
    static inline QString getSongUrl(QString id)
    {
//        return "https://v1.itooi.cn/netease/song?id="+id;
        return NETEASE_DOMAIN + "song/detail?ids=" + id;
    }

    /**
     * 获取歌曲下载地址
     * NodeJS版地址在 SongUrl 获取到的结果中有
     */
    static inline QString getSongDownloadUrl(QString id)
    {
<<<<<<< Updated upstream
//        return "https://v1.itooi.cn/netease/url?id="+id;
        return NETEASE_DOMAIN + "song/url?id=" + id;
=======
        return "https://v1.itooi.cn/netease/url?id="+id;
>>>>>>> Stashed changes
    }
};

#endif // NETEASEAPI_H
