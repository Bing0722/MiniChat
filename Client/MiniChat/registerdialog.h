#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_get_code_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_confirm_btn_clicked();

    void on_return_btn_clicked();

    void on_cancel_btn_clicked();

signals:
    void sigSwitchLogin();

private:
    void initHttpHandlers();
    void ChangeTipPage();

    void showTip(QString str, bool b_ok);

    void addTipErr(TipErr tipErr, QString tips);
    void delTipErr(TipErr tipErr);

    bool checkUserValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkEmailValid();
    bool checkVerifyValid();

    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> handlers_;
    QMap<TipErr, QString> tip_errs_;

    QTimer *countdown_timer_;
    int countdown_;
};

#endif // REGISTERDIALOG_H
