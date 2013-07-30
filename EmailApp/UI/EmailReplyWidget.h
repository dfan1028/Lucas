#ifndef EMAILREPLYWIDGET_H
#define EMAILREPLYWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include "EmailTextBox.h"
#include "IMAP/IMAPEmail.h"
#include "AttachmentOpenDisplay.h"

enum SendType {
    COMPOSE,
    FORWARD,
    REPLY
};

class EmailReplyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmailReplyWidget(QWidget *parent = 0);
    

signals:
    void submitPressed();
    void closePressed();

public slots:
    void showCC();
    void showBCC();
    void reply(IMAPEmail* email);
    void replyToAll(IMAPEmail* email);
    void forward(IMAPEmail* email);
    void compose();
    void reset();
    void addAttachment();
    void attachmentRemoved(AttachmentOpenDisplay* attachment);
    void send();

private:
    QTextEdit* m_replyBox;
    QLineEdit* m_subject;
    EmailTextBox* m_to;
    EmailTextBox* m_cc;
    EmailTextBox* m_bcc;
    QPushButton* m_showCC;
    QPushButton* m_showBCC;
    QPushButton* m_submit;
    QPushButton* m_addAttachment;
    QPushButton* m_close;
    QList<AttachmentOpenDisplay*> m_attachments;
    QHBoxLayout* m_attachmentLayout;
    IMAPEmail* m_currentEmail;
    SendType m_currentMode;

    void createUI();
    void setConnections();
    QString getOriginalEmailInfo();
};

#endif // EMAILREPLYWIDGET_H
