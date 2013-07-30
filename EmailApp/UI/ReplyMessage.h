#ifndef REPLYMESSAGE_H
#define REPLYMESSAGE_H
#include "NewMessage.h"
class ReplyMessage : public NewMessage
{
    Q_OBJECT

public:
    explicit ReplyMessage();
    void setTo(const QString &to);
    void setBody(const QString &body);
    void setSubject(const QString &subject);
    ~ReplyMessage();

private:
    void sendEmail();


private slots:
};

#endif // REPLYMESSAGE_H
