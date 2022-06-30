#ifndef CRAWLER_H
#define CRAWLER_H

#include <QUrl>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>
#include <QDebug>
#include <QString>
#include <QEventLoop>
#include <QRegularExpression>
#include <QFile>
#include <QVector>
struct imageMessage{
    QString imageNmae;
    QString imageSuffix;
    QByteArray image;
};
class crawler:public QObject
{
    Q_OBJECT
public:
    crawler(QNetworkAccessManager *pManager);
    void getReq();
    void setReq();
    void decode(QByteArray code);
    void downImg(imageMessage imageMes);
    void replace();
    void imgFinish(QNetworkReply *replyImg,imageMessage imageMes);
    QVector<imageMessage> *imageMess;
    QStringList imgList;
    QString imgName;
    QString imgType;
    QStringList imgNameList;
    QNetworkAccessManager *r_Manager;
public slots:
    void finished();

    void slotReadyRead();
    void downloadProgress(qint64 bytesSent, qint64 bytesTotal);

};

#endif // CRAWLER_H
