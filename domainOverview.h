#ifndef DOMAINOVERVIEW_H
#define DOMAINOVERVIEW_H

#include "dataBase.h"
#include "managerwindow.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QCryptographicHash>
#include <QPushButton>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class domainOverviewWindow;
}
QT_END_NAMESPACE

class domainOverviewWindow : public QMainWindow{
    Q_OBJECT

public:
    domainOverviewWindow(dataBase &db,QWidget *parent = nullptr);
    void updateData();
    ~domainOverviewWindow();
private slots:
    void onDetailsButtonClicked(int i);

    void on_searchChoose_activated(int index);

    void onCategoryButtonClicked(int i, int res);
private:
    Ui::domainOverviewWindow *ui;
    dataBase &db;
    void setcolumnWidth();
    QVector<QString>domains;
};
#endif // DOMAINOVERVIEW_H
