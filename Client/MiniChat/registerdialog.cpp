#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
    , countdown_(5)
{
    ui->setupUi(this);
    // 设置输入模式 密码模式
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    ui->error_tip->setProperty("state", "normal");
    repolish(ui->error_tip);

    // 将http的注册信号与注册完成的槽函数连接
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    initHttpHandlers();

    ui->error_tip->clear();

    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this](){ checkUserValid(); });

    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){ checkEmailValid(); });

    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this](){ checkPassValid(); });

    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this](){ checkConfirmValid(); });

    connect(ui->verify_edit, &QLineEdit::editingFinished, this, [this](){ checkVerifyValid(); });

    // 设置鼠标小手
    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    ui->pass_visible->SetState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
    ui->confirm_visible->SetState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");

    // 连接点击事件
    connect(ui->pass_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->pass_visible->GetCursorState();
        if (state == ClickedLabelState::NORMAL) {
            ui->pass_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Pass Label was clicked!";
    });

    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->confirm_visible->GetCursorState();
        if (state == ClickedLabelState::NORMAL) {
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Confirm Label was clicked!";
    });

    // 创建定时器
    countdown_timer_ = new QTimer(this);

    connect(countdown_timer_, &QTimer::timeout, [this]() {
        if (countdown_ == 0) {
            countdown_timer_->stop();
            emit sigSwitchLogin();
            return;
        }
        countdown_--;
        auto str = QString("注册成功, %1 s后返回登录").arg(countdown_);
        ui->tip1_lb->setText(str);
    });
}

RegisterDialog::~RegisterDialog()
{
    qDebug() << "Destruct RegisterDialog";
    delete ui;
}

void RegisterDialog::on_get_code_clicked()
{
    // bing123@gmail.com
    auto email = ui->email_edit->text();
    // 匹配正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match) {
        // 发送http验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verify_code"),
                                            json_obj, ReqId::ID_GET_VERIFY_CODE, Modules::REGISTERMOD);
    }else {
        showTip(tr("邮箱地址不正确"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if (err != ErrorCodes::SUCCESS) {
        showTip(tr("网络请求错误"), false);
        return;
    }

    // 解析 Json 字符串, res 转化为 QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()) {
        showTip(tr("json解析失败"), false);
        return;
    }
    // json 解析失败
    if(!jsonDoc.isObject()) {
        showTip(tr("json解析失败"), false);
        return;
    }

    // 调用对应的逻辑
    handlers_[id](jsonDoc.object());
    return;
}

// 初始化
void RegisterDialog::initHttpHandlers()
{
    // 注册 获取验证码回包的逻辑
    handlers_.insert(ReqId::ID_GET_VERIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，请注意查收"), true);
        qDebug() << "email is " << email;
    });

    // 注册 注册用户的回报逻辑
    handlers_.insert(ReqId::ID_REG_USER, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug() << "email is " << email;
        qDebug() << "user uuid is " << jsonObj["uuid"].toString();
        ChangeTipPage();
    });
}

void RegisterDialog::ChangeTipPage()
{
    countdown_timer_->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    // 启动定时器， 设置时间间隔为1000ms
    countdown_timer_->start(1000);
}

// 显示提示
void RegisterDialog::showTip(QString str, bool b_ok)
{
    if (b_ok){
        ui->error_tip->setProperty("state", "normal");
    } else {
        ui->error_tip->setProperty("state", "error");
    }
    ui->error_tip->setText(str);

    repolish(ui->error_tip);
}

void RegisterDialog::addTipErr(TipErr tipErr, QString tips)
{
    tip_errs_[tipErr] = tips;
    showTip(tips, false);
}

void RegisterDialog::delTipErr(TipErr tipErr)
{
    tip_errs_.remove(tipErr);
    if (tip_errs_.empty()){
        ui->error_tip->clear();
        return;
    }

    showTip(tip_errs_.first(), false);
}

bool RegisterDialog::checkUserValid()
{
    if (ui->user_edit->text() == "") {
        addTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }
    delTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkPassValid()
{
    auto pass = ui->pass_edit->text();
    if (pass.length() < 6 || pass.length() > 15) {
        addTipErr(TipErr::TIP_PASSWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if (!match){
        addTipErr(TipErr::TIP_PASSWD_ERR, tr("不能包含非法字符"));
        return false;
    }

    delTipErr(TipErr::TIP_PASSWD_ERR);
    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    auto confirm_pass = ui->confirm_edit->text();
    if (confirm_pass != ui->pass_edit->text()){
        addTipErr(TipErr::TIP_CONFIRM_PASSWD_ERR, tr("两次输入的密码不匹配"));
        return false;
    }

    delTipErr(TipErr::TIP_CONFIRM_PASSWD_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regExp(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regExp.match(email).hasMatch();
    if (!match){
        addTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }

    delTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkVerifyValid()
{
    auto verify_code = ui->verify_edit->text();
    if (verify_code.isEmpty()){
        addTipErr(TipErr::TIP_VERIFY_ERR, tr("验证码不能为空"));
        return false;
    }

    delTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}


// 注册的确认按钮
void RegisterDialog::on_confirm_btn_clicked()
{
    bool valid = checkUserValid();
    if (!valid) {
        return;
    }

    valid = checkEmailValid();
    if (!valid) {
        return;
    }

    valid = checkPassValid();
    if (!valid) {
        return;
    }

    valid = checkConfirmValid();
    if (!valid) {
        return;
    }

    valid = checkVerifyValid();
    if (!valid) {
        return;
    }

    // 发送http请求
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["pass"] = ui->pass_edit->text();
    json_obj["confirm"] = ui->confirm_edit->text();
    json_obj["verify_code"] = ui->verify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);
}

void RegisterDialog::on_return_btn_clicked()
{
    countdown_timer_->stop();
    emit sigSwitchLogin();
}

void RegisterDialog::on_cancel_btn_clicked()
{
    countdown_timer_->stop();
    emit sigSwitchLogin();
}
