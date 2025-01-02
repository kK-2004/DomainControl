#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "dataBase.h"
#include "userwindow.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFileInfo>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui {
class AuthenticationWindow;
}
QT_END_NAMESPACE

class AuthenticationWindow : public QMainWindow {
    Q_OBJECT

public:
    AuthenticationWindow(dataBase& db,const QString &username,QWidget *parent = nullptr);
    ~AuthenticationWindow();

private slots:
    void on_back_4_clicked();

    void on_pushButton_pass_clicked();

    void on_postinfoButton_clicked();

    void on_pushButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::AuthenticationWindow *ui;
    const QString username;
    QWidget *parent;
    dataBase &db;
};
#endif // AUTHENTICATION_H
