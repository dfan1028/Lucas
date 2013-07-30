#ifndef VALIDEMAILBOX_H
#define VALIDEMAILBOX_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>

class ValidEmailBox : public QFrame
{
    Q_OBJECT
public:
    explicit ValidEmailBox(QString email, QWidget *parent = 0);
    QString getEmail();
    
signals:
    void boxClosed(ValidEmailBox* box);
public slots:
    void closeBox();

private:
    QString m_emailText;
    QLabel* m_email;
    QPushButton* m_close;

    void createUI();
    void setConnections();
};

#endif // VALIDEMAILBOX_H
