#include "FolderTreeWidget.h"
#include <QHeaderView>

FolderTreeWidget::FolderTreeWidget(QWidget* parent) : QTreeWidget(parent) {
    initializeSettings();
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)) , SLOT(onItemClick(QTreeWidgetItem*)));
}

void FolderTreeWidget::onItemClick(QTreeWidgetItem *item) {
    FolderTreeItem* folderItem = static_cast<FolderTreeItem *>(item);
    IMAPFolder* folder = folderItem->getFolder();
    if (folder != NULL && !folder->hasChildren()) {
        emit onFolderClick(folderItem->getFolder());
    }
}

void FolderTreeWidget::initializeSettings() {
    this->header()->hide();
}

void FolderTreeWidget::addFolderChildren(FolderTreeItem* parent, QList<IMAPFolder*>& folders) {
    for (int i = 0; i < folders.size(); i++) {
        QString name = folders[i]->getName();
        FolderTreeItem* item = new FolderTreeItem(folders[i]);
        item->setText(0, name);
        if (parent == NULL) {
            addTopLevelItem(item);
        } else {
            parent->addChild(item);
        }
        if(item->getName().compare("Trash") == 0)
        {
            QIcon icon("://trash.png");
            item->setIcon(0, icon);
        }
        if (folders[i]->hasChildren()) {
            QList<IMAPFolder*> children = folders[i]->getChildren();
            addFolderChildren(item, children);
        }
    }
}

void FolderTreeWidget::loadFolders(IMAPClient* client) {
    m_client = client;
    FolderTreeItem* folder = new FolderTreeItem(NULL);
    folder->setText(0, m_client->getUser().getUsername());
    addTopLevelItem(folder);
    QList<IMAPFolder*> folders = m_client->getFolders();
    addFolderChildren(folder, folders);
    expandAll();
}
