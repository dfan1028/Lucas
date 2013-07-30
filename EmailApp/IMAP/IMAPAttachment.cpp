#include "IMAPAttachment.h"
#include <QDesktopServices>
#include <QUrl>
#include <QStringList>

IMAPAttachment::IMAPAttachment(QString filename, QByteArray data) {
    m_filename = filename;
    m_data = data;
    m_saved = false;
}

IMAPAttachment::~IMAPAttachment() {

}

QString IMAPAttachment::getFileName() {
    return m_filename;
}

void IMAPAttachment::open() {
    if (!m_saved) {
        saveToDisk();
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_filepath));

}

void IMAPAttachment::saveToDisk() {

    QString filename = m_filename;
    bool validFileName = false;
    int i = 0;
    while (!validFileName) {
        if (i != 0) {
            filename = m_filename;
            QStringList list = filename.split(".");
            int index = list.size() > 1 ? list.size() - 2 : list.size() - 1;
            list[index].append(" (%1)");
            list[index] = list[index].arg(i);
            filename = list.join(".");
        }
        QFile file(filename);
        if (!file.exists()) {
            validFileName = true;
        }
        i++;
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(m_data);
    m_filepath = file.fileName();
    file.close();
}
