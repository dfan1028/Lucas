#ifndef EMAILHEADERWIDGET_H
#define EMAILHEADERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "IMAP/IMAPEmail.h"
#include "AttachmentEnclosedDisplay.h"
#include <QHBoxLayout>

class EmailHeaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmailHeaderWidget(QWidget *parent = 0);
    
signals:
    void replyClicked(IMAPEmail* email);
    void replyAllClicked(IMAPEmail* email);
    void forwardClicked(IMAPEmail* email);
    void deleteClicked(IMAPEmail* email);
public slots:
    void setEmail(IMAPEmail* email);
    void clearAndHide();
    void replyPressed();
    void replyAllPressed();
    void forwardPressed();
    void deletePressed();

private:
    QLabel* m_from;
    QLabel* m_fromValue;
    QLabel* m_subject;
    QLabel* m_subjectValue;
    QLabel* m_to;
    QLabel* m_toValue;
    QLabel* m_cc;
    QLabel* m_ccValue;
    QPushButton* m_reply;
    QPushButton* m_replyAll;
    QPushButton* m_forward;
    QPushButton* m_delete;
    QLabel* m_date;
    IMAPEmail* m_currentEmail;
    QList<AttachmentEnclosedDisplay*> m_attachments;
    QHBoxLayout* m_attachmentLayout;

    void createUI();
    void setConnections();
};

#endif // EMAILHEADERWIDGET_H
