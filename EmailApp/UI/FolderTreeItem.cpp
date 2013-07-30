#include "FolderTreeItem.h"

FolderTreeItem::FolderTreeItem(IMAPFolder* folder) {
    m_folder = folder;
}

FolderTreeItem::~FolderTreeItem() {
    if (m_folder != NULL) {
        delete m_folder;
        m_folder = NULL;
    }
}

IMAPFolder* FolderTreeItem::getFolder() {
    return m_folder;
}
QString FolderTreeItem::getName()
{
    return m_folder->getName();
}
