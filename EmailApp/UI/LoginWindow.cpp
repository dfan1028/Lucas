#include "LoginWindow.h"
#include <QGridLayout>
#include <QApplication>

LoginWindow::LoginWindow(IMAPClient* client, QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags)
{
    m_client = client;
    createUI();
    setConnections();
}

void LoginWindow::createUI() {

    QGridLayout* layout = new QGridLayout();
    setLayout(layout);

    m_nameLabel = new QLabel("Name", this);
    m_nameInput = new QLineEdit("Lucas", this);
    m_usernameLabel = new QLabel("Username", this);
    m_usernameInput = new QLineEdit("lucas.cmpt212@gmail.com", this);
    m_passwordLabel = new QLabel("Password", this);
    m_passwordInput = new QLineEdit("1lucas23", this);
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_providers = new QButtonGroup(this);
    m_gmail = new QRadioButton("Gmail", this);
    m_gmail->setChecked(true);
    m_yahoo = new QRadioButton("Yahoo", this);
    m_providers->addButton(m_gmail);
    m_providers->addButton(m_yahoo);
    m_providers->setExclusive(true);

    QHBoxLayout* submitLayout = new QHBoxLayout();
    m_loginButton = new QPushButton("Login", this);
    m_exit = new QPushButton("Exit", this);
    submitLayout->addWidget(m_loginButton);
    submitLayout->addWidget(m_exit);
    m_error = new QLabel(this);
    m_error->hide();

    layout->addWidget(m_nameLabel, 0, 0);
    layout->addWidget(m_nameInput, 0, 1);
    layout->addWidget(m_usernameLabel, 1, 0);
    layout->addWidget(m_usernameInput, 1, 1);
    layout->addWidget(m_passwordLabel, 2, 0);
    layout->addWidget(m_passwordInput, 2, 1);
    layout->addWidget(m_gmail, 3, 0);
    layout->addWidget(m_yahoo, 4, 0);
    layout->addLayout(submitLayout, 5, 1);
    layout->addWidget(m_error, 6, 0);

    setMinimumWidth(400);
    setMinimumHeight(250);
    setModal(true);
}

void LoginWindow::setConnections() {
    connect(m_loginButton, SIGNAL(clicked()), SLOT(login()));
    connect(m_exit, SIGNAL(clicked()), SLOT(exit()));
}

void LoginWindow::login() {
    QString name = m_nameInput->text();
    QString username = m_usernameInput->text();
    QString password = m_passwordInput->text();

    EmailProvider provider = GmailProvider();
    if (m_yahoo->isChecked()) {
        provider = YahooProvider();
    }

    User user(name, username, password, provider);
    IMAPConnection::Status status = m_client->connectToHost(user);

    if (status == IMAPConnection::ConnectionFailure) {
        m_error->setText("Error: Connection failure");
        m_error->show();
    } else if (status == IMAPConnection::InvalidCredentials) {
        m_error->setText("Error: Invalid Credentials");
        m_error->show();
    } else {
        hide();
        emit loginSuccessful();
    }
}

void LoginWindow::exit() {
    QApplication::exit(0);
}
