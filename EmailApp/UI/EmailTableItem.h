#ifndef EMAILTABLEITEM_H
#define EMAILTABLEITEM_H

#include <QTableWidgetItem>
#include "IMAP/IMAPEmail.h"

class EmailTableItem : public QTableWidgetItem {

public:
    EmailTableItem(IMAPEmail* email);
    virtual ~EmailTableItem();
    IMAPEmail* getEmail();
private:
    IMAPEmail* m_email;
};


#endif // EMAILTABLEITEM_H
