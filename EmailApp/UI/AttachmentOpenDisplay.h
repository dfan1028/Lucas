#ifndef ATTACHMENTOPENDISPLAY_H
#define ATTACHMENTOPENDISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class AttachmentOpenDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit AttachmentOpenDisplay(QString filePath, QString fileName, QWidget *parent = 0);

    QString getFilePath();
    
signals:
    void attachmentRemoved(AttachmentOpenDisplay* attachment);
    
public slots:
    void attachmentClosed();

private:
    QString m_filePath;
    QString m_fileName;
    QLabel* m_fileLabel;
    QPushButton* m_close;
    void createUI();
    void setConnections();
};

#endif // ATTACHMENTOPENDISPLAY_H
