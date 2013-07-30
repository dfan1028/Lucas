#ifndef EMAILTEXTBOX_H
#define EMAILTEXTBOX_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include "ValidEmailBox.h"

class EmailTextBox : public QWidget
{
    Q_OBJECT
public:
    explicit EmailTextBox(QString type, QWidget *parent = 0);
    
signals:
    
public slots:
    void inputEmailTextChanged(QString text);
    void emailRemoved(ValidEmailBox* box);
    void addEmail(QString email);
    void reset();
    QList<QString> getEmails();

private:
    QString m_type;
    QLabel* m_label;
    QLineEdit* m_emailInput;
    QHBoxLayout* m_left;
    QHBoxLayout* m_right;
    QStringList m_emailList;
    QList<ValidEmailBox*> m_emailBoxes;

    void createUI();
    void setConnections();
    bool isValidEmail(QString input);
};

#endif // EMAILTEXTBOX_H
