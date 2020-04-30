#ifndef NETUTIL_H
#define NETUTIL_H

#include <QObject>
#include <QThread>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QUrl>
#include <QTextCodec>
#include <QFile>
#include <QFileInfo>
#include <initializer_list>

/**
 * 网络操作做工具类，读取网络源码
 */
class NetUtil : public QObject
{
    Q_OBJECT
public:
    static QString getWebData(QString uri)
    {
        QUrl url(uri);
        QNetworkAccessManager manager;
        QEventLoop loop;
        QNetworkReply *reply;

        reply = manager.get(QNetworkRequest(url));
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit())); //请求结束并下载完成后，退出子事件循环
        loop.exec(); //开启子事件循环

        QString code_content(reply->readAll().data());

        reply->deleteLater();
        return code_content;
    }

    static QString postWebData(QString uri, QString data)
    {
        QUrl url(uri);
        QNetworkAccessManager manager;
        QEventLoop loop;
        QNetworkReply *reply;

        reply = manager.post(QNetworkRequest(url), data.toLatin1());
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit())); //请求结束并下载完成后，退出子事件循环
        loop.exec(); //开启子事件循环

        QString code_content(reply->readAll().data());
        reply->deleteLater();
        return code_content;
    }

    static QString downloadWebFile(QString uri, QString path)
    {
        QNetworkAccessManager manager;
        QEventLoop loop;
        QNetworkReply *reply;

        reply = manager.get(QNetworkRequest(QUrl(uri)));
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit())); //请求结束并下载完成后，退出子事件循环
        loop.exec(); //开启子事件循环

        QFile file(path);
        if (!file.open(QFile::WriteOnly))
        {
            qDebug() << "写入文件失败" << path;
            reply->deleteLater();
            return "";
        }
        QByteArray data = reply->readAll();
        if (!data.isEmpty())
        {
            qint64 write_bytes = file.write(data);
            file.flush();
            if (write_bytes != data.size())
                qDebug() << "写入文件大小错误" << write_bytes << "/" << data.size();
        }

        reply->deleteLater();
        return path;
    }

    static NetUtil* downloadOnce(QString uri, QString path)
    {
        NetUtil* net = new NetUtil;
        net->download(uri, path);
        return net;
    }

    static QStringList extractList(QString text, QString pat)
    {
        QRegularExpression rx(pat);
        QRegularExpressionMatch match;
        if (text.indexOf(rx, 0, &match))
            return match.capturedTexts();
        else
            return QStringList();
    }

    static QString extractStr(QString text, QString pat)
    {
        QStringList list = extractList(text, pat);
        if (list.size())
            return list.first();
        else
            return QString();
    }

    /**
     * 用来提取括号第一个
     */
    static QString extractOne(QString text, QString pat)
    {
        QStringList list = extractList(text, pat);
        if (list.size() >= 2)
            return list.at(1);
        else if (list.size())
            return list.first();
        else
            return QString();
    }

public:
    NetUtil(QObject* parent = nullptr) : QObject(parent) {}

    NetUtil(QString uri)
    {
        get(uri);
        connect(this, &NetUtil::finished, [=]{
            this->deleteLater();
        });
    }

    NetUtil(QString uri, QString param)
    {
        post(uri, param);
        connect(this, &NetUtil::finished, [=]{
            this->deleteLater();
        });
    }

    NetUtil(QString uri, QStringList params)
    {
        QString data;
        for (int i = 0; i < params.size(); i++)
        {
            if (i & 1) // 用户数据
                data += QUrl::toPercentEncoding(params.at(i));
            else // 固定变量
                data += (i==0?"":"&") + params.at(i) + "=";
        }
        post(uri, data);
        connect(this, &NetUtil::finished, [=]{
            this->deleteLater();
        });
    }

    /*NetUtil(QString uri, QString param, Args...args)
    {

    }*/

public:

    void get(QString uri)
    {
        QNetworkRequest request;
        request.setUrl(QUrl(uri));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QNetworkReply *reply = manager->get(request);
        QObject::connect(reply, &QNetworkReply::finished, [=]{
            QString str = QString(reply->readAll().data());
            emit finished(str);
            reply->deleteLater();
        });
    }

    void post(QString uri, QString data)
    {
        QNetworkRequest request;
        request.setUrl(QUrl(uri));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
#ifdef NET_DEBUG
qDebug() << "网址 post ：" << uri << data;
#endif
        auto body = data.toLatin1();
        QNetworkReply *reply = manager->post(request, body);

        QObject::connect(reply, &QNetworkReply::finished, [=]{
            QString str = QString(reply->readAll().data());
#ifdef NET_DEBUG
            qDebug() << "返回结果：" << str; // 注意：要是传回来的内容太长（超过3万6左右），qDebug不会输出。这是可以输出size()来查看
#endif
            emit finished(str);
            reply->deleteLater();
        });
    }

    /**
     * 获取http头中的重定向
     * 用于解决文件下载地址重定向的问题
     */
    void getRedirection(QString uri)
    {
        QNetworkRequest request;
        request.setUrl(QUrl(uri));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QNetworkReply *reply = manager->get(request);
        QObject::connect(reply, &QNetworkReply::finished, [=]{
            qDebug() << reply->header(QNetworkRequest::LocationHeader).toString();
            emit redirected(reply->header(QNetworkRequest::LocationHeader).toString());
            reply->deleteLater();
            manager->deleteLater();
        });
    }

    /**
     * 注意：这个函数，不是一次性的，需要手动释放
     */
    void download(QString uri, QString path)
    {
        QNetworkAccessManager* manager = new QNetworkAccessManager;
        QEventLoop loop;
        QNetworkReply *reply;

        reply = manager->get(QNetworkRequest(QUrl(uri)));

        connect(reply, &QNetworkReply::downloadProgress, [=](qint64 recv, qint64 total){
            emit progress(recv, total);
        });

        QObject::connect(reply, &QNetworkReply::finished, [=]{
            QFile file(path);
            if (!file.open(QFile::WriteOnly))
            {
                qDebug() << "文件打开失败" << path;
                reply->deleteLater();
                emit finished("");
                return ;
            }

            QByteArray data = reply->readAll();
            if (!data.isEmpty())
            {
                qint64 write_bytes = file.write(data);
                file.flush();
                if (write_bytes != data.size())
                    qDebug() << "写入文件大小错误" << write_bytes << "/" << data.size();
            }
qDebug() << "文件下载完成";
            emit finished(path);
            reply->deleteLater();
            manager->deleteLater();
        });
    }

signals:
    void progress(qint64, qint64);
    void finished(QString);
    void redirected(QString url);
};

#endif // NETUTIL_H
