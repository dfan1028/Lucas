#include "IMAPClient.h"
#include "IMAPTag.h"
#include "IMAPFolder.h"

IMAPClient::IMAPClient() : m_connection() {
}

IMAPConnection::Status  IMAPClient::connectToHost(User& user) {
    m_user = user;
    m_connection.connectToHost(user);
    if (m_connection.getStatus() == IMAPConnection::Connected) {
        createDefaultFolders();
    }
    return m_connection.getStatus();
}

void IMAPClient::disconnect() {
    m_connection.disconnect();
}

bool IMAPClient::isConnected() {
    return m_connection.isConnected();
}

QList<IMAPFolder*> IMAPClient::getFolders() {
    return getFolders("");
}

QList<IMAPFolder*> IMAPClient::getFolders(QString name) {
    QString tag = IMAPTag::getNextTag();
    QString command = QString("%1 LIST  %2/%\r\n").arg(tag, name);
    m_connection.send(command);
    QByteArray response;
    m_connection.read(tag, response);
    QList<IMAPFolder*> folders = m_parser.parseFolders(QString(response), m_connection);

    for (int i = 0; i < folders.size(); i++) {
        if (folders[i]->hasChildren()) {
            QList<IMAPFolder*> children = getFolders(folders[i]->getName());
            folders[i]->setChildren(children);
        }
    }
    return folders;
}

void IMAPClient::createDefaultFolders() {
    createFolder("Trash");
}

void IMAPClient::createFolder(QString name) {
    QString tag = IMAPTag::getNextTag();
    QString command = QString("%1 CREATE %2\r\n").arg(tag, name);
    m_connection.send(command);
    QByteArray response;
    m_connection.read(tag, response);
}

void IMAPClient::deleteFolder(QString name) {
    QString tag = IMAPTag::getNextTag();
    QString command = QString("%1 DELETE %2\r\n").arg(tag, name);
    m_connection.send(command);
    QByteArray response;
    m_connection.read(tag, response);
}

User IMAPClient::getUser() {
    return m_user;
}
