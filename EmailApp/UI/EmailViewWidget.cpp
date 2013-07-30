#include "EmailViewWidget.h"
#include <QWebPage>
#include <QFile>
#include <QDir>
#include <QVBoxLayout>

EmailViewWidget::EmailViewWidget(QWidget* parent) : QWidget(parent) {
    createUI();
    initializeSettings();
}

void EmailViewWidget::displayEmail(IMAPEmail* email) {
    cleanDirectory();
    clearAndHide();
    QFile file("Temp/file.html");
    file.open(QFile::ReadWrite);
    QByteArray data;
    QString content = email->getHTML();
    qDebug() << "HTML TEST";
    qDebug() << content;
    if (content.isEmpty()) {
        content = email->getContent();
        m_textView->setText(content);
        m_textView->show();
    } else {
        data.append(content);
        file.write(data);
        file.close();
        QFileInfo fileInfo(file);
        QString absPath = fileInfo.absoluteFilePath();
        QUrl url = QUrl::fromLocalFile(absPath);
        m_webView->load(url);
        m_webView->show();
    }
    show();
}

void EmailViewWidget::cleanDirectory() {
    QDir dir("Temp");
    QStringList files = dir.entryList();
    for (int i = 0; i < files.size(); i++) {
        dir.remove(files[i]);
    }
}

void EmailViewWidget::initializeSettings() {
    if (!QDir("Temp").exists()) {
        QDir().mkdir("Temp");
    }
    clearAndHide();
}

void EmailViewWidget::clearAndHide() {
    hide();
    m_webView->hide();
    m_textView->hide();
}

void EmailViewWidget::createUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    m_webView = new QWebView(this);
    m_textView = new QTextEdit(this);
    m_textView->setReadOnly(true);

    mainLayout->addWidget(m_webView);
    mainLayout->addWidget(m_textView);
}
