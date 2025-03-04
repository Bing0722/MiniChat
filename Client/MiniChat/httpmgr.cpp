#include "httpmgr.h"


HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{
    // 连接http请求和完成的信号，信号槽机制保证队列消费
    connect(this, &HttpMgr::sig_http_finsh, this, &HttpMgr::slot_http_finish);
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    // 创建一个HTTP POST请求，并设置请求头和请求体
    QByteArray data = QJsonDocument(json).toJson();
    // 通过 url 构造请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    //
    auto self = shared_from_this();
    QNetworkReply *reply = manager_.post(request, data);
    // 设置信号和槽等待发送完成
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, req_id, mod](){
        // 处理错误的情况
        if(reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString();
            // 发送信号通知完成
            emit self->sig_http_finsh(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater();
            return;
        }

        // 无错误 则读回请求
        QString res = reply->readAll();

        // 发送信号通知完成
        emit self->sig_http_finsh(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
    });
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTERMOD) {
        // 发送信号通知指定模块http的响应结束了
        emit sig_reg_mod_finish(id, res, err);
    }

    if (mod == Modules::REGISTERMOD) {
        emit sig_reset_mod_finish(id, res, err);
    }

    if (mod == Modules::LOGINMOD) {
        emit sig_login_mod_finish(id, res, err);
    }
}
