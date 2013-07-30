#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>
#include "EmailProviders.h"
#include "User.h"
#include "IMAP/IMAPClient.h"

class LoginWindow : public QDialog
{
    Q_OBJECT
public:
    explicit LoginWindow(IMAPClient* client, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    
signals:
    void loginSuccessful();
public slots:
    void login();
    void exit();

private:
    QLabel* m_nameLabel;
    QLabel* m_usernameLabel;
    QLabel* m_passwordLabel;
    QLineEdit* m_nameInput;
    QLineEdit* m_usernameInput;
    QLineEdit* m_passwordInput;
    QButtonGroup* m_providers;
    QRadioButton* m_gmail;
    QRadioButton* m_yahoo;
    QPushButton* m_loginButton;
    QLabel* m_error;
    IMAPClient* m_client;
    QPushButton* m_exit;

    void createUI();
    void setConnections();
};

#endif // LOGINWINDOW_H
