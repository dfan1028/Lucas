#ifndef EMAILVIEWWIDGET_H
#define EMAILVIEWWIDGET_H
#include <QWebView>
#include <QFrame>
#include <QTextEdit>
#include "IMAP/IMAPEmail.h"

class EmailViewWidget : public QWidget {

Q_OBJECT

public:
    EmailViewWidget(QWidget* parent);
public slots:
    void displayEmail(IMAPEmail* email);
    void clearAndHide();
private:
    void createUI();
    void initializeSettings();
    void cleanDirectory();

    QWebView* m_webView;
    QTextEdit* m_textView;
};



#endif // EMAILVIEWWIDGET_H
