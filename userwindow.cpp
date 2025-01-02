#include "userwindow.h"
#include "ui_userwindow.h"
#include "mainwindow.h"
#include "user_updatepassword.h"
#include "authentication.h"
#include "headers.h"
#include "createTree.h"
#include "findTree.h"
#include "domainDetails.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include "tools.h"
#include "userdomainGet.h"
#include "saveDomain.h"

extern Node* root;
extern char pathSave[];
extern std::fstream file;
UserWindow::UserWindow(dataBase &db, const QString &username, QWidget *parent)
    : QMainWindow(nullptr), ui(new Ui::UserWindow), db(db), username(username) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    setcolumnWidth();
    ui->domainInput->setPlaceholderText("请输入关键域名,如neu");
    updateData();
}


void UserWindow::setcolumnWidth(){
    int width=ui->tableWidget->width();
    int proportions[]={20,60,20};
    for(int i=0;i<3;i++){
        int colWidth=width*proportions[i]/100;
        ui->tableWidget->setColumnWidth(i,colWidth);
    }
}

UserWindow::~UserWindow(){
};

void UserWindow::on_infoButton_clicked()
{
    PasswordWindow * passwordwindow =new PasswordWindow(db,username,this);
    this->hide();
    passwordwindow->show();
}


void UserWindow::on_quitButton_clicked()
{
    MainWindow *mainwindow = new MainWindow(db,nullptr);
    this->close();
    mainwindow->show();
}



//跳转实名界面
void UserWindow::on_usernameshow_clicked()
{
    AuthenticationWindow *authenticationwindow = new AuthenticationWindow(db,username,this);
    this->close();
    authenticationwindow->show();
}


void UserWindow::on_pushButton_clicked()
{
    loadTreeFromFile(root,"tree.txt");
    ui->tableWidget->clear();
    updateData();
    // UserWindow *newwindow= new UserWindow(db,username,nullptr);
    // this->close();
    // newwindow->show();
}

//创建域名
void UserWindow::on_searchButton_clicked()
{
    QString authen=db.getinfo(username,"authentication");
    if(authen!="已实名"){
        QMessageBox::warning(this,"Fail","您还未完成实名，不允许操作域名！");
    }else{
        QString domain=ui->domainInput->text()+ui->domainChoose->currentText();
        int choice=ui->categoryChoose->currentIndex();
        if(choice==0){
            QMessageBox::warning(this,"Warning","请选择类别！");
        }else{
            //判断合法
            QRegularExpression regexcreate("^[\\w]{1,63}$");
            if(regexcreate.match(ui->domainInput->text()).hasMatch()){
                //先查找域名是否存在
                if(findExactNode(root,reverseDomainParts(domain.toLocal8Bit().constData()))!=nullptr){
                    QMessageBox::warning(this,"Warning","该域名已存在！");
                }else{
                    if(createHelper(root,reverseDomainParts(domain.toLocal8Bit().constData()),0,username,choice)){
                        updateData();

                        saveTreeToFile(root, "tree.txt");

                        // qDebug()<<"testting"<<pathSave;
                        // savedomaindata(1,root,0);
                        // file.close();
                        QMessageBox::information(this,"SUCCESS!","创建成功！");
                    }else{
                        QMessageBox::warning(this,"Fail!","创建失败！");
                    }
                }
            }else{
                QMessageBox::warning(this,"Warning","输入不合法！");
            }
        }
    }
}

void UserWindow::onDetailsButtonClicked(int i)
{
    domainDetailsWindow* newWindow=new domainDetailsWindow(db,username,domains[i],this);
    newWindow->show();
}

void UserWindow::on_searchButton_2_clicked()
{
    QString domain=ui->findEdit->text();
    QRegularExpression regex("^[\\w.]+$");
    if(regex.match(domain).hasMatch()){
        if(domains.contains(domain)){
            domainDetailsWindow* newWindow=new domainDetailsWindow(db,username,domain,nullptr);
            newWindow->show();
        }else{
            QMessageBox::warning(this,"Fail","你未拥有该域名！");
        }
    }else{
        QMessageBox::warning(this,"Fail","输入不合法!");
    }
}

