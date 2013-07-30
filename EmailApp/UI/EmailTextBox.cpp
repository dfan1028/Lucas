#include "EmailTextBox.h"

EmailTextBox::EmailTextBox(QString type, QWidget *parent) :
    QWidget(parent)
{
    m_type = type;
    createUI();
    setConnections();
}

void EmailTextBox::createUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    m_left = new QHBoxLayout();
    m_right = new QHBoxLayout();
    mainLayout->addLayout(m_left, 1);
    mainLayout->addLayout(m_right, 10);

    m_label = new QLabel(m_type, this);
    m_left->addWidget(m_label);

    m_emailInput = new QLineEdit(this);
    m_right->addWidget(m_emailInput);
}

void EmailTextBox::inputEmailTextChanged(QString text) {
    if (text.endsWith(";") || text.endsWith(",")) {
        text.remove(text.length() - 1, 1);
        if (isValidEmail(text)) {
            addEmail(text);
        }
    }
}

void EmailTextBox::addEmail(QString email) {
    m_emailList.push_back(email);
    ValidEmailBox* box = new ValidEmailBox(email, this);
    connect(box, SIGNAL(boxClosed(ValidEmailBox*)), SLOT(emailRemoved(ValidEmailBox*)));
    m_left->addWidget(box);
    m_emailBoxes.push_back(box);
    m_emailInput->clear();
}

void EmailTextBox::setConnections() {
    connect(m_emailInput, SIGNAL(textChanged(QString)), SLOT(inputEmailTextChanged(QString)));
}

bool EmailTextBox::isValidEmail(QString input) {
    QRegExp regex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    regex.setCaseSensitivity(Qt::CaseInsensitive);
    regex.setPatternSyntax(QRegExp::RegExp);
    return regex.exactMatch(input);
}

void EmailTextBox::emailRemoved(ValidEmailBox* box) {
    m_left->removeWidget(box);
    m_emailBoxes.removeAll(box);
    delete box;
}

void EmailTextBox::reset() {
    m_emailInput->clear();
    for (int i = 0; i < m_emailBoxes.size(); i++) {
        ValidEmailBox* box = m_emailBoxes[i];
        m_left->removeWidget(box);
        delete box;
    }
    m_emailBoxes.clear();
}

QList<QString> EmailTextBox::getEmails() {
    QList<QString> emails;
    for (int i = 0; i < m_emailBoxes.size(); i++) {
        QString email = m_emailBoxes[i]->getEmail();
        emails.push_back(email);
    }
    if (isValidEmail(m_emailInput->text())) {
        emails.push_back(m_emailInput->text());
    }
    return emails;
}
