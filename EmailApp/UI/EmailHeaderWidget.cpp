#include "EmailHeaderWidget.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include "EmailAddress.h"
EmailHeaderWidget::EmailHeaderWidget(QWidget *parent) :
    QWidget(parent)
{
    createUI();
    setConnections();
}

void EmailHeaderWidget::setEmail(IMAPEmail* email) {
    clearAndHide();
    m_currentEmail = email;
    EmailAddress from = email->getFrom();
    EmailAddress to = email->getTo();
    QString subject = email->getSubject();
    QList<EmailAddress> cclist = email->getCC();
    QStringList cc;

    QString format = QString("%1 <%2>");
    for (int i = 0; i < cclist.size(); i++) {
        QString address = format.arg(cclist[i].getName(), cclist[i].getAddress());
        cc.push_back(address);
    }

    m_fromValue->setText(format.arg(from.getName(), from.getAddress()));
    m_subjectValue->setText(subject);
    m_toValue->setText(format.arg(to.getName(), to.getAddress()));

    if (cc.join(", ").isEmpty()) {
        m_cc->hide();
        m_ccValue->hide();
    } else {
        m_ccValue->setText(cc.join(", "));
        m_cc->show();
        m_ccValue->show();
    }
    m_date->setText(email->getDate().toString());

    QList<IMAPAttachment*> attachments = email->getAttachments();
     for (int i = 0; i < attachments.size(); i++) {
        AttachmentEnclosedDisplay* display = new AttachmentEnclosedDisplay(attachments[i], this);
        m_attachmentLayout->addWidget(display);
        m_attachments.push_back(display);
    }

    show();
}

void EmailHeaderWidget::clearAndHide() {
    m_currentEmail = NULL;
    for (int i = 0; i < m_attachments.size(); i++) {
        m_attachmentLayout->removeWidget(m_attachments[i]);
        delete m_attachments[i];
    }
    m_attachments.clear();
    hide();
}

void EmailHeaderWidget::createUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    mainLayout->addLayout(headerLayout);

    QGridLayout* left = new QGridLayout();
    m_from = new QLabel("From:", this);
    m_fromValue = new QLabel(this);
    m_subject = new QLabel("Subject:", this);
    m_subjectValue = new QLabel(this);
    m_to = new QLabel("To:", this);
    m_toValue = new QLabel(this);
    m_cc = new QLabel("Cc:", this);
    m_ccValue = new QLabel(this);

    left->addWidget(m_from, 0, 0);
    left->addWidget(m_fromValue, 0, 1);
    left->addWidget(m_subject, 1, 0);
    left->addWidget(m_subjectValue, 1, 1);
    left->addWidget(m_to, 2, 0);
    left->addWidget(m_toValue, 2, 1);
    left->addWidget(m_cc, 3, 0);
    left->addWidget(m_cc, 3, 1);


    QVBoxLayout* right = new QVBoxLayout();

    QHBoxLayout* topRight = new QHBoxLayout();
    m_reply = new QPushButton("Reply", this);
    m_replyAll = new QPushButton("Reply All", this);
    m_forward = new QPushButton("Forward", this);
    m_delete = new QPushButton("Delete", this);
    topRight->addWidget(m_reply);
    topRight->addWidget(m_replyAll);
    topRight->addWidget(m_forward);
    topRight->addWidget(m_delete);

    QHBoxLayout* bottomRight = new QHBoxLayout();
    m_date = new QLabel();
    bottomRight->addWidget(m_date, 1, Qt::AlignRight);

    right->addLayout(topRight, 1);
    right->addLayout(bottomRight, 1);

    headerLayout->addLayout(left, 1);
    headerLayout->addStretch(2);
    headerLayout->addLayout(right, 1);

    QHBoxLayout* attachmentLeft = new QHBoxLayout();
    mainLayout->addLayout(attachmentLeft);
    m_attachmentLayout = new QHBoxLayout();
    attachmentLeft->addLayout(m_attachmentLayout);
    QHBoxLayout* attachmentRight = new QHBoxLayout();
    attachmentRight->addStretch();
    attachmentLeft->addLayout(attachmentRight);

    hide();
}

void EmailHeaderWidget::setConnections() {
    connect(m_reply, SIGNAL(clicked()), SLOT(replyPressed()));
    connect(m_replyAll, SIGNAL(clicked()), SLOT(replyAllPressed()));
    connect(m_forward, SIGNAL(clicked()), SLOT(forwardPressed()));
    connect(m_delete, SIGNAL(clicked()), SLOT(deletePressed()));
}

void EmailHeaderWidget::replyPressed() {
    if (m_currentEmail != NULL) {
        emit replyClicked(m_currentEmail);
    }
}

void EmailHeaderWidget::replyAllPressed() {
    if (m_currentEmail != NULL) {
        emit replyAllClicked(m_currentEmail);
    }
}

void EmailHeaderWidget::forwardPressed() {
    if (m_currentEmail != NULL) {
        emit forwardClicked(m_currentEmail);
    }
}

void EmailHeaderWidget::deletePressed() {
    if (m_currentEmail != NULL) {
        m_currentEmail->deleteEmail();
        emit deleteClicked(m_currentEmail);
        m_currentEmail = NULL;
    }
}
