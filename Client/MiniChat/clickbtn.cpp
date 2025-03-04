#include "clickbtn.h"
#include "global.h"

ClickBtn::ClickBtn(QWidget *parent)
    : QPushButton(parent)
{
    setCursor(Qt::PointingHandCursor); // 设置光标为小手
}

ClickBtn::~ClickBtn() {}

void ClickBtn::SetState(QString normal, QString hover, QString press)
{
    hover_ = hover;
    normal_ = normal;
    press_ = press;
    setProperty("state", normal);
    repolish(this);
    update();
}

void ClickBtn::enterEvent(QEnterEvent *event)
{
    setProperty("state", hover_);
    repolish(this);
    update();
    QPushButton::enterEvent(event);
}

void ClickBtn::leaveEvent(QEvent *event)
{
    setProperty("state", normal_);
    repolish(this);
    update();
    QPushButton::leaveEvent(event);
}

void ClickBtn::mousePressEvent(QMouseEvent *e)
{
    setProperty("state", press_);
    repolish(this);
    update();
    QPushButton::mousePressEvent(e);
}

void ClickBtn::mouseReleaseEvent(QMouseEvent *e)
{
    setProperty("state", hover_);
    repolish(this);
    update();
    QPushButton::mouseReleaseEvent(e);
}
