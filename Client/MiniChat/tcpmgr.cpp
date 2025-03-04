#include "tcpmgr.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "usermgr.h"

TcpMgr::~TcpMgr() {}

// 构造函数
TcpMgr::TcpMgr()
    : host_("")
    , port_(0)
    , b_recv_pending_(false)
    , message_id_(0)
    , message_len_(0)
{
    // 连接一个 连接信号
    connect(&socket_, &QTcpSocket::connected, this, [&]() {
        qDebug() << "Connection to Server!";
        // 发送一个连接成功的信号
        emit sig_con_success(true);
    });

    // 连接一个 连接到达时的信号
    connect(&socket_, &QTcpSocket::readyRead, this, [&]() {
        // 当有数据可读时 读取所有数据并追加到缓冲区
        buffer_.append(socket_.readAll());

        QDataStream stream(&buffer_, QIODevice::ReadOnly);
        // 设置版本
        stream.setVersion(QDataStream::Qt_6_8);

        forever {
            // 解析头部
            if (!b_recv_pending_) {
                // 解析缓冲区的中接收的数据是否足够解析出一个消息头(消息ID + 消息长度)
                if (buffer_.size() < static_cast<int>(sizeof(quint16) * 2)) {
                    return; // 数据不够 等待更多数据
                }
                // 预读取消息id 和长度 但是不从缓冲区移除
                stream >> message_id_ >> message_len_;

                // 将buffer 中前四个1字节移出
                buffer_ = buffer_.mid(sizeof(quint16) * 2);

                // 输出读取的数据
                qDebug() << "Message ID: " << message_id_ << ", Length: " << message_len_;
            }

            // buffer剩余长度是否满足消息体长度 不满足则退出继续等待接收
            if (buffer_.size() < message_len_) {
                b_recv_pending_ = true;
                return;
            }

            b_recv_pending_ = false;
            // 读取消息体
            QByteArray messageBody = buffer_.mid(0, message_len_);
            qDebug() << "Recevice body message is " << messageBody;

            buffer_ = buffer_.mid(message_len_);
        }
    });

    // 处理错误
    connect(&socket_,
            QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this,
            [&](QAbstractSocket::SocketError socketError) {
                Q_UNUSED(socketError)
                qDebug() << "Error: " << socket_.errorString();
            });

    // 处理连接断开
    connect(&socket_, &QTcpSocket::disconnected, this, [&]() {
        qDebug() << "Disconnected from server.";
    });

    connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
}

void TcpMgr::initHandlers()
{
    handlers_.insert(ReqId::ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data) {
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }
        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err;
            // 触发登录失败信息
            emit sig_login_failed(err);
            return;
        }
        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Login Failed, err is " << err;
            emit sig_login_failed(err);
            return;
        }

        UserMgr::GetInstance()->SetUid(jsonObj["uid"].toInt());
        UserMgr::GetInstance()->SetName(jsonObj["name"].toString());
        UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());
        // 登录信号
        emit sig_switch_chat_ui();
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto find_iter = handlers_.find(id);
    if (find_iter == handlers_.end()) {
        qDebug() << "not found id [" << id << "] to handle";
        return;
    }
    find_iter.value()(id, len, data);
}

// 链接对端
void TcpMgr::slot_tcp_connect(ServerInfo serverinfo)
{
    qDebug() << "Receive tcp connect signal.";
    // 尝试连接到服务器
    qDebug() << "Connecting to server...";
    host_ = serverinfo.Host;
    port_ = static_cast<uint16_t>(serverinfo.Port.toUInt());
    socket_.connectToHost(host_, port_);
}

void TcpMgr::slot_send_data(ReqId id, QString data)
{
    uint16_t reqid = id;

    // 将字符串转换为 UTF-8 编码的字节数组
    QByteArray dataBytes = data.toUtf8();

    // 计算长度 (使用网络字节序转换)
    quint16 len = static_cast<quint16>(data.size());

    // 创建一个 QByteArray 用于存储要发送的所有数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    // 设置数据流使用网络字节序
    out.setByteOrder(QDataStream::BigEndian);

    // 写入 ID 和 长度
    out << reqid << len;

    // 添加字符串数据
    block.append(dataBytes);

    // 发送数据
    socket_.write(block);
}
