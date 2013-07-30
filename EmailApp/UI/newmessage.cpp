#include "NewMessage.h"
#include "ui_newmessage.h"
#include "SMTP/smtpclient.h"
#include "SMTP/MimeMessage.h"
#include "SMTP/MimeText.h"
#include "SMTP/MimeAttachment.h"
#include <QFileDialog>
#include <QVector>

NewMessage::NewMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewMessage)
{
    ui->setupUi(this);
}

NewMessage::~NewMessage()
{
    delete ui;
}
void NewMessage::setTo(const QString &to)
{
    ui->toLine->setText(to);
    this->to = to;
}
void NewMessage::setBody(const QString &body)
{
    ui->messageBody->setText(body);
    this->body = body;
}
void NewMessage::setSubject(const QString &subject)
{
    ui->subjectLine->setText(subject);
    this->subject = subject;
}

void NewMessage::on_sendButton_clicked()
{
    //form email, send it, close new message window
    //gets the to, cc, bcc fields, and message body
    body = ui->messageBody->toPlainText();
    to = ui->toLine->text();
    cc = ui->ccLine->text();
    bcc = ui->bccLine->text();
    subject = ui->subjectLine->text();
    sendEmail();
    this->close();  //would be nice to give the user feedback before this closes, or maybe display feedback on the status bar.
}

void NewMessage::on_cancelButton_clicked()
{
    //close new message window
    this->close();
}

void NewMessage::sendEmail ()
{
    //Connecting SMTP
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);
    smtp.setUser("lucas.cmpt212@gmail.com");
    smtp.setPassword("1lucas23");

    //Setting up email
    MimeMessage message;
    message.setSender(new EmailAddress("lucas.cmpt212@gmail.com", "Lucas"));
    message.addRecipient(new EmailAddress("con.brad@gmail.com", "You"));
    message.setSubject("Testing SMTP sending email");

    //Set body of email
    MimeText text;
    text.setText("Hi,\nJust testing out sending emails woohoo.\n");
    message.addPart(&text);
    qDebug() << message.toString();

    MimeAttachment attachment (new QFile(tr("C:\\Users\\Conor\\Documents\\CMPT 212 Project\\Penguins.jpg")));

    // the file type can be setted. (by default is application/octet-stream)
    //attachment.setContentType("image/jpg");
    // Now add it to message
    message.addPart(&attachment);

    // Add an another attachment
    //message.addPart(new MimeAttachment(new QFile("document.pdf")));

    //Connect, login, send email, and quit
    smtp.connect_and_send(message);    

}

void NewMessage::on_pushButton_clicked()
{
    //grabs path to file that user selected as attachment, appends it to our list of attachments
    fileNames.append(QFileDialog::getOpenFileNames(this, tr("Attach File")));
    if(!fileNames.empty())
    {
        ui->listWidget->addItem(fileNames.back());
    }
}
