#include <QApplication>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QIcon>
#include "mainwindow.h"
#include "dataBase.h"
#include "headers.h"
#include "tools.h"

Node* root=new Node("",0);
int main(int argc, char *argv[]) {

    loadTreeFromFile(root,"tree.txt");
    // fileinitial();
    QApplication app(argc, argv);
    dataBase db;
    MainWindow window(db,nullptr);
    window.setWindowIcon(QIcon(":/images/logo.ico"));
    window.show();
    return app.exec();
}
