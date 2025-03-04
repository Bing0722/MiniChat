#ifndef TCPMGR_H
#define TCPMGR_H

#include <QTcpSocket>
#include "global.h"
#include "singleton.h"

class TcpMgr : public QObject, public Singleton<TcpMgr>, std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
    ~TcpMgr();

private:
    friend class Singleton<TcpMgr>;
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);

    QTcpSocket socket_;
    QString host_;
    uint16_t port_;
    QByteArray buffer_;
    bool b_recv_pending_;
    quint16 message_id_;
    quint16 message_len_;
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> handlers_;
public slots:
    void slot_tcp_connect(ServerInfo serverinfo);
    void slot_send_data(ReqId id, QString data);
signals:
    void sig_con_success(bool success);
    void sig_send_data(ReqId id, QString data);
    void sig_login_failed(int);
    void sig_switch_chat_ui(); // 切换chat ui信号
};

#endif // TCPMGR_H
