#include "IMAPFolder.h"
#include "IMAPTag.h"
#include <QDate>
#include <QtAlgorithms>
#include "IMAPEmail.h"

IMAPFolder::IMAPFolder(QString name, IMAPConnection* con) {
    m_name = name;
    m_connection = con;
    m_hasChildren = false;
    m_unread = 0;
}

QString IMAPFolder::getName() {
    return m_name;
}

QList<IMAPFolder*> IMAPFolder::getChildren() {
    return m_children;
}

void IMAPFolder::addChild(IMAPFolder* child) {
    m_children.push_back(child);
}

bool IMAPFolder::hasChildren() {
    return m_hasChildren;
}

int IMAPFolder::getNumberOfEmails() {
    return selectFolder();
}

QList<IMAPEmail*> IMAPFolder::getEmails(int start, int end) {
    int count = getNumberOfEmails();
    if (end > count) {
        end = count;
    }
    int initialStart = start;
    start = count - (end - initialStart) - initialStart + 1;
    end = count - initialStart;
    QString tag = IMAPTag::getNextTag();
    QString command = QString("%1 FETCH %2:%3 (FLAGS BODY.PEEK[HEADER.FIELDS (FROM SUBJECT DATE)])\r\n").arg(tag, QString::number(start), QString::number(end));
    m_connection->send(command);
    QByteArray response;
    m_connection->read(tag, response);
    QList<IMAPEmail*> emails = m_parser.parseEmails(response, *m_connection, this);
    qSort(emails.begin(), emails.end(), IMAPEmail::compareGreaterThan);
    return emails;
}

void IMAPFolder::setChildren(QList<IMAPFolder*> children) {
    m_children = children;
}

void IMAPFolder::setHasChildren(bool value) {
    m_hasChildren = value;
}

QList<IMAPEmail*> IMAPFolder::search(QString query, int start, int end) {
    QString command = QString("CHARSET utf-8 BODY \"%1\"").arg(query);
    return searchCommand(command, start, end);
}

int IMAPFolder::searchCount(QString query) {
    selectFolder();
    QString tag = IMAPTag::getNextTag();
    QString command = QString("%1 SEARCH CHARSET utf-8 BODY \"%2\"\r\n").arg(tag, query);
    m_connection->send(command);
    QByteArray response;
    m_connection->read(tag, response);
    QStringList ids = m_parser.parseSearch(response);
    return ids.size();
}

int IMAPFolder::selectFolder() {
    QString tag = IMAPTag::getNextTag();
    QString command = QString("%1 SELECT \"%2\"\r\n").arg(tag, m_name);
    m_connection->send(command);
    QByteArray response;
    m_connection->read(tag, response);
    int emailCount = m_parser.parseSelect(response);

    tag = IMAPTag::getNextTag();
    command = QString("%1 SEARCH UNSEEN\r\n").arg(tag);
    m_connection->send(command);
    m_connection->read(tag, response);
    m_unread = m_parser.parseSearch(response).size();
    return emailCount;
}

int IMAPFolder::getUnread() {
    return m_unread;
}

void IMAPFolder::decrementUnread() {
    m_unread--;
    emit unreadCountUpdated(this);
}

void IMAPFolder::incrementUnread() {
    m_unread++;
    emit unreadCountUpdated(this);
}

QList<IMAPEmail*> IMAPFolder::searchCommand(QString term, int start, int end) {
    selectFolder();
    QString tag = IMAPTag::getNextTag();
    QString command = QString("%1 SEARCH %2\r\n").arg(tag, term);
    m_connection->send(command);
    QByteArray response;
    m_connection->read(tag, response);

    QStringList emailIDList = m_parser.parseSearch(response);
    int count = emailIDList.size();
    if (end > count) {
        end = count;
    }
    int initialStart = start;
    start = count - (end - initialStart) - initialStart + 1;
    end = count - initialStart;
    for (int i = 0; (i < start) && i < emailIDList.size(); i++) {
        emailIDList.removeAt(i);
    }

    while(emailIDList.size() > (end - start)) {
        emailIDList.pop_back();
    }
    QList<IMAPEmail*> emails;

    if (emailIDList.size() > 0) {
        tag = IMAPTag::getNextTag();
        command = QString("%1 FETCH %2 (FLAGS BODY.PEEK[HEADER.FIELDS (FROM SUBJECT DATE)])\r\n").arg(tag, emailIDList.join(","));
        m_connection->send(command);
        m_connection->read(tag, response);
        emails = m_parser.parseEmails(response, *m_connection, this);
    }
    qSort(emails.begin(), emails.end(), IMAPEmail::compareGreaterThan);
    return emails;
}
