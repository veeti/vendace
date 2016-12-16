#include "Uploader.hpp"

Uploader::Uploader(QObject *parent) : QObject(parent) {
}

QNetworkRequest Uploader::makeRequest() {
    QNetworkRequest request(QUrl(mSettings.uploadUrl()));
    request.setRawHeader("Authorization", mSettings.uploadKey().toUtf8());
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

    QString url = doc.object()["data"].toObject()["link"].toString();
    if (mSettings.uploadHttps() == SETTING_UPLOAD_HTTPS) {
        url = url.replace("http:", "https:");
    }
    return url;
}
