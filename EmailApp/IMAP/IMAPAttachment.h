#ifndef IMAPATTACHMENT_H
#define IMAPATTACHMENT_H

#include <QString>
#include <QByteArray>
#include <QFile>

class IMAPAttachment {
public:
    IMAPAttachment(QString filename, QByteArray data);
    virtual ~IMAPAttachment();
    virtual QString getFileName(); // Returns the filename of the attachment
    virtual void open(); // Opens the file in the default program
    virtual void saveToDisk(); // Saves the file to disk
private:
    QByteArray m_data;
    QString m_filename;
    QString m_filepath;
    bool m_saved;

};

#endif // IMAPATTACHMENT_H
