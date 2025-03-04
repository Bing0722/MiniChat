#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include "global.h"

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent);
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

    void SetState(QString normal="", QString hover="", QString press="",
                  QString select="", QString select_hover="", QString select_press="");

    ClickedLabelState GetCursorState();

protected:

private:
    QString normal_;
    QString normal_hover_;
    QString normal_press_;

    QString selected_;
    QString selected_hover_;
    QString selected_press_;

    ClickedLabelState cursor_state_;

signals:
    void clicked(void);
};

#endif // CLICKEDLABEL_H
