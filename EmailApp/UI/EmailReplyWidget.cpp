#include "EmailReplyWidget.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include "SMTP/SmtpMime"

EmailReplyWidget::EmailReplyWidget(QWidget *parent) :
    QWidget(parent)
{
    createUI();
    setConnections();
    reset();
}

void EmailReplyWidget::createUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    QHBoxLayout* subjectLayout = new QHBoxLayout();
    QHBoxLayout* subjectLeft = new QHBoxLayout();
    QHBoxLayout* subjectRight = new QHBoxLayout();


    QWidget* subjectWidget = new QWidget(this);
    mainLayout->addWidget(subjectWidget, 1);
    subjectWidget->setLayout(subjectLayout);
    subjectLayout->addLayout(subjectLeft, 1);
    subjectLayout->addLayout(subjectRight, 10);

    QLabel* subjectLabel = new QLabel("Subject", this);
    m_subject = new QLineEdit(this);
    subjectLeft->addWidget(subjectLabel);
    subjectRight->addWidget(m_subject);

    QHBoxLayout* top = new QHBoxLayout();
    m_to = new EmailTextBox("To", this);
    m_showCC = new QPushButton("Cc", this);
    m_showBCC = new QPushButton("Bcc", this);

    top->addWidget(m_to);
    top->addWidget(m_showCC);
    top->addWidget(m_showBCC);
    mainLayout->addLayout(top, 1);

    m_cc = new EmailTextBox("Cc", this);
    m_bcc = new EmailTextBox("Bcc", this);
    m_replyBox = new QTextEdit(this);
    mainLayout->addWidget(m_cc, 1);
    mainLayout->addWidget(m_bcc, 1);

    QHBoxLayout* attachment = new QHBoxLayout();
    mainLayout->addLayout(attachment, 1);

    m_attachmentLayout = new QHBoxLayout();
    attachment->addLayout(m_attachmentLayout);
    QHBoxLayout* attachmentRight = new QHBoxLayout();
    attachmentRight->addStretch();
    attachment->addLayout(attachmentRight);

    mainLayout->addWidget(m_replyBox, 10);
    QHBoxLayout* bottom = new QHBoxLayout();
    m_submit = new QPushButton("Send", this);
    m_addAttachment = new QPushButton("Attachment", this);
    m_close = new QPushButton("Close", this);
    bottom->addWidget(m_submit);
    bottom->addWidget(m_addAttachment);
    bottom->addWidget(m_close);
    bottom->addStretch();

    mainLayout->addLayout(bottom, 1);
    // Visibility
    m_cc->hide();
    m_bcc->hide();

}

void EmailReplyWidget::setConnections() {
    connect(m_showCC, SIGNAL(clicked()), SLOT(showCC()));
    connect(m_showBCC, SIGNAL(clicked()), SLOT(showBCC()));
    connect(m_addAttachment, SIGNAL(clicked()), SLOT(addAttachment()));
    connect(m_submit, SIGNAL(clicked()) , SLOT(send()) );
    connect(m_submit, SIGNAL(clicked()), SIGNAL(submitPressed()));
    connect(m_close, SIGNAL(clicked()), SIGNAL(closePressed()));
}

void EmailReplyWidget::showCC() {
    m_cc->show();
    m_showCC->hide();
}

void EmailReplyWidget::showBCC() {
    m_bcc->show();
    m_showBCC->hide();
}

void EmailReplyWidget::reply(IMAPEmail* email) {
    reset();
    m_currentEmail = email;
    m_currentMode = REPLY;
    QString to = email->getFrom().getAddress();
    if (!to.isEmpty()) {
        m_to->addEmail(to);
    }
    show();
}

void EmailReplyWidget::replyToAll(IMAPEmail* email) {
    reset();
    m_currentEmail = email;
    m_currentMode = REPLY;
    QString to = email->getFrom().getAddress();
    if (!to.isEmpty()) {
        m_to->addEmail(to);
    }

    QList<EmailAddress> cc = email->getCC();
    if (cc.size() > 0) {
        for (int i = 0; i < cc.size(); i++) {
            QString email = cc[i].getAddress();
            m_cc->addEmail(email);
        }
        showCC();
    }
    show();
}

void EmailReplyWidget::forward(IMAPEmail* email) {
    reset();
    m_currentEmail = email;
    m_currentMode = FORWARD;
    show();
}

void EmailReplyWidget::compose() {
    reset();
    m_currentEmail = NULL;
    m_currentMode = COMPOSE;
    m_subject->show();
    show();
}

