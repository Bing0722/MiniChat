#include "mainwindow.h"
#include "tcpmgr.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 继承自this 由父亲管理其生命周期
    login_ui_ = new LoginDialog(this);
    // 设置窗口标志, 自定义窗口行为和外观
    login_ui_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    // 设置主窗口的中央部件
    setCentralWidget(login_ui_);

    // 创建和注册消息连接
    connect(login_ui_, &LoginDialog::switchRegister, this, &MainWindow::slotSwitchReg);
    // 连接登录界面的忘记密码信号
    connect(login_ui_, &LoginDialog::switchReset, this, &MainWindow::slotSwitchReset);
    // 连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),
            &TcpMgr::sig_switch_chat_ui,
            this,
            &MainWindow::slotSwitchChat);

    emit TcpMgr::GetInstance() -> sig_switch_chat_ui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSwitchReg()
{
    reg_ui_ = new RegisterDialog(this);
    // 设置窗口标志, 自定义窗口行为和外观
    reg_ui_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    reg_ui_->hide();

    connect(reg_ui_, &RegisterDialog::sigSwitchLogin, this, &MainWindow::slotSwitchLogin);
    setCentralWidget(reg_ui_);
    login_ui_->hide();
    reg_ui_->show();
}

void MainWindow::slotSwitchLogin()
{
    login_ui_ = new LoginDialog(this);
    login_ui_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(login_ui_);

    reg_ui_->hide();
    login_ui_->show();

    // 创建和注册消息连接
    connect(login_ui_, &LoginDialog::switchRegister, this, &MainWindow::slotSwitchReg);
    // 连接登录界面的忘记密码信号
    connect(login_ui_, &LoginDialog::switchReset, this, &MainWindow::slotSwitchReset);
    // 连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),
            &TcpMgr::sig_switch_chat_ui,
            this,
            &MainWindow::slotSwitchChat);
}

void MainWindow::slotSwitchLogin2()
{
    login_ui_ = new LoginDialog(this);
    login_ui_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(login_ui_);

    reset_ui_->hide();
    login_ui_->show();

    // 创建和注册消息连接
    connect(login_ui_, &LoginDialog::switchRegister, this, &MainWindow::slotSwitchReg);
    // 连接登录界面的忘记密码信号
    connect(login_ui_, &LoginDialog::switchReset, this, &MainWindow::slotSwitchReset);
}

void MainWindow::slotSwitchReset()
{
    reset_ui_ = new ResetDialog(this);
    reset_ui_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    connect(reset_ui_, &ResetDialog::sigSwitchLogin, this, &MainWindow::slotSwitchLogin2);
    setCentralWidget(reset_ui_);

    login_ui_->hide();
    reset_ui_->show();
}

void MainWindow::slotSwitchChat()
{
    chat_ui_ = new ChatDialog();
    chat_ui_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(chat_ui_);
    chat_ui_->show();
    login_ui_->hide();
    this->setMinimumSize(QSize(1050, 900));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}
