#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatdialog.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"

/************************************************************
 *
 * @file        mainwindow.h
 * @brief       主界面功能 Function
 *
 * @author      Bing
 * @date        2025/02/09
 * @history
 ************************************************************/
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotSwitchReg(); // 切换到注册界面 的槽函数
    void slotSwitchLogin();
    void slotSwitchLogin2();
    void slotSwitchReset(); // 切换到重置密码界面 的槽函数
    void slotSwitchChat();  // 切换到聊天界面 的槽函数

private:
    Ui::MainWindow *ui;
    LoginDialog *login_ui_;  // 登录ui
    RegisterDialog *reg_ui_; // 注册ui
    ResetDialog *reset_ui_;  // 重置密码ui
    ChatDialog *chat_ui_;    // 聊天ui
};
#endif // MAINWINDOW_H
