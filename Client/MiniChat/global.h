#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include <QStyle>
#include <QRegularExpression>
#include <memory.h>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QNetworkReply>
#include <QDir>
#include <QSettings>

/**
 * @brief repolish 用来刷新qss
 */
extern std::function<void(QWidget*)> repolish;

extern QString gate_url_prefix;

/**
 * @brief The ReqId enum 请求ID
 */
enum ReqId {
    ID_GET_VERIFY_CODE = 1001, // 获取验证码
    ID_REG_USER = 1002,        // 注册用户
    ID_RESET_PASSWD = 1003,    // 重置密码
    ID_LOGIN_USER = 1004,      // 登录用户
    ID_CHAT_LOGIN = 1005,      // Chat 登录
    ID_CHAT_LOGIN_RSP = 1006,  // 登录聊天服务回包
};

/**
 * @brief The Modules enum 组件模块
 */
enum Modules {
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
};

/**
 * @brief The ErrorCodes enum 错误类型
 */
enum ErrorCodes{
    SUCCESS = 0,        // 成功
    ERR_JSON = 1,       // json 解析失败
    ERR_NETWORK = 2,    // 网络错误
};

/**
 * @brief The TipError enum 错误提示类型
 */
enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PASSWD_ERR = 2,
    TIP_CONFIRM_PASSWD_ERR = 3,
    TIP_PASSWD_CONFIRM = 4,
    TIP_VERIFY_ERR = 5,
    TIP_USER_ERR = 6,
};

/**
 * @brief The ClickedLabelState enum 点击标签的状态
 */
enum ClickedLabelState {
    NORMAL = 0,
    SELECTED = 1,
};

/**
 * @brief The ServerInfo class 存储 Chat 服务信息
 */
struct ServerInfo
{
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

/**
 * @brief The ChatUIMode enum 聊天模式枚举
 */
enum ChatUIMode {
    SearchMode,  // 搜索模式
    ChatMode,    // 聊天模式
    ContactMode, // 联系人模式
};

/**
 * @brief The ListItemType enum 自定义QListWidgetItem的几种类型
 */
enum ListItemType {
    CHAT_USER_ITEM,    // 聊天用户
    CONTACT_USER_ITEM, // 联系人用户
    SEARCH_USER_ITEM,  // 搜索到的用户
    ADD_USER_TIP_ITEM, // 提示添加用户
    INVALID_ITEM,      // 不可点击条目
    GROUP_TIP_ITEM,    // 分组提示条目
};

/**
 * @brief The ChatRole enum 聊天角色
 */
enum class ChatRole {

    Self,
    Other
};

struct MsgInfo
{
    QString msgFlag; //"text,image,file"
    QString content; //表示文件和图像的url,文本信息
    QPixmap pixmap;  //文件和图片的缩略图
};

#endif // GLOBAL_H
