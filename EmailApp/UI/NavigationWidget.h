#ifndef NAVIGATIONWIDGET_H
#define NAVIGATIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class NavigationWidget : public QWidget
{
    Q_OBJECT
public:
     NavigationWidget(QWidget *parent = 0);
    
signals:
     void nextClicked();
     void prevClicked();
     void searchClicked(QString query);
public slots:
     void emailsSelected(int start, int end, int total);
     void onSearchClick();

private:
    QPushButton* m_prevEmailPage;
    QPushButton* m_nextEmailPage;
    QLabel* m_pageLabel;
    QLineEdit* m_searchBar;
    QPushButton* m_submitSearch;
    void createUI();

    
};

#endif // NAVIGATIONWIDGET_H
