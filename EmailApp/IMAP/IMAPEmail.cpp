#include "IMAPEmail.h"
#include "IMAPTag.h"
#include "MIMEParser.h"

IMAPEmail::IMAPEmail(QString id, QString from, QString subject, QDateTime date, IMAPConnection* connection) {
    m_id = id;
    m_from = m_parser->parseEmailAddress(from);
    m_subject = subject;
    m_connection = connection;
    m_haveHeader = false;
    m_haveEmail = false;
    m_read = false;
    m_date = date;
}

IMAPEmail::~IMAPEmail() {
    for (int i = 0; i < m_attachments.size(); i++) {
        delete m_attachments[i];
    }
}

EmailAddress IMAPEmail::getTo() {
    loadHeaderValues();
    return m_to;
}

EmailAddress IMAPEmail::getFrom() {
    return m_from;
}

QString IMAPEmail::getSubject() {
    return m_subject;
}

QDateTime IMAPEmail::getDate() {
    return m_date;
}

QList<EmailAddress> IMAPEmail::getCC() {
    loadHeaderValues();
    return m_cc;
}

QList<EmailAddress> IMAPEmail::getBCC() {
    loadHeaderValues();
    return m_bcc;
}

QString IMAPEmail::getContent() {
    loadEmail();
    return m_content;
}

QList<IMAPAttachment*> IMAPEmail::getAttachments() {
    loadEmail();
    return m_attachments;
}

QString IMAPEmail::getHTML() {
    loadEmail();
    return m_html;
}

QString IMAPEmail::getContentType() {
    loadHeaderValues();
    return m_contentType;
}

QString IMAPEmail::getContentEncoding() {
    loadHeaderValues();
    return m_contentEncoding;
}

void IMAPEmail::loadHeaderValues() {
    if (!m_haveHeader) {
        m_parent->selectFolder();
        QString tag = IMAPTag::getNextTag();
        QString command = QString("%1 FETCH %2 (FLAGS BODY.PEEK[HEADER.FIELDS (CC TO Content-Type Content-Transfer-Encoding)])\r\n").arg(tag, m_id);
        m_connection->send(command);
        QByteArray response;
        m_connection->read(tag, response);
        m_cc = m_parser->parseCC(response);
        m_to = m_parser->parseTo(response);
        m_contentType = m_parser->parseContentType(response);
        m_contentEncoding = m_parser->parseContentEncoding(response);
    }
    m_haveHeader = true;
}

void IMAPEmail::loadEmail() {
    if (!m_haveEmail) {
        m_parent->selectFolder();
        QString tag = IMAPTag::getNextTag();
        QString command = QString("%1 FETCH %2 BODY[TEXT]\r\n").arg(tag, m_id);
        m_connection->send(command);
        QByteArray response;
        m_connection->read(tag, response);

        MIMEParser parser(response);
        QString contentType = getContentType();
        QString contentEncoding = getContentEncoding();
        if (contentType == "html" || contentType == "text") {
            parser.parseText(contentType, contentEncoding);
        } else {
            parser.parseMime();
        }
        m_attachments = parser.getAttachments();
        m_content = parser.getText();
        m_html = parser.getHTML();
        setRead(true);
        m_parent->decrementUnread();


        tag = IMAPTag::getNextTag();
        command = QString("%1 FETCH %2 BODY[]\r\n").arg(tag, m_id);
        m_connection->send(command);
        m_connection->read(tag, response);
    }
    m_haveEmail = true;
}

void IMAPEmail::setRead(bool value) {
    m_read = value;
}

bool IMAPEmail::isRead() {
    return m_read;
}

void IMAPEmail::markRead() {
    if (m_read == false) {
        m_parent->selectFolder();
        QString tag = IMAPTag::getNextTag();
        QString command = QString("%1 STORE %2 +FLAGS (\\Seen)\r\n").arg(tag, m_id);
        m_connection->send(command);
        QByteArray response;
        m_connection->read(tag, response);

        m_read = true;
        m_parent->decrementUnread();
    }
}

void IMAPEmail::markUnread() {
    if (m_read == true) {
        m_parent->selectFolder();
        QString tag = IMAPTag::getNextTag();
        QString command = QString("%1 STORE %2 -FLAGS (\\Seen)\r\n").arg(tag, m_id);
        m_connection->send(command);
        QByteArray response;
        m_connection->read(tag, response);

        m_read = false;
        m_parent->incrementUnread();
    }
}

void IMAPEmail::move(QString folder) {
    m_parent->selectFolder();
    QString tag = IMAPTag::getNextTag();
    QString command = QString("%1 COPY %2 %3\r\n").arg(tag, m_id, folder);
    m_connection->send(command);
    QByteArray response;
    m_connection->read(tag, response);

    tag = IMAPTag::getNextTag();
    command = QString("%1 STORE %2 +FLAGS (\\Deleted)\r\n").arg(tag, m_id);
    m_connection->send(command);
    m_connection->read(tag, response);

    tag = IMAPTag::getNextTag();
    command = QString("%1 EXPUNGE\r\n").arg(tag);
    m_connection->send(command);
    m_connection->read(tag, response);
}

void IMAPEmail::deleteEmail() {
    move("Trash");
}

void IMAPEmail::setParent(IMAPFolder* parent) {
    m_parent = parent;
}

bool IMAPEmail::compareGreaterThan(IMAPEmail* email1, IMAPEmail* email2) {
        return email1->getDate() > email2->getDate();
}
