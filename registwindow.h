#ifndef REGISTWINDOW_H
#define REGISTWINDOW_H

#include "dataBase.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>

QT_BEGIN_NAMESPACE
namespace Ui {
class RegistWindow;
}
QT_END_NAMESPACE

class RegistWindow : public QMainWindow {
    Q_OBJECT

public:
    RegistWindow(dataBase& db,QWidget *parent = nullptr);
    ~RegistWindow();

private slots:
    void on_registerButton_clicked();

private:
    Ui::RegistWindow *ui;
    dataBase &db;
};

#endif // REGISTWINDOW_H
