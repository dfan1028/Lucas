#include "AttachmentDisplay.h"
#include <QHBoxLayout>

AttachmentDisplay::AttachmentDisplay(QString filePath, QString fileName, QWidget *parent) :
    QWidget(parent)
{
    m_filePath = filePath;
    m_fileName = fileName;
    createUI();
    setConnections();
}

void AttachmentDisplay::createUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout();
    setLayout(mainLayout);

    m_fileLabel = new QLabel(m_fileName, this);
    m_close = new QPushButton("X", this);
    mainLayout->addWidget(m_fileLabel);
    mainLayout->addWidget(m_close);
}

void AttachmentDisplay::setConnections() {
    connect(m_close, SIGNAL(clicked()), SLOT(attachmentClosed()));
}

void AttachmentDisplay::attachmentClosed() {
    emit attachmentRemoved(this);
}

QString AttachmentDisplay::getFilePath() {
    return m_filePath;
}
