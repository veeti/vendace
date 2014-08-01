#include "Uploader.hpp"

Uploader::Uploader(QObject *parent) : QObject(parent) {
}

QNetworkRequest Uploader::makeRequest() {
    QNetworkRequest request(QUrl("https://api.imgur.com/3/image"));
    request.setRawHeader("Authorization", "Client-ID ede801c7c97f184");
    return request;
}

QHttpMultiPart* Uploader::makeMultiPart(QIODevice *image) {
    QHttpMultiPart *mp = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);

    QHttpPart ip;
    ip.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    ip.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\""));
    ip.setBodyDevice(image);

    mp->append(ip);
    // Own the image IO device.
    image->setParent(mp);
    return mp;
}

QString Uploader::getUrlFromReply(QNetworkReply *reply) {
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if (doc.isNull()) {
        return "";
    }

    return doc.object()["data"].toObject()["link"].toString().replace("http:", "https:");
}
