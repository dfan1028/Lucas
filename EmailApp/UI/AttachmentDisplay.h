#ifndef ATTACHMENTDISPLAY_H
#define ATTACHMENTDISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class AttachmentDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit AttachmentDisplay(QString filePath, QString fileName, QWidget *parent = 0);

    QString getFilePath();
    
signals:
    void attachmentRemoved(AttachmentDisplay* attachment);
    
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

#endif // ATTACHMENTDISPLAY_H
