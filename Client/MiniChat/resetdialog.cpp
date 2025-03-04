#include "resetdialog.h"
#include <QDebug>
#include <QRegularExpression>
#include "global.h"
#include "httpmgr.h"
#include "ui_resetdialog.h"

ResetDialog::ResetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResetDialog)
{
    ui->setupUi(this);

    ui->error_tip->setProperty("state", "normal");
    repolish(ui->error_tip);
    ui->error_tip->clear();

    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this]() { checkUserValid(); });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this]() { checkEmailValid(); });
    connect(ui->new_passwd_edit, &QLineEdit::editingFinished, this, [this]() { checkPassValid(); });
    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this]() { checkConfirmValid(); });
    connect(ui->verify_edit, &QLineEdit::editingFinished, this, [this]() { checkVerifyValid(); });

    // 连接reset相关信号和注册处理回调
    initHandlers();
    connect(HttpMgr::GetInstance().get(),
            &HttpMgr::sig_reset_mod_finish,
            this,
            &ResetDialog::slot_reset_mod_finish);
}

ResetDialog::~ResetDialog()
{
    qDebug() << "Destruct ResetDialog";
    delete ui;
}

bool ResetDialog::checkUserValid()
{
    if (ui->user_edit->text() == "") {
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool ResetDialog::checkPassValid()
{
    auto pass = ui->new_passwd_edit->text();
    if (pass.length() < 6 || pass.length() > 15) {
        // 提示长度不准确
        AddTipErr(TipErr::TIP_PASSWD_ERR, tr("密码长度应为6-15"));
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if (!match) {
        AddTipErr(TipErr::TIP_PASSWD_ERR, tr("不能包含非法字符"));
        return false;
    }

    DelTipErr(TipErr::TIP_PASSWD_ERR);
    return true;
}

bool ResetDialog::checkEmailValid()
{
    auto email = ui->email_edit->text();

    QRegularExpression regExp(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regExp.match(email).hasMatch();
    if (!match) {
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool ResetDialog::checkVerifyValid()
{
    auto pass = ui->verify_edit->text();
    if (pass.isEmpty()) {
        AddTipErr(TipErr::TIP_VERIFY_ERR, tr("验证码不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

bool ResetDialog::checkConfirmValid()
{
    auto confirm_pass = ui->confirm_edit->text();
    if (confirm_pass != ui->new_passwd_edit->text()) {
        AddTipErr(TipErr::TIP_CONFIRM_PASSWD_ERR, tr("两次密码不匹配"));
        return false;
    }

    DelTipErr(TipErr::TIP_CONFIRM_PASSWD_ERR);
    return true;
}

void ResetDialog::AddTipErr(TipErr te, QString tips)
{
    tip_errs_[te] = tips;
    ShowTip(tips, false);
}

void ResetDialog::DelTipErr(TipErr te)
{
    tip_errs_.remove(te);
    if (tip_errs_.empty()) {
        ui->error_tip->clear();
        return;
    }
    ShowTip(tip_errs_.first(), false);
}

void ResetDialog::ShowTip(QString str, bool ok_)
{
    if (ok_) {
        ui->error_tip->setProperty("state", "normal");
    } else {
        ui->error_tip->setProperty("state", "error");
    }

    ui->error_tip->setText(str);

    repolish(ui->error_tip);
}

void ResetDialog::initHandlers()
{
    // 获取验证码的逻辑
    handlers_.insert(ReqId::ID_GET_VERIFY_CODE, [this](QJsonObject json_obj) {
        int error = json_obj["email"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            ShowTip(tr("参数错误"), false);
            return;
        }
        auto email = json_obj["email"].toString();
        ShowTip(tr("验证码已发送到邮箱, 请注意查收"), true);
        qDebug() << "email is " << email;
    });

    // 重置密码的逻辑
    handlers_.insert(ReqId::ID_RESET_PASSWD, [this](QJsonObject json_obj) {
        int error = json_obj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            ShowTip(tr("参数错误"), false);
            return;
        }
        auto email = json_obj["email"].toString();
        ShowTip(tr("重置成功, 点击返回登录"), true);
        qDebug() << "email is " << email;
        qDebug() << "user uuid is " << json_obj["uuid"].toString();
    });
}

void ResetDialog::on_get_verify_btn_clicked()
{
    qDebug() << "receive verify btn clicked";
    auto email = ui->email_edit->text();
    auto bcheck = checkEmailValid();
    if (!bcheck) {
        return;
    }

    QJsonObject json_obj;
    json_obj["email"] = email;
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verify_code"),
                                        json_obj,
                                        ReqId::ID_GET_VERIFY_CODE,
                                        Modules::RESETMOD);
}

void ResetDialog::on_sure_btn_clicked()
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

    valid = checkVerifyValid();
    if (!valid) {
        return;
    }

    valid = checkConfirmValid();
    if (!valid) {
        return;
    }

    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = ui->new_passwd_edit->text();
    json_obj["cofirm"] = ui->confirm_edit->text();
    json_obj["verify_code"] = ui->verify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/reset_passwd"),
                                        json_obj,
                                        ReqId::ID_RESET_PASSWD,
                                        Modules::RESETMOD);
}

void ResetDialog::slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if (err != ErrorCodes::SUCCESS) {
        ShowTip(tr("网络请求错误"), false);
        return;
    }

    QJsonDocument json_doc = QJsonDocument::fromJson(res.toUtf8());
    if (json_doc.isNull()) {
        ShowTip(tr("json解析错误"), false);
        return;
    }

    if (!json_doc.isObject()) {
        ShowTip(tr("json解析错误"), false);
        return;
    }

    handlers_[id](json_doc.object());

    return;
}

void ResetDialog::on_cancle_btn_clicked()
{
    emit sigSwitchLogin();
}
