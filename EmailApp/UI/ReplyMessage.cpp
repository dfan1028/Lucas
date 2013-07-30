#include "ReplyMessage.h"
#include "NewMessage.h"
#include "SMTP/smtpclient.h"
#include "SMTP/MimeMessage.h"
#include "SMTP/MimeText.h"
#include "SMTP/MimeAttachment.h"
#include <QFileDialog>
#include <QVector>

ReplyMessage::ReplyMessage() :
    NewMessage()
{
    //default constructor, calls parent class: NewMessage
}

ReplyMessage::~ReplyMessage()
{
    //delete ui;
}
/*
 *  Sets to, body and subject fields to those appropriate for
 *  forwarding a message.
 */
void ReplyMessage::setTo(const QString &to)
{
    NewMessage::setTo(to);
}
void ReplyMessage::setBody(const QString &body)
{
    NewMessage::setBody(body);
}
void ReplyMessage::setSubject(const QString &subject)
{
    NewMessage::setSubject(QString::fromUtf8("RE: ") + subject);
}

void ReplyMessage::sendEmail ()
{
    NewMessage::sendEmail();
}