void UserWindow::updateData(){
    setcolumnWidth();
    ui->tableWidget->clear();
    ui->usernameshow->setText(username+"("+db.getinfo(username,"authentication")+")");  // 设置欢迎用户名
    //如果未实名点击可跳转至实名界面
    if(db.getinfo(username,"authentication")=="未实名"||db.getinfo(username,"authentication")=="审核未通过"){
        ui->usernameshow->setEnabled(true);
        ui->usernameshow->setCursor(Qt::PointingHandCursor);
        ui->usernameshow->setToolTip("点击去实名");
    }else if(db.getinfo(username,"authentication")=="已实名"||db.getinfo(username,"authentication")=="审核中"){
        ui->usernameshow->setEnabled(false);
    }

    //禁止水平滚动
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 禁止调整列宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->domainInput->clear();
    char path[256] = "";
    char validPaths[MAX_PATHS][MAX_PATH_LEN];  // 存储路径
    int validPathCount = 0;
    printSubtreeUser(root, path, 0, username.toLocal8Bit().constData(), validPaths, validPathCount);
    // char path[256] = "";
    // char *validPaths[MAX_PATHS];  // 存储路径
    // int validPathCount = 0;
    // collectFullDomainPaths(root, validPaths, validPathCount,username);
    // //// 测试数据
    // char example[3][500] = {
    //     "www.baidu.com",   // 域名 1
    //     "www.123.com",     // 域名 2
    //     "www.test.com"     // 域名 3
    // };
    //如果有数据
    for (int i = 0; i < validPathCount / 2; i++) {
        char temp[MAX_PATH_LEN];
        // 交换 validPaths[i] 和 validPaths[validPathCount - i - 1]
        std::strcpy(temp, validPaths[i]);
        std::strcpy(validPaths[i], validPaths[validPathCount - i - 1]);
        std::strcpy(validPaths[validPathCount - i - 1], temp);
    }
    int size=validPathCount;
    domains.clear();
    for(int i=0;i<validPathCount;i++){
        domains.append(QString::fromLocal8Bit(validPaths[i]));
    }
    if(size){
        ui->tableWidget->verticalHeader()->setVisible(true);  // 显示行名
        ui->tableWidget->horizontalHeader()->setVisible(true);  // 显示列名
        // int size=sizeof(example)/sizeof(example[0]);
        int size=validPathCount;
        // 设置表格的行数和列数
        ui->tableWidget->setRowCount(size);
        ui->tableWidget->setColumnCount(3);  // 3 列 (序号, 域名, 操作)
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // 选择整行
        ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);   // 禁止选中
        ui->tableWidget->verticalHeader()->setDefaultSectionSize(35); // 设置默认行高
        // 禁用选中时的背景色，包含拖动时的背景色
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
        // 设置表头
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "序号" << "域名" << "操作");
        ui->tableWidget->verticalHeader()->setVisible(false);
        // 显示数据
        for (int i = 0; i < size; i++) {
            QTableWidgetItem *idxItem = new QTableWidgetItem(QString::number(i+1));  // 序号
            QTableWidgetItem *domainItem = new QTableWidgetItem(domains[i]);  // 域名
            QPushButton *detailsButton = new QPushButton("详情"); //操作
            // 设置单元格不可编辑
            idxItem->setFlags(idxItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
            domainItem->setFlags(domainItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
            // 设置文字居中
            idxItem->setTextAlignment(Qt::AlignCenter);
            domainItem->setTextAlignment(Qt::AlignCenter);
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
            ui->tableWidget->setItem(i, 0, idxItem);  // 设置序号列
            ui->tableWidget->setItem(i, 1, domainItem);  // 设置域名列
            ui->tableWidget->setCellWidget(i, 2, detailsButton);// 设置处理列
            // 将每个按钮与槽函数连接，并传递行号
            connect(detailsButton, &QPushButton::clicked, this, [=]() {
                onDetailsButtonClicked(i);
            });
        }

        // 调整列宽以适应内容
        setcolumnWidth();
    }else{
        ui->tableWidget->verticalHeader()->setVisible(false);  // 隐藏行名
        ui->tableWidget->horizontalHeader()->setVisible(false);  // 隐藏列名
        ui->tableWidget->setRowCount(1);
        ui->tableWidget->setColumnCount(1);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // 选择整行
        ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);   // 禁止选中
        // 禁用选中时的背景色，包含拖动时的背景色
        ui->tableWidget->setStyleSheet(
            "QTableWidget::item:selected { background-color: transparent; }"  // 禁止选中时的背景色
            "QTableWidget::item:hover { background-color: transparent; }"      // 禁止鼠标悬停时的背景色
            "QTableWidget::item:pressed { background-color: transparent; }"    // 禁止按下时的背景色
            );
        QTableWidgetItem *infoItem = new QTableWidgetItem("还没有属于你的域名哦~");
        // 设置单元格不可编辑
        infoItem->setFlags(infoItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
        // 设置文字居中
        infoItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(0, 0, infoItem);
        ui->tableWidget->setColumnWidth(0,800);
    }
}

