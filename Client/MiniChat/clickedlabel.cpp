#include "clickedlabel.h"
#include <QMouseEvent>

ClickedLabel::ClickedLabel(QWidget *parent)
    : QLabel(parent)
    , cursor_state_(ClickedLabelState::NORMAL)
{
    setCursor(Qt::PointingHandCursor); // 设置光标为小手
}

// 处理鼠标点击事件
void ClickedLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        if (cursor_state_ == ClickedLabelState::NORMAL) {
            qDebug() << "clicked, change to selected hover: " << selected_press_;
            cursor_state_ = ClickedLabelState::SELECTED;
            setProperty("state", selected_press_);
            repolish(this);
            update();
        } else {
            qDebug() << "clicked, change to normal hover: " << normal_hover_;
            cursor_state_ = ClickedLabelState::NORMAL;
            setProperty("state", normal_press_);
            repolish(this);
            update();
        }
        emit clicked();
        return;
    }

    // 调用基类的鼠标点击事件以保证正常处理事件
    QLabel::mousePressEvent(ev);
}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (cursor_state_ == ClickedLabelState::NORMAL) {
            setProperty("state", normal_hover_);
            repolish(this);
            update();
        } else {
            setProperty("state", selected_hover_);
            repolish(this);
            update();
        }
        emit clicked();
        return;
    }
    QLabel::mouseReleaseEvent(e);
}

// 悬浮事件
void ClickedLabel::enterEvent(QEnterEvent *event)
{
    if (cursor_state_ == ClickedLabelState::NORMAL) {
        qDebug() << "enter, change to normal hover: " << normal_hover_;
        setProperty("state", normal_hover_);
        repolish(this);
        update();
    } else {
        qDebug() << "enter, change to select hover: " << selected_hover_;
        setProperty("state", selected_hover_);
        repolish(this);
        update();
    }

    QLabel::enterEvent(event);
}

// 离开事件
void ClickedLabel::leaveEvent(QEvent *event)
{
    if (cursor_state_ == ClickedLabelState::NORMAL) {
        qDebug() << "leave, change to normal: " << normal_;
        setProperty("state", normal_);
        repolish(this);
        update();
    } else {
        qDebug() << "leave, change to hover: " << selected_;
        setProperty("state", selected_);
        repolish(this);
        update();
    }

    QLabel::leaveEvent(event);
}

void ClickedLabel::SetState(QString normal,
                            QString hover,
                            QString press,
                            QString select,
                            QString select_hover,
                            QString select_press)
{
    normal_ = normal;
    normal_hover_ = hover;
    normal_press_ = press;

    selected_ = select;
    selected_hover_ = select_hover;
    selected_press_ = select_press;

    setProperty("state", normal);
    repolish(this);
}

ClickedLabelState ClickedLabel::GetCursorState()
{
    return cursor_state_;
}
