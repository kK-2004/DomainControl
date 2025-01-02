#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dataBase.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(dataBase &db,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_registerButton1_clicked();
    void on_loginButton_clicked();

    void on_passwordLineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    dataBase &db;
    bool checkAndCreateUserTable();
};

#endif // MAINWINDOW_H
