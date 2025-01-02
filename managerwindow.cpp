#include "managerwindow.h"
#include "ui_managerwindow.h"
#include "mainwindow.h"
#include "user_updatepassword.h"
#include "authentication.h"
#include "dealauthen.h"
#include "userdomainGet.h"
#include "userDetails.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include "domainOverview.h"

extern Node* root;
ManagerWindow::ManagerWindow(dataBase &db, const QString &username, QWidget *parent)
    : QMainWindow(nullptr), ui(new Ui::ManagerWindow), db(db), username(username) {
    this->parent=dynamic_cast<MainWindow *>(parent);
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    ui->allDomainButton->setStyleSheet("QPushButton {"
                                        "    background-color: #1f49a9;"
                                        "    color: white;"
                                        "    border: none;"
                                        "    padding: 10px 20px;"
                                        "    border-radius: 5px;"
                                        "} "
                                        "QPushButton:hover {"
                                        "    background-color: rgb(39, 86, 197);"
                                        "    color: white;"
                                        "    border: 2px solid rgb(255, 255, 0);"
                                        "}"
                                        );
    ui->usernameshow->setText("管理员："+username);  // 设置欢迎用户名
    ui->usernameshow->setEnabled(false);
    updateData();
}


ManagerWindow::~ManagerWindow(){
};


void ManagerWindow::on_adminpushButton_clicked()
{
    loadTreeFromFile(root,"tree.txt");
    // ManagerWindow *newwindow= new ManagerWindow(db,username,nullptr);
    // this->close();
    // newwindow->show();
    updateData();
}


void ManagerWindow::on_admininfoButton_clicked()
{
    PasswordWindow * passwordwindow =new PasswordWindow(db,username,this);
    this->hide();
    passwordwindow->show();
}


void ManagerWindow::on_adminquitButton_clicked()
{
    MainWindow *mainwindow = new MainWindow(db,nullptr);
    this->close();
    mainwindow->show();
}


void ManagerWindow::on_dealAuthenButton_clicked()
{
    DealAuthenWindow *dealauthenwindow=new DealAuthenWindow(db,username,this);
    dealauthenwindow->show();
}

void ManagerWindow::onDetailsButtonClicked(int i)
{
    UserDetailsWindow* newWindow=new UserDetailsWindow(db,data[i].username,this);
    newWindow->show();
}

void ManagerWindow::setcolumnWidth(){
    int width=ui->tableWidget->width()-10;
    int proportions[]={20,30,30,20};
    for(int i=0;i<4;i++){
        int colWidth=width*proportions[i]/100;
        ui->tableWidget->setColumnWidth(i,colWidth);
    }
}

void ManagerWindow::updateData(){
    ui->tableWidget->clear();
    data=db.fetchUserInfoByCondition("identity",0);
    int size=data.size();
    // 设置表格的行数和列数
    ui->tableWidget->setRowCount(size);
    ui->tableWidget->setColumnCount(4);  // 4 列 (序号, 用户名,域名数量,查看按钮)
    //禁止水平滚动
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 禁止调整列宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    // 设置表头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "序号" << "用户名"<<"域名数量"<<"操作");
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);   // 禁止选中
    ui->tableWidget->setStyleSheet(
        "QHeaderView::section {"
        "color: black;"
        "font: bold 12px Arial;"
        "border: 1px solid #000000;"
        "    text-align: center;"
        "padding: 4px;"
        "}"

        "QTableWidget::item:selected { background-color: transparent; }"  // 禁止选中时的背景色
        "QTableWidget::item:hover { background-color: transparent; }"      // 禁止鼠标悬停时的背景色
        "QTableWidget::item:pressed { background-color: transparent; }"    // 禁止按下时的背景色
        );
    // 显示数据
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40); // 设置默认行高
    for (int i = 0; i < size; i++) {
        QTableWidgetItem *idxItem = new QTableWidgetItem(QString::number(i+1));  // 序号
        QTableWidgetItem *usernameItem = new QTableWidgetItem(data[i].username);  // 用户名
        char path[256] = "";
        char validPaths[MAX_PATHS][MAX_PATH_LEN];  // 存储路径
        int domainnum=0;
        printSubtreeUser(root, path, 0, data[i].username.toLocal8Bit().constData(), validPaths, domainnum);
        QTableWidgetItem *numItem = new QTableWidgetItem(QString::number(domainnum));  //域名数量
        // char path[256] = "";
        // char *validPaths[MAX_PATHS];  // 存储路径
        // int domainnum=0;/////////////////////////////////////////////////////
        // collectFullDomainPaths(root, validPaths, domainnum,data[i].username);
        // QTableWidgetItem *numItem = new QTableWidgetItem(QString::number(domainnum));  //域名数量
        QPushButton *detailsButton = new QPushButton("查看");
        detailsButton->setStyleSheet(
            "QPushButton {"
            "    background: transparent;"  // 背景透明
            "    color: black;"             // 字体颜色黑色
            "    border: none;"             // 可选：移除按钮边框
            "    border-radius: 0px;"
            "    font-size:12px;"
            "}"
            "QPushButton:hover {"
            "    background: rgba(0, 0, 0, 0.1);" // 鼠标悬停时稍微显示一个半透明背景
            "}"
            );

        // 设置单元格不可编辑
        idxItem->setFlags(idxItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
        usernameItem->setFlags(usernameItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
        numItem->setFlags(usernameItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
        // 设置文字居中
        idxItem->setTextAlignment(Qt::AlignCenter);
        usernameItem->setTextAlignment(Qt::AlignCenter);
        numItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, idxItem);  // 设置序号列
        ui->tableWidget->setItem(i, 1, usernameItem);  // 设置用户名列
        ui->tableWidget->setItem(i, 2, numItem);  // 设置域名数量列
        ui->tableWidget->setCellWidget(i, 3, detailsButton);// 设置处理列
        // 调整列宽以适应内容
        setcolumnWidth();
        // 将每个按钮与槽函数连接，并传递行号
        connect(detailsButton, &QPushButton::clicked, this, [=]() {
            onDetailsButtonClicked(i);
        });
    }
    QString authen_num = db.getNum("authentication","2");
    if(authen_num!="0"){
        ui->dealAuthenButton->setStyleSheet("QPushButton {"
                                            "    background-color: rgb(169, 31, 56);"
                                            "    color: white;"
                                            "    border: none;"
                                            "    padding: 10px 20px;"
                                            "    border-radius: 5px;"
                                            "} "
                                            "QPushButton:hover {"
                                            "    background-color: rgb(255, 0, 0);"
                                            "    color: yellow;"
                                            "    border: 2px solid rgb(255, 255, 0);"
                                            "}"
                                            );
        ui->dealAuthenButton->setText(QString("实名审核")+"("+authen_num+")");
    }else{
        ui->dealAuthenButton->setStyleSheet("QPushButton {"
                                            "    background-color: #1f49a9;"
                                            "    color: white;"
                                            "    border: none;"
                                            "    padding: 10px 20px;"
                                            "    border-radius: 5px;"
                                            "} "
                                            "QPushButton:hover {"
                                            "    background-color: rgb(39, 86, 197);"
                                            "    color: white;"
                                            "    border: 2px solid rgb(255, 255, 0);"
                                            "}"
                                            );
        ui->dealAuthenButton->setText(QString("实名审核"));
    }
}

void ManagerWindow::on_allDomainButton_clicked()
{
    domainOverviewWindow *window = new domainOverviewWindow(db,this);
    window->show();
}

