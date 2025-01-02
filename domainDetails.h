#ifndef DOMAINDETAILS_H
#define DOMAINDETAILS_H

#include "dataBase.h"
#include "userwindow.h"
#include "userDetails.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>
#include <QPushButton>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class domainDetailsWindow;
}
QT_END_NAMESPACE

class domainDetailsWindow : public QMainWindow{
    Q_OBJECT

public:
    domainDetailsWindow(dataBase &db,const QString &username,const QString &domain,QWidget *parent = nullptr);
    ~domainDetailsWindow();

private slots:
    void on_deleteButton_clicked();

    void on_insertButton_clicked();

private:
    Ui::domainDetailsWindow *ui;
    dataBase &db;
    QString username;
    QString domain;
    void closeEvent(QCloseEvent *event);
    QWidget *window;
};


#endif // DOMAINDETAILS_H
