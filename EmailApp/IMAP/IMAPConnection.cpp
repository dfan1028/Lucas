#include "IMAPConnection.h"
#include "IMAPTag.h"
#include <QThread>

IMAPConnection::IMAPConnection() {
}

IMAPConnection::~IMAPConnection() {
}


void IMAPConnection::connectToHost(User& user) {
    EmailProvider provider = user.getProvider();
    m_socket.connectToHostEncrypted(provider.getIMAPAddress(), provider.getIMAPPort());
    m_socket.waitForConnected();

    if (m_socket.state() == QSslSocket::ConnectedState) {
        m_status = Connected;
    } else {
        m_status = ConnectionFailure;
        return;
    }

    QString tag = IMAPTag::getNextTag();
    QString loginCommand = QString("%1 LOGIN %2 %3\r\n").arg(tag, user.getUsername(), user.getPassword());

    send(loginCommand);
    QByteArray responseData;
    read(tag, responseData);

    if (!m_parser.successfulConnect(tag, responseData)) {
       m_status = InvalidCredentials;
       disconnect();
    }
}

void IMAPConnection::disconnect() {
    QString tag = IMAPTag::getNextTag();
    QString loginCommand = QString("%1 BYE\r\n").arg(tag);
    send(loginCommand);
    QByteArray responseData;
    read(tag, responseData);
    m_socket.disconnectFromHost();
}

bool IMAPConnection::isConnected() {
    return m_socket.ConnectedState == QAbstractSocket::UnconnectedState;
}

void IMAPConnection::send(const QString& request) {
    QByteArray data;
    qDebug() << request;
    data.append(request);
    m_socket.write(data);
}

void IMAPConnection::read(const QString& tag, QByteArray& response) {
    response.clear();
    bool doneReading = false;
    QString terminal = QString("%1").arg(tag);
    while (!doneReading) {
        m_socket.waitForReadyRead(50);
        QString data(m_socket.readAll());
        if (data.contains(terminal)) {
            doneReading = true;
        }
        response.append(data);
    }
    qDebug() << response;
}

IMAPConnection::Status IMAPConnection::getStatus() {
    return m_status;
}
