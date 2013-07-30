#include "NewMessage.h"
#include "ForwardMessage.h"
#include "SMTP/smtpclient.h"
#include "SMTP/MimeMessage.h"
#include "SMTP/MimeText.h"
#include "SMTP/MimeAttachment.h"
#include <QFileDialog>
#include <QVector>

ForwardMessage::ForwardMessage() :
    NewMessage()
{
    //default constructor, calls parent class: NewMessage
}

ForwardMessage::~ForwardMessage()
{
    //delete ui;
}
/*
 *  Sets to, body and subject fields to those appropriate for
 *  forwarding a message.
 */
void ForwardMessage::setTo(const QString &to)
{
    NewMessage::setTo(to);
}
void ForwardMessage::setBody(const QString &body)
{
    NewMessage::setBody(body);
}
void ForwardMessage::setSubject(const QString &subject)
{
    NewMessage::setSubject(QString::fromUtf8("FW: ") + subject);
}
void ForwardMessage::sendEmail()
{
    NewMessage::sendEmail();
}



