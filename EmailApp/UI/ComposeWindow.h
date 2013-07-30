#ifndef COMPOSEWINDOW_H
#define COMPOSEWINDOW_H

#include <QDialog>
#include "EmailReplyWidget.h"

class ComposeWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ComposeWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    
signals:
    
public slots:
    void close();
    void display();

private:
    EmailReplyWidget* m_emailReplyWidget;

    void createUI();
    void setConnections();

    
};

#endif // COMPOSEWINDOW_H
