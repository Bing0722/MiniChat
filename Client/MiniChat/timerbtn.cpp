#include "timerbtn.h"
#include <QMouseEvent>
#include <QDebug>


TimerBtn::TimerBtn(QWidget *parent): QPushButton(parent), count_(10)
{
    timer_ = new QTimer(this);
    QObject::connect(timer_, &QTimer::timeout, this, [this]() {
        count_--;
        if (count_ <= 0){
            timer_->stop();
            count_ = 10;
            this->setText("获取");
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(count_));
    });
}

TimerBtn::~TimerBtn() {
    timer_->stop();
}

void TimerBtn::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton){
        qDebug() << "MyButton was release!";
        this->setEnabled(false);
        this->setText(QString::number(count_));
        timer_->start(1000);
        emit clicked();
    }

    QPushButton::mousePressEvent(e);
}
