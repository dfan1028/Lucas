#include "NavigationWidget.h"
#include <QHBoxLayout>

NavigationWidget::NavigationWidget(QWidget *parent) :
    QFrame(parent)
{
    createUI();
}

void NavigationWidget::createUI() {
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    m_composeButton = new QPushButton("Compose", this);
    layout->addWidget(m_composeButton, 1);
    layout->addStretch(3);
    m_prevEmailPage = new QPushButton("Prev", this);
    m_nextEmailPage = new QPushButton("Next", this);
    m_pageLabel = new QLabel("0 of 0", this);
    layout->addWidget(m_prevEmailPage, 1);
    layout->addWidget(m_pageLabel);
    layout->addWidget(m_nextEmailPage, 1);
    layout->addStretch(3);

    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Search");
    layout->addWidget(m_searchBar, 3);
    setMaximumHeight(50);

    connect(m_prevEmailPage, SIGNAL(clicked()) , SIGNAL(prevClicked()) );
    connect(m_nextEmailPage, SIGNAL(clicked()) , SIGNAL(nextClicked()) );
    connect(m_searchBar, SIGNAL(returnPressed()) , SLOT(onSearchEnterPressed()) );
    connect(m_composeButton, SIGNAL(clicked()), SLOT(onComposeClick()));
}

void NavigationWidget::onSearchEnterPressed() {
    QString query = m_searchBar->text().trimmed();
    emit searchClicked(query);
}

void NavigationWidget::emailsSelected(int start, int end, int total) {
    if (start != 0 || end != 0) {
        start = start + 1;
    }
    QString text = QString("%1 - %2 of %3").arg(QString::number(start), QString::number(end), QString::number(total));
    m_pageLabel->setText(text);
}

void NavigationWidget::onComposeClick() {
    emit composeClicked();
}
