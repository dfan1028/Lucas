#include "MIMEParser.h"
#include "IMAPAttachment.h"
#include "QuotedPrintable.h"
#include <QRegExp>
#include <QDebug>
#include <QTextDecoder>
#include <QStringList>

MIMEParser::MIMEParser(QByteArray data) {
    m_data = data;
}

QString MIMEParser::getText() {
    return m_content;
}

QString MIMEParser::getHTML() {
    return m_html;
}

QList<IMAPAttachment*> MIMEParser::getAttachments() {
    return m_attachments;
}

void MIMEParser::parseMime() {
    QTextStream stream(m_data);
    stream.readLine();
    parse(stream, "");
}

void MIMEParser::parseText(QString contentType, QString contentEncoding) {
    QTextStream stream(m_data);
    QString text;

    stream.readLine(); // Remove first line
    while (!stream.atEnd()) {
        QString firstLine = stream.readLine();
        QString secondLine = stream.readLine();

        firstLine = removeEqualSize(firstLine);
        secondLine = removeEqualSize(secondLine);
        if (firstLine.contains(")") && secondLine.contains("OK Success")) {
                continue;
        } else {
            if (contentEncoding == "quoted-printable") {
                text.append(firstLine);
                text.append(secondLine);
            } else {
                text.append(firstLine);
                text.append("\r\n");
                text.append(secondLine);
                text.append("\r\n");
            }
        }
    }
    if (contentEncoding == "quoted-printable") {
        text = QuotedPrintable::decode(text);
    }

    if (contentType == "html") {
        m_html = text;
    } else {
        m_content = text;
    }


}

void MIMEParser::parse(QTextStream& stream, QString endBoundary) {
    QString boundary = stream.readLine().replace("--", "");
     while (!stream.atEnd()) {

         MIMEHeader header = readHeader(stream, endBoundary);
         if (header.isMultiPart) {
             parse(stream, boundary);
             continue;
         } else if (header.isEnd) {
             return;
         }

         QString content;
         QString contentLine;
         while ( !(contentLine = stream.readLine()).contains(boundary) && !stream.atEnd()) {
             contentLine = removeEqualSize(contentLine);
             content.append(contentLine);
             content.append("\r\n");
         }
         if (header.isAttachment) {
             QByteArray data;
             data.append(content);
             IMAPAttachment* attachment = new IMAPAttachment(header.fileName, QByteArray::fromBase64(data));
             m_attachments.append(attachment);
         } else if (header.isText) {
            m_content = decode(header, content);
         } else if (header.isHTML) {
             m_html = decode(header, content);
         }
     }
}

MIMEHeader MIMEParser::readHeader(QTextStream& stream, QString endBoundary) {
    MIMEHeader header;
    QString line;
    while ( !(line = stream.readLine()).isEmpty() )  {
        if (line.contains(endBoundary, Qt::CaseInsensitive) && !endBoundary.isEmpty()) {
            header.isEnd = true;
            return header;
        }
        else if (line.contains("Content-Type", Qt::CaseInsensitive)) {
            readContentType(header, line);
        } else if (line.contains("Content-Disposition")) {
            readContentDisposition(header, line);
        } else if (line.contains("Content-Transfer-Encoding")) {
            readEncoding(header, line);
        }
    }
    return header;
}

void MIMEParser::readContentType(MIMEHeader& header, QString line) {
    if (line.contains("text/plain", Qt::CaseInsensitive)) {
        header.isText = true;
    } else if (line.contains("HTML", Qt::CaseInsensitive)){
        header.isHTML = true;
    } else if (line.contains("multipart")) {
        QRegExp reg("boundary=([0-9a-fA-F]*)");
        reg.indexIn(line);
        header.boundary = reg.cap(1);
        header.isMultiPart = true;
    }
    if (line.contains("CHARSET", Qt::CaseInsensitive)) {
        QStringList list = line.split("CHARSET=", QString::KeepEmptyParts, Qt::CaseInsensitive);
        if (list.size() > 1) {
            header.charSet = list[1];
        }
    }

    if (line.contains("name", Qt::CaseInsensitive)) {
        QStringList list = line.split(";");
        for (int i = 0; i < list.size(); i++) {
            if (list[i].contains("name", Qt::CaseInsensitive)) {
                header.fileName = list[i].split("=")[1].remove("\"");
                header.isAttachment = true;
            }
        }
    }
}

void MIMEParser::readContentDisposition(MIMEHeader& header, QString line) {
    if (line.contains("attachment")) {
        header.isAttachment = true;
        QRegExp reg("filename=\"(.*)\"");
        reg.indexIn(line);
        header.fileName = reg.cap(1);
    }
}

void MIMEParser::readEncoding(MIMEHeader& header, QString line) {
    header.encoding = line.replace("Content-Transfer-Encoding:", "").trimmed();
}

QString MIMEParser::decode(MIMEHeader& header, QString string) {
    QString decodedString = string;
    if (header.encoding.contains("quoted-printable", Qt::CaseInsensitive)) {
        qDebug() << string;
        decodedString = QString::fromLatin1(QuotedPrintable::decode(decodedString));

    }
    return decodedString;
}

QString MIMEParser::removeEqualSize(QString line) {
    int size = line.size();
    if (size == 76 && line[75] == '=') {
        line = line.remove(75, 1);
    }
    return line;
}
