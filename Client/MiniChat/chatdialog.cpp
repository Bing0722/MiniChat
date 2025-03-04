#include "chatdialog.h"
#include <QRandomGenerator>
#include "chatuserwid.h"
#include "loadingdialog.h"
#include "ui_chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog)
    , mode_(ChatUIMode::ChatMode)
    , state_(ChatUIMode::ChatMode)
    , b_loading_(false)
{
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "hover", "press");
    // 设置搜索框最大长度
    ui->search_edit->SetMaxLength(15);

    QAction *searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/res/search.png"));
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));

    // 创建一个清除动作并设置图标的行为
    QAction *clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/res/close_transparent.png"));
    // 初始化时不显示清除图标
    // 将清除动作添加到 LineEdit的末尾位置
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);

    // 当需要清除图标时, 更改为实际的清除图标
    connect(ui->search_edit, &QLineEdit::textChanged, this, [clearAction](const QString &text) {
        if (!text.isEmpty()) {
            clearAction->setIcon(QIcon(":/res/close_search.png"));
        } else {
            // 文本为空时 切换为透明状态
            clearAction->setIcon(QIcon(":/res/close_transparent.png"));
        }
    });

    connect(clearAction, &QAction::triggered, this, [this, clearAction]() {
        ui->search_edit->clear();
        // 清除文本后 切换回透明状态
        clearAction->setIcon(QIcon(":/res/close_transparent.png"));
        // 清除按钮被按下则不显示搜索框
        ShowSearch(false);
    });

    ShowSearch(false);

    // 连接加载信号槽
    connect(ui->chat_user_list,
            &ChatUserList::sig_loading_chat_user,
            this,
            &ChatDialog::slot_loading_chat_user);

    addChatUserList();
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

std::vector<QString> strs = {"hello world !",
                             "nice to meet u",
                             "New year, new life",
                             "You have to love yourself",
                             "My love is written in the wind ever since the whole world is you"};

std::vector<QString> heads = {
    ":/res/head_1.jpg",
    ":/res/head_2.jpg",
    ":/res/head_3.jpg",
    ":/res/head_4.jpg",
    ":/res/head_5.jpg",
};

std::vector<QString> names = {"Bing", "rust", "golang", "cpp", "lua", "javascript", "python", "java"};

void ChatDialog::addChatUserList()
{
    for (int i = 0; i < 13; i++) {
        // 生成从 0~99之间的随机整数
        int randomValue = QRandomGenerator::global()->bounded(100);
        int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();

        auto *chat_user_wid = new ChatUserWid();
        chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
        QListWidgetItem *item = new QListWidgetItem;
        qDebug() << "chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
    }
}

void ChatDialog::ShowSearch(bool bsearch)
{
    if (bsearch) {
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        mode_ = ChatUIMode::SearchMode;
    } else if (state_ == ChatUIMode::ChatMode) {
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        mode_ = ChatUIMode::ChatMode;
    } else if (state_ == ChatUIMode::ContactMode) {
        ui->chat_user_list->hide();
        ui->con_user_list->show();
        ui->search_list->hide();
        mode_ = ChatUIMode::ContactMode;
    }
}

void ChatDialog::slot_loading_chat_user()
{
    if (b_loading_) {
        return;
    }

    b_loading_ = true;
    LoadingDialog *loadingDialog = new LoadingDialog(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    qDebug() << "add new data to list...";
    addChatUserList();
    // 加载完毕后关闭对话框
    loadingDialog->deleteLater();

    b_loading_ = false;
}
