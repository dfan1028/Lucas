#ifndef IMAPPARSER_H
#define IMAPPARSER_H

#include "EmailAddress.h"
#include <QList>
#include <QStringList>


class IMAPFolder;
class IMAPEmail;
class IMAPConnection;

// Parses IMAP responses
class IMAPParser {

public:
    IMAPParser();
    QList<IMAPFolder*> parseFolders(QString data, IMAPConnection& connection);
    int parseSelect(QString data);
    QList<IMAPEmail*> parseEmails(QString data, IMAPConnection& connection, IMAPFolder* folder);
    IMAPEmail* parseEmail(QString data, IMAPConnection& connection, IMAPFolder* folder);
    QString parseBody(QString data);
    QStringList parseSearch(QString data);
    EmailAddress parseEmailAddress(QString data);
    QList<EmailAddress> parseCC(QString data);
    EmailAddress parseTo(QString data);
    QString parseContentType(QString data);
    QString parseContentEncoding(QString data);
    bool successfulConnect(QString tag, QString data);
};

#endif // IMAPPARSER_H
