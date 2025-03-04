#include "customizeedit.h"

CustomizeEdit::CustomizeEdit(QWidget *parent)
    : QLineEdit(parent)
    , max_len_(0)
{
    connect(this, &QLineEdit::textChanged, this, &CustomizeEdit::limitTextLength);
}

void CustomizeEdit::SetMaxLength(int maxlen)
{
    max_len_ = maxlen;
}

void CustomizeEdit::focusOutEvent(QFocusEvent *event)
{
    // 执行失去焦点时的逻辑
    qDebug() << "CustomizeEdit focusout";
    // 调用基类的focusOutEvent方法 保证基类的行为得到执行
    QLineEdit::focusOutEvent(event);
    // 发送失去焦点的信号
    emit sig_foucus_out();
}

void CustomizeEdit::limitTextLength(QString text)
{
    if (max_len_ <= 0) {
        return;
    }

    QByteArray byteArray = text.toUtf8();

    if (byteArray.size() > max_len_) {
        byteArray = byteArray.left(max_len_);
        this->setText(QString::fromUtf8(byteArray));
    }
}
