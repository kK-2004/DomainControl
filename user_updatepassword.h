#ifndef USER_UPDATEPASSWORD_H
#define USER_UPDATEPASSWORD_H

#include "dataBase.h"
#include "userwindow.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>

QT_BEGIN_NAMESPACE
namespace Ui {
class PasswordWindow;
}
QT_END_NAMESPACE

class PasswordWindow : public QMainWindow {
    Q_OBJECT

public:
    PasswordWindow(dataBase& db,const QString &username,QWidget *parent = nullptr);
    ~PasswordWindow();

private slots:
    void on_back_clicked();

    void on_submitButton_clicked();

    void on_quitButton_clicked();

    void on_pushButton_authen_clicked();

private:
    Ui::PasswordWindow *ui;
    QString username;
    QWidget *parent;
    dataBase &db;
};


#endif // USER_UPDATEPASSWORD_H
