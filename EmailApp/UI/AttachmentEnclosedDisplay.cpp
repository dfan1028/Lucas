#include "AttachmentEnclosedDisplay.h"
#include <QHBoxLayout>

AttachmentEnclosedDisplay::AttachmentEnclosedDisplay(IMAPAttachment* attachment, QWidget *parent) :
    QWidget(parent)
{
    m_attachment = attachment;
    createUI();
}

void AttachmentEnclosedDisplay::createUI() {
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    QString fileName = m_attachment->getFileName();
    m_label = new QPushButton(fileName, this);
    connect(m_label, SIGNAL(clicked()), SLOT(attachmentClicked()) );
    layout->addWidget(m_label);
}

void AttachmentEnclosedDisplay::attachmentClicked() {
    m_attachment->open();
}
