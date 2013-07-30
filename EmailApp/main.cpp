#include "IMAP/IMAPClient.h"
#include <QApplication>
#include "UI/EmailWindow.h"
#include "QuotedPrintable.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/stylesheet.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(file.readAll());
        file.close();
    }
    EmailWindow w;
    w.show();
    return a.exec();

}

