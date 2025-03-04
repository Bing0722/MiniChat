#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class ChatView : public QWidget
{
    Q_OBJECT
public:
    ChatView(QWidget *parent = nullptr);
    void appendChatItem(QWidget *item);                  // 尾插
    void prependChat(QWidget *item);                     // 头插
    void insertChatItem(QWidget *brfore, QWidget *item); //中间插

protected:
    virtual bool eventFilter(QObject *watch, QEvent *ev) override;
    virtual void paintEvent(QPaintEvent *ev) override;

private slots:
    void onVScrollBarMoved(int min, int max);

private:
    void initStyleSheet();

    QVBoxLayout *m_pV1;
    QScrollArea *m_pScrollArea;
    bool isAppended;
};

#endif // CHATVIEW_H
