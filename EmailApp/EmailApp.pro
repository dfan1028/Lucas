#-------------------------------------------------
#
# Project created by QtCreator 2013-06-25T15:05:19
#
#-------------------------------------------------

QT       += core gui network webkit webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = EmailApp
TEMPLATE = app

INCLUDEPATH += $$PWD

SOURCES += main.cpp \
    IMAP/IMAPParser.cpp \
    IMAP/IMAPConnection.cpp \
    IMAP/IMAPClient.cpp \
    SMTP/SMTPClient.cpp \
    IMAP/IMAPFolder.cpp \
    IMAP/IMAPEmail.cpp \
    SMTP/MimeText.cpp \
    SMTP/MimePart.cpp \
    SMTP/mimemultipart.cpp \
    SMTP/MimeMessage.cpp \
    SMTP/MimeInlinefile.cpp \
    SMTP/MimeHtml.cpp \
    SMTP/MimeFile.cpp \
    SMTP/mimecontentformatter.cpp \
    SMTP/MimeAttachment.cpp \
    UI/NewMessage.cpp \
    UI/ReplyMessage.cpp \
    UI/ForwardMessage.cpp \
    UI/TableModel.cpp \
    IMAP/MIMEParser.cpp \
    IMAP/IMAPAttachment.cpp \
    EmailAddress.cpp \
    User.cpp \
    UI/EmailWindow.cpp \
    UI/FolderTreeWidget.cpp \
    UI/FolderTreeItem.cpp \
    UI/EmailTableItem.cpp \
    UI/EmailTableWidget.cpp \
    UI/EmailViewWidget.cpp \
    quotedprintable.cpp \
    UI/NavigationWidget.cpp \
    UI/EmailHeaderWidget.cpp \
    UI/EmailReplyWidget.cpp \
    UI/EmailTextBox.cpp \
    UI/ValidEmailBox.cpp \
    UI/AttachmentOpenDisplay.cpp \
    UI/ClickableLabel.cpp \
    UI/AttachmentEnclosedDisplay.cpp \
    UI/ComposeWindow.cpp \
    UI/LoginWindow.cpp \
    EmailProviders.cpp

HEADERS  += \
    IMAP/IMAPTag.h \
    IMAP/IMAPParser.h \
    IMAP/IMAPConnection.h \
    IMAP/IMAPClient.h \
    SMTP/SMTPClient.h \
    IMAP/IMAPFolder.h \
    IMAP/IMAPEmail.h \
    SMTP/SmtpMime \
    SMTP/MimeText.h \
    SMTP/MimePart.h \
    SMTP/mimemultipart.h \
    SMTP/MimeMessage.h \
    SMTP/MimeInlinefile.h \
    SMTP/MimeHtml.h \
    SMTP/MimeFile.h \
    SMTP/mimecontentformatter.h \
    SMTP/MimeAttachment.h \
    UI/NewMessage.h \
    UI/ReplyMessage.h \
    UI/ForwardMessage.h \
    UI/TableModel.h \
    IMAP/MIMEParser.h \
    IMAP/IMAPAttachment.h \
    EmailAddress.h \
    EmailProviders.h \
    User.h \
    UI/EmailViewWidget.h \
    UI/EmailWindow.h \
    UI/FolderTreeItem.h \
    UI/FolderTreeWidget.h \
    UI/EmailTableItem.h \
    UI/EmailTableWidget.h \
    QuotedPrintable.h \
    UI/NavigationWidget.h \
    UI/EmailHeaderWidget.h \
    UI/EmailReplyWidget.h \
    UI/EmailTextBox.h \
    UI/ValidEmailBox.h \
    UI/AttachmentOpenDisplay.h \
    UI/ClickableLabel.h \
    UI/AttachmentEnclosedDisplay.h \
    UI/ComposeWindow.h \
    UI/LoginWindow.h

FORMS    +=

OTHER_FILES += \
    SMTP/README.txt \
    stylesheet.qss

RESOURCES += \
    attachmenticon.qrc \
    trashicon.qrc
