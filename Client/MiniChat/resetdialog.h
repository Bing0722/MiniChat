#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();

    bool checkUserValid();
    bool checkPassValid();
    bool checkEmailValid();
    bool checkVerifyValid();
    bool checkConfirmValid();

    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);

    void ShowTip(QString str, bool ok_);

    void initHandlers();
private slots:
    void on_get_verify_btn_clicked();

    void on_sure_btn_clicked();
    void slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_cancle_btn_clicked();

signals:
    void sigSwitchLogin();

private:
    Ui::ResetDialog *ui;
    QMap<TipErr, QString> tip_errs_;
    QMap<ReqId, std::function<void(const QJsonObject &)>> handlers_;
};

#endif // RESETDIALOG_H
