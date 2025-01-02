#ifndef DEALAUTHEN_H
#define DEALAUTHEN_H

#include "dataBase.h"
#include "managerwindow.h"
#include <QCloseEvent>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>
#include <QPushButton>
#include <QMetaObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class DealAuthenWindow;
}
QT_END_NAMESPACE

class DealAuthenWindow : public QMainWindow{
    Q_OBJECT

public:
    DealAuthenWindow(dataBase &db,const QString &username,QWidget *parent);
    void updateData();
    ~DealAuthenWindow();

private slots:
    void onActionButtonClicked(int i);  // 槽函数声明
private:
    Ui::DealAuthenWindow *ui;
    QVector<UserInfo> data;
    dataBase &db;
    void setcolumnWidth();
    void closeEvent(QCloseEvent *event);
    ManagerWindow *managerwindow;
    QString username;
};

#endif // DEALAUTHEN_H
