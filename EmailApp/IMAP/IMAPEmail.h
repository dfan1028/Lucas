#ifndef IMAPEMAIL_H
#define IMAPEMAIL_H

#include "IMAPConnection.h"
#include "IMAPParser.h"
#include "IMAPAttachment.h"
#include "IMAPFolder.h"
#include "EmailAddress.h"
#include <QList>
#include <QDateTime>

class IMAPFolder;

// Represents an email on an IMAP server
class IMAPEmail {
public:
    IMAPEmail(QString id, QString from, QString subject, QDateTime date, IMAPConnection* connection);
    virtual ~IMAPEmail();
    virtual EmailAddress getTo(); // Returns the recipient
    virtual EmailAddress getFrom(); // Returns the sender
    virtual QString getSubject(); // Returns the subject
    virtual QDateTime getDate(); // Returns the date the email was sent
    virtual QList<EmailAddress> getCC(); // Returns the additional recipients
    virtual QList<EmailAddress> getBCC(); // Returns the bcc recipients
    virtual QString getContent(); // Returns the email content
    virtual QList<IMAPAttachment*> getAttachments(); // Returns the attachments
    virtual QString getHTML();
    virtual QString getContentType();
    virtual QString getContentEncoding();
    virtual void setRead(bool value); // Sets the read status
    virtual bool isRead(); // Returns the read status
    virtual void markRead(); // Marks the email as read
    virtual void markUnread(); // Marks the email as unread
    virtual void move(QString folder); // Moves the email to the specified folder
    virtual void deleteEmail(); // Moves the email to the trash
    virtual void setParent(IMAPFolder* parent); // Sets the email's folder
    static bool compareGreaterThan(IMAPEmail* email1, IMAPEmail* email2); // Compares the email by date

private:
    QString m_id;
    bool m_haveHeader;
    EmailAddress m_to;
    EmailAddress m_from;
    QString m_subject;
    QDateTime m_date;
    QList<EmailAddress> m_cc;
    QList<EmailAddress> m_bcc;
    QString m_content;
    QList<IMAPAttachment*> m_attachments;
    IMAPConnection* m_connection;
    IMAPParser* m_parser;
    IMAPFolder* m_parent;
    QString m_html;
    bool m_read;
    bool m_haveEmail;
    QString m_contentType;
    QString m_contentEncoding;

    void loadHeaderValues();
    void loadEmail();
};

#endif // IMAPEMAIL_H
