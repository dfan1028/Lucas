#include "EmailTableWidget.h"
#include <QHeaderView>
EmailTableWidget::EmailTableWidget(QWidget* parent) : QTableWidget(parent) {
    m_currentFolder = NULL;
    initializeSettings();
    createLabels();
    connect(this, SIGNAL(cellClicked(int,int)), SLOT(onItemClick(int)) );
}

void EmailTableWidget::onItemClick(int row) {
    QTableWidgetItem* item = this->item(row, 0);
    EmailTableItem* emailitem = static_cast<EmailTableItem *>(item);
    emit onEmailClick(emailitem->getEmail());
}

void EmailTableWidget::folderSelect(IMAPFolder* folder) {
    m_isSearching = false;
    m_currentFolder = folder;
    m_currentPage = 0;
    setPage(m_currentPage);
}

void EmailTableWidget::initializeSettings() {
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setColumnCount(3);
    verticalHeader()->hide();
    horizontalHeader()->resizeSections(QHeaderView::Interactive);
}

void EmailTableWidget::createLabels() {
    QStringList list;
    list << "Subject" << "From" << "Date";
    setHorizontalHeaderLabels(list);
}

void EmailTableWidget::nextPage() {
    setPage(m_currentPage + 1);
}

void EmailTableWidget::prevPage() {
    setPage(m_currentPage - 1);
}

void EmailTableWidget::setPage(int page) {
    if (m_currentFolder == NULL || page < 0) {
        return;
    }

    int totalEmails = 0;
    if (m_isSearching) {
        totalEmails = m_currentFolder->searchCount(m_currentQuery);
    } else {
        totalEmails = m_currentFolder->getNumberOfEmails();
    }

    int start = page * numberOfEmailsDisplayed;
    int end = start + numberOfEmailsDisplayed;

    if (start >= totalEmails && start != 0) {
        return;
    }

    m_currentPage = page;
    QList<IMAPEmail*> emails;
    if (m_isSearching) {
        emails = m_currentFolder->search(m_currentQuery, start, end);
    } else {
        emails = m_currentFolder->getEmails(start, end);
    }
    setEmails(emails, totalEmails);
}

void EmailTableWidget::setEmails(QList<IMAPEmail*> emails, int total) {
    this->clearContents();
    this->setRowCount(emails.size());
    for (int i = 0; i < emails.size(); i++) {
        EmailTableItem* item = new EmailTableItem(emails[i]);
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        item->setText(emails[i]->getSubject());
        this->setItem(i, 0, item);

        item = new EmailTableItem(NULL);
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        item->setText(emails[i]->getFrom().getName());
        this->setItem(i, 1, item);

        item = new EmailTableItem(NULL);
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        item->setText(emails[i]->getDate().toString());
        this->setItem(i, 2, item);
    }
    horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);

    int start = m_currentPage * numberOfEmailsDisplayed;
    emit emailPageSelected(start, start + emails.size(), total);
}

void EmailTableWidget::search(QString query) {
    if (query.isEmpty()) {
        m_isSearching = false;
        m_currentPage = 0;
        setPage(m_currentPage);
    } else {
        m_isSearching = true;
        m_currentQuery = query;
        m_currentPage = 0;
        setPage(m_currentPage);
    }
}

void EmailTableWidget::reload() {
    setPage(m_currentPage);
}
