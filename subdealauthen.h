#ifndef SUBDEALAUTHEN_H
#define SUBDEALAUTHEN_H

#include "dataBase.h"
#include "dealauthen.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class SubDealAuthenWindow;
}
QT_END_NAMESPACE

class SubDealAuthenWindow : public QMainWindow{
    Q_OBJECT

public:
    SubDealAuthenWindow(dataBase &db,const UserInfo &userinfo,const QString &managername,QWidget *parent = nullptr);
    ~SubDealAuthenWindow();

private slots:
    void on_passButton_clicked();

    void on_rejectButton_clicked();

private:
    Ui::SubDealAuthenWindow *ui;
    UserInfo userinfo;
    DealAuthenWindow *dealauthenwindow;
    void showInfo();
    dataBase &db;
    QString managername;
    void closeEvent(QCloseEvent *event);
};

#endif // SUBDEALAUTHEN_H
