#ifndef UPLOADER_HPP
#define UPLOADER_HPP

#include <QtCore>
#include <QtNetwork>
#include "Settings.hpp"

class Uploader : public QObject {

    Q_OBJECT

    public:
        Uploader(QObject *parent = 0);
        QNetworkRequest makeRequest();
        QHttpMultiPart* makeMultiPart(QIODevice*);
        QString getUrlFromReply(QNetworkReply*);

    protected:
        Settings mSettings;

};

#endif
