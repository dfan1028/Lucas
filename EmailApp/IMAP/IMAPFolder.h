#ifndef IMAPFOLDER_H
#define IMAPFOLDER_H

#include <QObject>
#include "IMAPConnection.h"
#include "IMAPParser.h"

class IMAPEmail;

// Represents a folder on an IMAP Server
class IMAPFolder : public QObject {

Q_OBJECT

public:
    IMAPFolder(QString name, IMAPConnection* con);
    virtual QString getName();
    virtual QList<IMAPFolder*> getChildren(); // Returns the folder children
    virtual void setChildren(QList<IMAPFolder*> children); // Sets the child folders
    virtual void addChild(IMAPFolder* child); // Adds a child folder
    virtual bool hasChildren(); // Returns true if this folder contains children
    virtual int getNumberOfEmails(); // Returns the total number of emails in this folder
    virtual QList<IMAPEmail*> getEmails(int start, int end); // Returns emails in this folder with the given indexes. Emails are ordered descending by date.
    virtual void setHasChildren(bool value); // Sets whether this folder contains child folders
    virtual QList<IMAPEmail*> search(QString query, int start, int end); // Searches this folder for emails matching the given query.
    virtual int searchCount(QString query); // Returns the number of emails the search will return
    virtual int selectFolder(); // Sets this folder as active
    virtual int getUnread(); // Gets the number of unread emails
    virtual void decrementUnread(); // Decrements the number of unread emails
    virtual void incrementUnread(); // Increments the number of unread emails

private:
    QString m_name;
    IMAPConnection* m_connection;
    QList<IMAPFolder*> m_children;
    bool m_hasChildren;
    IMAPParser m_parser;
    int m_unread;

    QList<IMAPEmail*> searchCommand(QString term, int start, int end);

signals:
    void unreadCountUpdated(IMAPFolder* folder);
};

#endif // IMAPFOLDER_H
