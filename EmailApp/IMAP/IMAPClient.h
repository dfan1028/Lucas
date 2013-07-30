#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include "IMAPConnection.h"
#include "IMAPParser.h"
#include "IMAPFolder.h"
#include "User.h"

class IMAPClient {

public:
    IMAPClient();
    IMAPConnection::Status connectToHost(User& user);
    void disconnect(); // Disconnects from the host
    bool isConnected(); // Returns the connection status
    QList<IMAPFolder*> getFolders(); // Returns the folders on the IMAP server
    void createFolder(QString name); // Creates a folder on IMAP server
    void deleteFolder(QString name); // Delete the specified folder off the IMAP server
    User getUser();

private:
    QList<IMAPFolder*> getFolders(QString name);
    IMAPConnection m_connection;
    IMAPParser m_parser;
    User m_user;
    void createDefaultFolders();


};

#endif // IMAPCLIENT_H
