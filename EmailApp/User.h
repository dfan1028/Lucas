#ifndef USER_H
#define USER_H

#include <QString>
#include "EmailProviders.h"

// Represent's a user's email settings
class User {
public:
    User() {};
    User(QString name, QString username, QString password, EmailProvider provider);

    QString getName();
    QString getUsername();
    QString getPassword();
    EmailProvider getProvider();
private:
    EmailProvider m_provider;
    QString m_name;
    QString m_username;
    QString m_password;
};

#endif // USER_H
