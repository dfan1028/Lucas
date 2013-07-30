#ifndef MIMEPARSER_H
#define MIMEPARSER_H

#include <QByteArray>
#include <QString>
#include <QTextStream>
#include <QList>
#include "IMAPAttachment.h"
#include "QuotedPrintable.h"

struct MIMEHeader {
    MIMEHeader() {
        contentType = "text";
        isText = false;
        isHTML = false;
        isAttachment = false;
        isMultiPart = false;
        isEnd = false;
    }

    bool isText;
    bool isAttachment;
    bool isMultiPart;
    bool isEnd;
    bool isHTML;
    QString contentType;
    QString fileName;
    QString boundary;
    QString charSet;
    QString encoding;
};

// Parses MIME data sent by the IMAP server
class MIMEParser {
    
public:
    MIMEParser(QByteArray data);
    QString getText();
    QString getHTML();
    QList<IMAPAttachment*> getAttachments();
    void parseMime();
    void parseText(QString contentType, QString contentEncoding);

private:
    void parse(QTextStream& stream, QString endBoundary);
    MIMEHeader readHeader(QTextStream& stream, QString endBoundary);
    void readContentType(MIMEHeader& header, QString line);
    void readContentDisposition(MIMEHeader& header, QString line);
    void readEncoding(MIMEHeader& header, QString line);
    QString decode(MIMEHeader& header, QString string);
    QString removeEqualSize(QString line);

    QList<IMAPAttachment*> m_attachments;
    QString m_content;
    QString m_html;
    QByteArray m_data;
};

#endif // MIMEPARSER_H
