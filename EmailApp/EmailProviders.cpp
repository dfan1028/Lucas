#include "EmailProviders.h"

QString EmailProvider::getIMAPAddress() {
    return m_imapAddress;
}

int EmailProvider::getIMAPPort() {
    return m_imapPort;
}

QString EmailProvider::getSMTPAddress() {
    return m_smtpAddress;
}

int EmailProvider::getSMTPPort() {
    return m_smtpPort;
}

GmailProvider::GmailProvider() {
    m_imapAddress = "imap.gmail.com";
    m_imapPort = 993;
    m_smtpAddress = "smtp.gmail.com";
    m_smtpPort = 465;
}

YahooProvider::YahooProvider() {
    m_imapAddress = "imap.mail.yahoo.com";
    m_imapPort = 993;
    m_smtpAddress = "smtp.mail.yahoo.com";
    m_smtpPort = 465;
}
