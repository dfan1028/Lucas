#ifndef MIMEATTACHMENT_H
#define MIMEATTACHMENT_H

#include <QFile>
#include "MimePart.h"
#include "MimeFile.h"

class MimeAttachment : public MimeFile
{
    Q_OBJECT
public:

    MimeAttachment(QFile* file);
    ~MimeAttachment();
protected:

    virtual void prepare();


};

#endif // MIMEATTACHMENT_H
