#ifndef IMAPTAG_H
#define IMAPTAG_H

// Represents a unique number for communicating with IMAP Servers
class IMAPTag {
public:
    static QString getNextTag() {
        static int currentID = 0;
        if (currentID >= 999) {
            currentID = 0;
        } else {
            currentID++;
        }
        QString number = QString("%1").arg(currentID).rightJustified(6, '0');
        return QString("a%1").arg(number);
    }
};

#endif // IMAPTAG_H
