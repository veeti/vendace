#ifndef UPLOADER_HPP
#define UPLOADER_HPP

#include <QtCore>
#include <QtNetwork>

class Uploader : public QObject {

    Q_OBJECT

    public:
        Uploader(QObject *parent = 0);
        QNetworkRequest makeRequest();
        QHttpMultiPart* makeMultiPart(QIODevice*);
        QString getUrlFromReply(QNetworkReply*);

};

#endif
