#include "AttachmentOpenDisplay.h"
#include <QHBoxLayout>

AttachmentOpenDisplay::AttachmentOpenDisplay(QString filePath, QString fileName, QWidget *parent) :
    QWidget(parent)
{
    m_filePath = filePath;
    m_fileName = fileName;
    createUI();
    setConnections();
}

void AttachmentOpenDisplay::createUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout();
    setLayout(mainLayout);

    m_fileLabel = new QLabel(m_fileName, this);
    m_close = new QPushButton("X", this);
    mainLayout->addWidget(m_fileLabel);
    mainLayout->addWidget(m_close);
}

void AttachmentOpenDisplay::setConnections() {
    connect(m_close, SIGNAL(clicked()), SLOT(attachmentClosed()));
}

void AttachmentOpenDisplay::attachmentClosed() {
    emit attachmentRemoved(this);
}

QString AttachmentOpenDisplay::getFilePath() {
    return m_filePath;
}
