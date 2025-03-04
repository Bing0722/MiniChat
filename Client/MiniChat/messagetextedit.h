#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H

#include <QApplication>
#include <QDrag>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QMimeData>
#include <QMimeType>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QTextEdit>
#include <QVector>
#include "global.h"

class MessageTextEdit : public QTextEdit
{
public:
    MessageTextEdit(QWidget *parent = nullptr);

    ~MessageTextEdit();

    QVector<MsgInfo> getMsgList();

private:
    void insertMsgList(QVector<MsgInfo> &list, QString flag, QString text, QPixmap pix);

    QVector<MsgInfo> mMsgList;
    QVector<MsgInfo> mGetMsgList;
};
#endif // MESSAGETEXTEDIT_H
