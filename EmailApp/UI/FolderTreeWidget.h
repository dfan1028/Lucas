#ifndef FOLDERTREEWIDGET_H
#define FOLDERTREEWIDGET_H

#include <QTreeWidget>
#include "FolderTreeItem.h"
#include "IMAP/IMAPFolder.h"
#include "IMAP/IMAPClient.h"

class FolderTreeWidget : public QTreeWidget {

Q_OBJECT

public:
    FolderTreeWidget(QWidget *parent);

signals:
    void onFolderClick(IMAPFolder* folder);

public slots:
    void loadFolders(IMAPClient* client);
    void onItemClick(QTreeWidgetItem* item);

private:
    IMAPClient* m_client;

    void initializeSettings();
    void addFolderChildren(FolderTreeItem* parent, QList<IMAPFolder*>& folders);

};

#endif // FOLDERTREEWIDGET_H
