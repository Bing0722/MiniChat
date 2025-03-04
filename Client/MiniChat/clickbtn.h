#ifndef CLICKBTN_H
#define CLICKBTN_H

#include <QPushButton>

class ClickBtn : public QPushButton
{
    Q_OBJECT
public:
    ClickBtn(QWidget *parent = nullptr);
    ~ClickBtn();

    void SetState(QString normal, QString hover, QString press);

protected:
    virtual void enterEvent(QEnterEvent *event) override;    // 鼠标进入
    virtual void leaveEvent(QEvent *event) override;         // 鼠标离开
    virtual void mousePressEvent(QMouseEvent *e) override;   // 鼠标按下
    virtual void mouseReleaseEvent(QMouseEvent *e) override; // 鼠标释放

private:
    QString normal_;
    QString hover_;
    QString press_;
};

#endif // CLICKBTN_H
