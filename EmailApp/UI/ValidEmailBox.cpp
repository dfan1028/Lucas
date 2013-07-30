#include "ValidEmailBox.h"
#include <QHBoxLayout>

ValidEmailBox::ValidEmailBox(QString email, QWidget *parent) :
    QFrame(parent)
{
    m_emailText = email;
    createUI();
    setConnections();
}

void ValidEmailBox::createUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    m_email = new QLabel(m_emailText, this);
    m_close = new QPushButton("X", this);

    mainLayout->addWidget(m_email);
    mainLayout->addWidget(m_close);
}

void ValidEmailBox::setConnections() {
    connect(m_close, SIGNAL(clicked()), SLOT(closeBox()) );
}


void ValidEmailBox::closeBox() {
    emit boxClosed(this);
}

QString ValidEmailBox::getEmail() {
    return m_emailText;
}
