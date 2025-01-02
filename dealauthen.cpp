#include "dealauthen.h"
#include "managerwindow.h"
#include "ui_dealauthen.h"
#include "subdealauthen.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

DealAuthenWindow::DealAuthenWindow(dataBase &db,const QString &username,QWidget *parent)
    :QMainWindow(parent), ui(new Ui::DealAuthenWindow), db(db),username(username),managerwindow(qobject_cast<ManagerWindow*>(parent)){
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    setFixedSize(width(), height());
    updateData();
    setcolumnWidth();
}

DealAuthenWindow::~DealAuthenWindow(){
};

void DealAuthenWindow::setcolumnWidth(){
    int width=ui->tableWidget->width();
    int proportions[]={20,60,20};
    for(int i=0;i<3;i++){
        int colWidth=width*proportions[i]/100;
        ui->tableWidget->setColumnWidth(i,colWidth);
    }
}

void DealAuthenWindow::onActionButtonClicked(int i)
{
    SubDealAuthenWindow *subdealauthenwindow = new SubDealAuthenWindow(db,data[i],username,this);
    subdealauthenwindow->show();
}

void DealAuthenWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    managerwindow->updateData();
}

void DealAuthenWindow::updateData(){
    data=db.fetchUserInfoByCondition("authentication","2");
    setcolumnWidth();
    int size=data.size();
    // 设置表格的行数和列数
    ui->tableWidget->setRowCount(size);
    ui->tableWidget->setColumnCount(3);  // 3 列 (序号, 用户名,操作按钮)
    // 设置表头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "序号" << "用户名"<<"操作");
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);   // 禁止选中
    ui->tableWidget->setStyleSheet(
        "QTableWidget::item:selected { background-color: transparent; }"  // 禁止选中时的背景色
        "QTableWidget::item:hover { background-color: transparent; }"      // 禁止鼠标悬停时的背景色
        "QTableWidget::item:pressed { background-color: transparent; }"    // 禁止按下时的背景色
        );
    // 显示数据
    for (int i = 0; i < size; i++) {
        QTableWidgetItem *idxItem = new QTableWidgetItem(QString::number(i+1));  // 序号
        QTableWidgetItem *domainItem = new QTableWidgetItem(data[i].username);  // 用户名
        QPushButton *actionButton = new QPushButton("处理");
        // 设置单元格不可编辑
        idxItem->setFlags(idxItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
        domainItem->setFlags(domainItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
        // 设置文字居中
        idxItem->setTextAlignment(Qt::AlignCenter);
        domainItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, idxItem);  // 设置序号列
        ui->tableWidget->setItem(i, 1, domainItem);  // 设置用户名列
        ui->tableWidget->setCellWidget(i, 2, actionButton);// 设置处理列

        // 将每个按钮与槽函数连接，并传递行号
        connect(actionButton, &QPushButton::clicked, this, [=]() {
            onActionButtonClicked(i);
        });
    }
}