void EmailReplyWidget::reset() {
    m_currentEmail = NULL;
    m_to->reset();
    m_cc->reset();
    m_bcc->reset();
    m_replyBox->clear();

    for (int i = 0; i < m_attachments.size(); i++) {
        AttachmentOpenDisplay* attachment = m_attachments[i];
        m_attachmentLayout->removeWidget(attachment);
        delete attachment;
    }
    m_attachments.clear();

    m_subject->hide();
    m_bcc->hide();
    m_cc->hide();
    m_showCC->show();
    m_showBCC->show();
    hide();
}

void EmailReplyWidget::addAttachment() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.*)"));
    QFileInfo fileInfo(filePath);
    QString fileName  = fileInfo.fileName();
    AttachmentOpenDisplay* attachment = new AttachmentOpenDisplay(filePath, fileName, this);
    m_attachments.push_back(attachment);
    m_attachmentLayout->addWidget(attachment, 0, Qt::AlignLeft);
    connect(attachment, SIGNAL(attachmentRemoved(AttachmentOpenDisplay*)), SLOT(attachmentRemoved(AttachmentOpenDisplay*)));
}

void EmailReplyWidget::attachmentRemoved(AttachmentOpenDisplay* attachment) {
    m_attachmentLayout->removeWidget(attachment);
    m_attachments.removeAll(attachment);
    delete attachment;
}

void EmailReplyWidget::send() {
    SmtpClient client("smtp.gmail.com", 465, SmtpClient::SslConnection);
    client.setUser("lucas.cmpt212@gmail.com");
    client.setPassword("1lucas23");

    MimeMessage message;
    message.setSender(new EmailAddress("lucas.cmpt212@gmail.com", "You"));

    QList<QString> to = m_to->getEmails();
    for (int i = 0; i < to.size(); i++) {
        message.addTo(new EmailAddress(to[i]));
    }

    if (to.size() == 0) {
        return;
    }

    QList<QString> cc = m_cc->getEmails();
    for (int i = 0; i < cc.size(); i++) {
        message.addCc(new EmailAddress(to[i]));
    }

    QList<QString> bcc = m_bcc->getEmails();
    for (int i = 0; i < bcc.size(); i++) {
        message.addBcc(new EmailAddress(to[i]));
    }
    QString subject;
    QString content = m_replyBox->toPlainText();
    if (m_currentMode == REPLY) {
        subject = "RE: ";
        subject.append(m_currentEmail->getSubject());

        QString replyContent;
        replyContent.append("\r\n\r\n\r\n");
        replyContent.append("--------- Reply Message ---------\r\n");
        replyContent.append(getOriginalEmailInfo());

        content.append(replyContent);

    } else if (m_currentMode == FORWARD) {
        subject = "FWD: ";
        subject.append(m_currentEmail->getSubject());

        QString forwardContent;
        forwardContent.append("--------- Forward Message ---------\r\n");
        forwardContent.append(getOriginalEmailInfo());
        content.append(forwardContent);
    } else if (m_currentMode == COMPOSE) {
        subject = m_subject->text();
    }

    message.setSubject(subject);
    MimeText text;
    text.setText(content);
    message.addPart(&text);

    QList<MimeAttachment*> attachments;
    for (int i = 0; i < m_attachments.size(); i++ ) {
        QString filePath = m_attachments[i]->getFilePath();
        QFile* file = new QFile(filePath);
        MimeAttachment* attachment = new MimeAttachment(file);
        attachments.push_back(attachment);
        message.addPart(attachment);
    }

    client.connect_and_send(message);

    for (int i = 0; i < attachments.size(); i++) {
        delete attachments[i];
    }
    attachments.clear();
    reset();
}

QString EmailReplyWidget::getOriginalEmailInfo() {
    QString info;
    QString from = QString("From: <%1>").arg(m_currentEmail->getFrom().getAddress());
    QString date = QString("Date: %1").arg(m_currentEmail->getDate().toString());
    QString subject = QString("Subject: %1").arg(m_currentEmail->getSubject());
    QString to = QString("To: %1").arg(m_currentEmail->getTo().getAddress());

    info = QString("%1\r\n%2\r\n%3\r\n%4\r\n\r\n\r\n").arg(from, date, subject, to);
    QString content = m_currentEmail->getContent();
    if (m_currentEmail->getContent().isEmpty()) {
        content = m_currentEmail->getHTML();
    }
    info.append(content);
    return info;
}
