#include "IMAPParser.h"
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QTextStream>
#include <QDateTime>
#include "IMAPFolder.h"
#include "IMAPEmail.h"

IMAPParser::IMAPParser() {
}

QList<IMAPFolder*> IMAPParser::parseFolders(QString data, IMAPConnection& connection) {
    QList<IMAPFolder*> folders;
    QString pattern = "\\* LIST \\(.*\\) \".*\" \"(.*)\"";
    QRegExp re(pattern);
    QStringList lines = data.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    for(int i = 0; i < lines.size(); i++) {
        if (re.indexIn(lines[i]) != -1) {
            qDebug() << re.cap(1);
            QString folderName = re.cap(1);
            IMAPFolder* folder = new IMAPFolder(folderName, &connection);

            if (lines[i].contains("HasChildren")) {
                folder->setHasChildren(true);
            }

            folders.push_back(folder);
        }
    }
    return folders;
}

int IMAPParser::parseSelect(QString data) {
    int count = 0;
    QStringList lines = data.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    QString pattern = "\\* ([0-9]+) EXISTS";
    QRegExp re(pattern);
    for (int i = 0; i < lines.size(); i++) {
        if (re.indexIn(lines[i]) != -1) {
            count = re.cap(1).toInt();
        }
    }
    return count;
}

QList<IMAPEmail*> IMAPParser::parseEmails(QString data, IMAPConnection& connection, IMAPFolder* folder) {
    QList<IMAPEmail*> headers;
    QString pattern = "\\* ([^\\*]*)";
    QRegExp re(pattern);

    int pos = 0;
    while ((pos = re.indexIn(data, pos)) != -1 ) {
        headers.push_front(parseEmail(re.cap(1), connection, folder));
        pos += re.matchedLength();
    }
    return headers;
}

IMAPEmail* IMAPParser::parseEmail(QString data, IMAPConnection& connection, IMAPFolder* folder) {
    QStringList lines = data.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    QString id, subject, from;
    QDateTime dt;
    id = data.split(QRegExp("[^0-9]"))[0];
    bool old = false;
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].contains("\\Seen", Qt::CaseInsensitive)) {
            old = true;
        }
        if (lines[i].contains("subject:", Qt::CaseInsensitive)) {
            subject = lines[i].split(QRegExp("[:]"), QString::SkipEmptyParts)[1];
        } else if (lines[i].contains("from:", Qt::CaseInsensitive) ) {
            from = lines[i].split(QRegExp("[:]"), QString::SkipEmptyParts)[1];
        } else if (lines[i].contains("date:", Qt::CaseInsensitive) ) {
            QStringList dateStrings = lines[i].split(QRegExp("[:]"), QString::SkipEmptyParts);
            dateStrings.removeFirst();
            QString datestr = dateStrings.join(":").trimmed();
            datestr = datestr.split("-")[0].trimmed();
            dt = QDateTime::fromString(datestr, "ddd, dd MMM yyyy HH:mm:ss");
        }
    }
    IMAPEmail *email = new IMAPEmail(id, from, subject, dt, &connection);
    email->setParent(folder);
    if (old) {
        email->setRead(true);
    }

    return email;
}

QString IMAPParser::parseBody(QString data) {
    QTextStream stream(&data);
    QString header = stream.readLine();
    QString contentID = stream.readLine();
    // Find plain text content
    while (!stream.atEnd() && !stream.readLine().contains("Content-Type: text/plain"));
    while (!stream.atEnd() && stream.readLine() != "");

    // Read content
    QStringList content;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line == contentID) {
            break;
        } else {
            content << line;
        }
    }
    return content.join("");
}

QStringList IMAPParser::parseSearch(QString data) {
    QTextStream stream(&data);
    QStringList list;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.contains("* Search", Qt::CaseInsensitive)) {
            QString emailIDs = line.remove("* Search", Qt::CaseInsensitive).trimmed();
            list = emailIDs.split(" ");
            list.removeAll("");
            return list;
        }
    }
    return list;
}

EmailAddress IMAPParser::parseEmailAddress(QString data) {
    data = data.remove(">");
    QStringList list = data.split("<");
    EmailAddress address;
    if (list.size() == 1) {
        address.setAddress(list[0]);
    } else {
        address.setName(list[0]);
        address.setAddress(list[1]);
    }
    return address;
}

QList<EmailAddress> IMAPParser::parseCC(QString data) {
    QList<EmailAddress> ccList;
    QTextStream stream(&data);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.contains("cc:", Qt::CaseInsensitive)) {
            QStringList addresses = line.split(":")[1].split(",");
            for (int i = 0; i < addresses.size(); i++) {
                EmailAddress address = parseEmailAddress(addresses[i]);
                ccList.push_back(address);
            }
        }
    }
    return ccList;
}

EmailAddress IMAPParser::parseTo(QString data) {
    EmailAddress to;
    QTextStream stream(&data);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.contains("to:", Qt::CaseInsensitive)) {
            QString address = line.split(":")[1];
            to = parseEmailAddress(address);
        }
    }
    return to;
}

QString IMAPParser::parseContentType(QString data) {
    QTextStream stream(&data);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.contains("Content-Type:", Qt::CaseInsensitive)) {
            if (line.contains("html", Qt::CaseInsensitive)) {
                return "html";
            } else if (line.contains("text", Qt::CaseInsensitive)) {
                return "text";
            }
        }
    }
    return "mime";
}

QString IMAPParser::parseContentEncoding(QString data) {
    QTextStream stream(&data);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.contains("Content-Transfer-Encoding:", Qt::CaseInsensitive)) {
            if (line.contains("quoted-printable", Qt::CaseInsensitive )) {
                return "quoted-printable";
            }
        }
    }
    return "unknown";
}

bool IMAPParser::successfulConnect(QString tag, QString data) {
    QTextStream stream(&data);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        QString find = QString("%1 OK").arg(tag);
        if (line.contains(find, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}
