#ifndef FORWARDMESSAGE_H
#define FORWARDMESSAGE_H
#include "newmessage.h"
class ForwardMessage : public NewMessage
{
    Q_OBJECT

public:
    explicit ForwardMessage();
    void setTo(const QString &to);
    void setBody(const QString &body);
    void setSubject(const QString &subject);
    ~ForwardMessage();

private:
    void sendEmail();


private slots:
};
#endif // FORWARDMESSAGE_H
