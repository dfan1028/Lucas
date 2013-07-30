#include "User.h"

User::User(QString name, QString username, QString password, EmailProvider provider) {
    m_name = name;
    m_username = username;
    m_password = password;
    m_provider = provider;
}

QString User::getName() {
    return m_name;
}

EmailProvider User::getProvider() {
    return m_provider;
}

QString User::getUsername() {
    return m_username;
}

QString User::getPassword() {
    return m_password;
}
