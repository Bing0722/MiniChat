#include "messagetextedit.h"
#include <QDebug>
#include <QMessageBox>

MessageTextEdit::MessageTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    //this->setStyleSheet("border: none;");
    this->setMaximumHeight(60);

    connect(this, SIGNAL(textChanged()), this, SLOT(textEditChanged()));
}

MessageTextEdit::~MessageTextEdit() {}

QVector<MsgInfo> MessageTextEdit::getMsgList()
{
    mGetMsgList.clear();

    QString doc = this->document()->toPlainText();
    QString text = ""; //存储文本信息
    int indexUrl = 0;
    int count = mMsgList.size();

    for (int index = 0; index < doc.size(); index++) {
        if (doc[index] == QChar::ObjectReplacementCharacter) {
            if (!text.isEmpty()) {
                QPixmap pix;
                insertMsgList(mGetMsgList, "text", text, pix);
                text.clear();
            }
            while (indexUrl < count) {
                MsgInfo msg = mMsgList[indexUrl];
                if (this->document()->toHtml().contains(msg.content, Qt::CaseSensitive)) {
                    indexUrl++;
                    mGetMsgList.append(msg);
                    break;
                }
                indexUrl++;
            }
        } else {
            text.append(doc[index]);
        }
    }
    if (!text.isEmpty()) {
        QPixmap pix;
        insertMsgList(mGetMsgList, "text", text, pix);
        text.clear();
    }
    mMsgList.clear();
    this->clear();
    return mGetMsgList;
}

void MessageTextEdit::insertMsgList(QVector<MsgInfo> &list, QString flag, QString text, QPixmap pix)
{
    MsgInfo msg;
    msg.msgFlag = flag;
    msg.content = text;
    msg.pixmap = pix;
    list.append(msg);
}
