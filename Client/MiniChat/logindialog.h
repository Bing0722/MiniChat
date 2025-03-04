#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    bool CheckUserValid();
    bool CheckPasswdValid();

    void initHttpHandlers();

private:
    Ui::LoginDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject &)>> handlers_;
    QString token_;
    int uid_;

public slots:
    void slot_forget_pwd();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);
    void slot_tcp_con_finish(bool success);

    void slot_login_failed(int);

signals:
    void switchRegister();
    void switchReset();
    void sig_connect_tcp(ServerInfo serverinfo);
private slots:
    void on_login_btn_clicked();
};

#endif // LOGINDIALOG_H
