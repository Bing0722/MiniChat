#include "logindialog.h"
#include <QJsonObject>
#include "httpmgr.h"
#include "tcpmgr.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);

    // 设置忘记密码的状态
    ui->forget_label->SetState("normal", "hover", "", "selected", "selected_hover", "");
    ui->forget_label->setCursor(Qt::PointingHandCursor); // 设置鼠标显示 小手
    // 连接一个忘记密码的信号
    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);

    // 初始化http处理逻辑
    initHttpHandlers();

    // 连接一个 登录完成的信号
    connect(HttpMgr::GetInstance().get(),
            &HttpMgr::sig_login_mod_finish,
            this,
            &LoginDialog::slot_login_mod_finish);

    // 连接tcp连接请求的信号和槽函数
    connect(this,
            &LoginDialog::sig_connect_tcp,
            TcpMgr::GetInstance().get(),
            &TcpMgr::slot_tcp_connect); // 发送tcp连接的信号

    // 连接tcp管理者发出的连接成功信号
    connect(TcpMgr::GetInstance().get(),
            &TcpMgr::sig_con_success,
            this,
            &LoginDialog::slot_tcp_con_finish);

    // 连接tcp管理者发出的登录失败信号
    connect(TcpMgr::GetInstance().get(),
            &TcpMgr::sig_login_failed,
            this,
            &LoginDialog::slot_login_failed);
}

LoginDialog::~LoginDialog()
{
    qDebug() << "Destruct LoginDialog";
    delete ui;
}

bool LoginDialog::CheckUserValid()
{
    auto user = ui->user_edit->text();
    if (user.isEmpty()) {
        qDebug() << "User empty.";
        return false;
    }

    return true;
}

bool LoginDialog::CheckPasswdValid()
{
    auto pass = ui->pass_edit->text();
    if (pass.length() < 6 || pass.length() > 15) {
        qDebug() << "Passwd length invalid.";
        return false;
    }
    return true;
}

void LoginDialog::initHttpHandlers()
{
    // 注册一个 获取登录回包的逻辑
    handlers_.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject json_obj) {
        int error = json_obj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            // TODO 添加错误显示
            qDebug() << "参数错误";
            return;
        }
        auto user = json_obj["user"].toString();

        // 发送信号通知 TcpMgr 发送长连接
        ServerInfo serverinfo;
        serverinfo.Uid = json_obj["uid"].toInt();
        serverinfo.Host = json_obj["host"].toString();
        serverinfo.Port = json_obj["port"].toString();
        serverinfo.Token = json_obj["token"].toString();

        uid_ = serverinfo.Uid;
        token_ = serverinfo.Token;

        qDebug() << "User is " << user << " uid is " << serverinfo.Uid << "host is "
                 << serverinfo.Host << " port is " << serverinfo.Port << "token is "
                 << serverinfo.Token;

        emit sig_connect_tcp(serverinfo);
    });
}

// 忘记密码
void LoginDialog::slot_forget_pwd()
{
    qDebug() << "slot forget passwd";
    emit switchReset();
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if (err != ErrorCodes::SUCCESS) {
        // TODO
        return;
    }

    QJsonDocument json_doc = QJsonDocument::fromJson(res.toUtf8());
    if (json_doc.isNull()) {
        // ShowTip(tr("json解析错误"), false);
        return;
    }

    if (!json_doc.isObject()) {
        // ShowTip(tr("json解析错误"), false);
        return;
    }

    handlers_[id](json_doc.object());

    return;
}

// 收到连接结果的槽函数
void LoginDialog::slot_tcp_con_finish(bool success)
{
    if (success) {
        // TODO 处理登录tip ShowTip
        QJsonObject json_obj;
        json_obj["uid"] = uid_;
        json_obj["token"] = token_;

        QJsonDocument doc(json_obj);
        QString json_str = doc.toJson(QJsonDocument::Indented);

        // 发送 tcp请求给 chat server
        emit TcpMgr::GetInstance() -> sig_send_data(ReqId::ID_CHAT_LOGIN, json_str);
    } else {
        // TODO 错误处理
        // ShowTip("网络异常", false);
        qDebug() << "网络异常";
    }
}

void LoginDialog::slot_login_failed(int err)
{
    QString result = QString("失败登录, err is %1").arg(err);
    // TODO 完成错误显示 ShowTip
}

// 登录按钮
void LoginDialog::on_login_btn_clicked()
{
    qDebug() << "Login btn clicked";
    if (CheckUserValid() == false) {
        return;
    }
    if (CheckPasswdValid() == false) {
        return;
    }

    auto user = ui->user_edit->text();
    auto pass = ui->pass_edit->text();

    QJsonObject json_obj;
    json_obj["user"] = user;
    json_obj["pass"] = pass;
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_login"),
                                        json_obj,
                                        ReqId::ID_LOGIN_USER,
                                        Modules::LOGINMOD);
}
