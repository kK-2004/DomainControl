#ifndef USERWINDOW_H
#define USERWINDOW_H

#include "dataBase.h"
#include "mainwindow.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>

QT_BEGIN_NAMESPACE
namespace Ui {
class UserWindow;
}
QT_END_NAMESPACE

class UserWindow : public QMainWindow {
    Q_OBJECT

public:
    UserWindow(dataBase& db,const QString &username,QWidget *parent = nullptr);
    void updateData();
    ~UserWindow();

private slots:
    void on_infoButton_clicked();

    void on_quitButton_clicked();

    void on_searchButton_clicked();

    void on_usernameshow_clicked();

    void on_pushButton_clicked();

    void onDetailsButtonClicked(int i);

    void on_searchButton_2_clicked();

private:
    Ui::UserWindow *ui;
    const QString username;
    dataBase &db;
    void setcolumnWidth();
    QVector<QString>domains;
};


#endif // USERWINDOW_H
