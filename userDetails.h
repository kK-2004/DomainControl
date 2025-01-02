#ifndef USERDETAILS_H
#define USERDETAILS_H

#include "dataBase.h"
#include "managerwindow.h"
#include "tools.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>
#include <QPushButton>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui {
class UserDetailsWindow;
}
QT_END_NAMESPACE

class UserDetailsWindow : public QMainWindow{
    Q_OBJECT

public:
    UserDetailsWindow(dataBase &db,const QString &username,QWidget *parent = nullptr);
    void updateData();
    ~UserDetailsWindow();

private slots:
    void on_deleteButton_clicked();
    void onDetailsButtonClicked(int i);

private:
    Ui::UserDetailsWindow *ui;
    dataBase &db;
    QString username;
    QVector<QString>domains;
    void closeEvent(QCloseEvent *event);
    ManagerWindow *managerwindow;
    void setcolumnWidth();
};


#endif // USERDETAILS_H
