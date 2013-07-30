#include "EmailTableItem.h"

EmailTableItem::EmailTableItem(IMAPEmail* email) {
    m_email = email;
}

EmailTableItem::~EmailTableItem() {
    if (m_email != NULL) {
        delete m_email;
        m_email = NULL;
    }
}

IMAPEmail* EmailTableItem::getEmail() {
    return m_email;
}
