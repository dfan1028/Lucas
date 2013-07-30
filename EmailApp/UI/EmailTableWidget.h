#ifndef EMAILLISTWIDGET_H
#define EMAILLISTWIDGET_H

#include <QTableWidget>
#include "EmailTableItem.h"
#include "IMAP/IMAPFolder.h"

class EmailTableWidget : public QTableWidget {

Q_OBJECT

public:
    EmailTableWidget(QWidget *parent);

signals:
    void onEmailClick(IMAPEmail* folder);
    void emailPageSelected(int start, int end, int total);

public slots:
    void onItemClick(int row);
    void folderSelect(IMAPFolder* folder);
    void nextPage();
    void prevPage();
    void search(QString query);
    void reload();

private:
    const static int numberOfEmailsDisplayed = 25;
    int m_currentPage;
    IMAPFolder* m_currentFolder;
    bool m_isSearching;
    QString m_currentQuery;

    void initializeSettings();
    void createLabels();
    void setEmails(QList<IMAPEmail*> emails, int total);
    void setPage(int page);
};

#endif // EMAILLISTWIDGET_H
