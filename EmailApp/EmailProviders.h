#ifndef EMAILPROVIDERS_H
#define EMAILPROVIDERS_H
#include <QString>

class EmailProvider {
public:
    QString getIMAPAddress();
    int getIMAPPort();
    QString getSMTPAddress();
    int getSMTPPort();
protected:
    QString m_imapAddress;
    int m_imapPort;
    QString m_smtpAddress;
    int m_smtpPort;
};

// Server setting for popular email providers
class GmailProvider : public EmailProvider {
public:
    GmailProvider();
};

class YahooProvider : public EmailProvider {
public:
    YahooProvider();
};

#endif // EMAILPROVIDERS_H
