#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include "dataBase.h"
#include "mainwindow.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>

QT_BEGIN_NAMESPACE
namespace Ui {
class ManagerWindow;
}
QT_END_NAMESPACE

class ManagerWindow : public QMainWindow {
    Q_OBJECT

public:
    ManagerWindow(dataBase& db,const QString &username,QWidget *parent = nullptr);
    void on_infoButton_clicked();
    void on_quitButton_clicked();
    void on_pushButton_clicked();
    void updateData();
    ~ManagerWindow();

private slots:
    void on_adminpushButton_clicked();

    void on_admininfoButton_clicked();

    void on_adminquitButton_clicked();

    void on_dealAuthenButton_clicked();

    void onDetailsButtonClicked(int i);

    void on_allDomainButton_clicked();

private:
    Ui::ManagerWindow *ui;
    MainWindow *parent;
    const QString username;
    dataBase &db;
    QVector<UserInfo> data;
    void setcolumnWidth();
};

#endif // MANAGERWINDOW_H
