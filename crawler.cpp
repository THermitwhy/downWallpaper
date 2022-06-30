#include "crawler.h"

crawler::crawler(QNetworkAccessManager *pManager):r_Manager(pManager)
{
    imageMess = new QVector<imageMessage>;
}

void crawler::getReq(){


    QUrl url;
    qDebug()<<"yes";
    url.setUrl("https://wallhaven.cc/search?q="+imgType);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.5005.124 Safari/537.36 Edg/102.0.1245.44");
    QNetworkReply *reply ;


    reply = r_Manager->get(request);

    connect(reply,SIGNAL(finished()),this,SLOT(finished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),
                this,SLOT(downloadProgress(qint64,qint64)));


    //0reply->readAll();


}

void crawler::downloadProgress(qint64 bytesSent, qint64 bytesTotal){
    qDebug()<< "\ndownloadProgress done:\n";
    qDebug() << "bytesSent: " << bytesSent
         << "  " << "bytesTocal: " << bytesTotal;
}

void crawler::finished(){
    QNetworkReply* reply = (QNetworkReply*)sender();
    QByteArray data=reply->readAll();
    if(!data.isNull()){
        decode(data);
    }
    //downImg();
    //replace();
    for(int i=0;i<imageMess->size();i++){
        //qDebug()<<imgList[i];
        //qDebug()<<imgNameList[i];
        downImg(imageMess->at(i));
    }

    //qDebug()<<imgList[0];
    //qDebug()<<imgNameList[0];

}

void crawler::slotReadyRead(){
    qDebug()<<"has";
}

void crawler::decode(QByteArray code){//将网页中的图片网址拆分并存储到imgList中
    QString pattern("<ul>(.*)</ul>");
    QRegularExpression rx(pattern);
    QRegularExpressionMatch match = rx.match(code);
    if(match.hasMatch()){
        QString ul=match.captured(1);
        QRegularExpression rxLi("<li>(.*)</li>");
        rxLi.setPatternOptions(QRegularExpression::InvertedGreedinessOption);
        QRegularExpressionMatchIterator matchLis = rxLi.globalMatch(ul);
        QRegularExpression rxSuffix("PNG");
        QRegularExpression rxImgurl("data-src=\"https://th.wallhaven.cc/small/[0-9A-Za-z]{2}/([0-9A-Za-z]{2,9}).jpg");
        while(matchLis.hasNext()){
            QRegularExpressionMatch matchLi = matchLis.next();
            QString imgMes = matchLi.captured(1);
            QRegularExpressionMatch matchImgurl = rxImgurl.match(imgMes);
            QRegularExpressionMatch matchSuffix = rxSuffix.match(imgMes);
            imageMessage image;
            image.imageNmae = matchImgurl.captured(1);
            if(matchSuffix.hasMatch()){
                image.imageSuffix = "png";
            }
            else{
                image.imageSuffix = "jpg";
            }
            imageMess->append(image);
        }
        /*QString ul=match.captured(1);
        QString patImg("data-src=\"(https://th.wallhaven.cc/small/[0-9A-Za-z]{2}/[0-9A-Za-z]{2,9}.jpg)");//https://th.wallhaven.cc/small/83/83gpqy.jpg

        QRegularExpression rxImg(patImg);
        QRegularExpressionMatchIterator matches = rxImg.globalMatch(ul);
        //QRegularExpressionMatch matchImg = rxImg.match(ul);
        imgList.clear();
        while(matches.hasNext()){
            QRegularExpressionMatch matchImg = matches.next();
            imgList<<matchImg.captured(1);
        }*/
    }
}

void crawler::downImg(imageMessage imageMes){
    QUrl urlImg;
    QString imgUrl = "https://w.wallhaven.cc/full/" + imageMes.imageNmae.left(2)+"/"+"wallhaven-"+imageMes.imageNmae+"."+imageMes.imageSuffix;
    //qDebug()<<"yes";
    urlImg.setUrl(imgUrl);
    //imgName = imgNames;
    QNetworkRequest requestImg(urlImg);
    requestImg.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.5005.124 Safari/537.36 Edg/102.0.1245.44");
    QNetworkReply *replyImg ;


    replyImg = r_Manager->get(requestImg);

    QEventLoop eventLoop;
    connect(replyImg, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    imgFinish(replyImg,imageMes);
    //connect(replyImg,SIGNAL(finished()),this,SLOT(imgFinish()));

    //connect(replyImg, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));



    //0reply->readAll();
}

void crawler::imgFinish(QNetworkReply *replyImg,imageMessage imageMes){
    //QNetworkReply* replyImg = (QNetworkReply*)sender();
    QByteArray data=replyImg->readAll();
    QFile file;

    file.setFileName("D:/wallpaper/"+imageMes.imageNmae+"."+imageMes.imageSuffix);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(data);
        file.close();
    }
    qDebug()<<imageMes.imageNmae;
}

void crawler::replace(){//将缩略图的url替换为原图url,并存储名字
    QString rep("https://th.wallhaven.cc/small/([0-9A-Za-z]{2})/([0-9A-Za-z]{2,9}).jpg");
    QString repName("https://w.wallhaven.cc/full/([0-9A-Za-z]{2})/wallhaven-([0-9A-Za-z]{2,9}).jpg");
    QRegularExpression rxRep(rep);
    QRegularExpression rxRepName(repName);
    for(int i=0;i<imgList.count();i++){
        imgList[i].replace(rxRep,"https://w.wallhaven.cc/full/\\1/wallhaven-\\2.jpg");

    }
    imgNameList = imgList;
    for(int i=0;i<imgNameList.count();i++){
        imgNameList[i].replace(rxRepName,"_\\2.jpg");
        imgNameList[i] = imgType + imgNameList[i];
    }
    //imgList[0].replace(rxRep,"https://w.wallhaven.cc/full/\\1/wallhaven-\\2.jpg");

}
