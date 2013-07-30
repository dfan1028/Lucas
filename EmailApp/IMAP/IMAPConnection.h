#ifndef IMAPCONNECTION_H
#define IMAPCONNECTION_H

#include <QtNetwork/QSslSocket>
#include "User.h"
#include "IMAPParser.h"

class IMAPConnection {

public:
    enum Status {
        Connected,
        InvalidCredentials,
        ConnectionFailure
    };

    IMAPConnection();
    ~IMAPConnection();
    void connectToHost(User& user);
    void disconnect();
    bool isConnected();
    void send(const QString& request);
    void read(const QString& tag, QByteArray& response);
    Status getStatus();

private:
    QSslSocket m_socket;
    Status m_status;
    IMAPParser m_parser;
};

#endif // IMAPCONNECTION_H
