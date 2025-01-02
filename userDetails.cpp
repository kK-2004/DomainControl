#include "userDetails.h"
#include "ui_userDetails.h"
#include "deleteDomain.h"
#include "userdomainGet.h"
#include "domainDetails.h"

extern Node* root;
UserDetailsWindow::UserDetailsWindow(dataBase &db,const QString &username,QWidget *parent)
    :QMainWindow(parent), ui(new Ui::UserDetailsWindow), db(db),username(username),managerwindow(qobject_cast<ManagerWindow*>(parent)){
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    setcolumnWidth();
    ui->userShow->setText(username);
    ui->userShow->setEnabled(false);
    updateData();
}
UserDetailsWindow::~UserDetailsWindow(){
};

void UserDetailsWindow::on_deleteButton_clicked()
{
    QMessageBox *box = new QMessageBox(QMessageBox::Question, "提示", "确认删除该用户吗？", QMessageBox::Yes | QMessageBox::No, this);
    box->button(QMessageBox::Yes)->setText("确认");
    box->button(QMessageBox::No)->setText("取消");
    int res = box->exec();
    if(QMessageBox::Yes == res){
        if(db.deleteUser(username)){
            QMessageBox::information(this,"SUCEESS","删除成功！");
            this->close();
        }else{
            QMessageBox::warning(this,"Fail","删除失败！");
        }
    }
}

void UserDetailsWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    managerwindow->updateData();
}

void UserDetailsWindow::updateData(){
    ui->tableWidget->clear();
    setcolumnWidth();
    //禁止水平滚动
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 禁止调整列宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    char path[256] = "";
    char validPaths[MAX_PATHS][MAX_PATH_LEN];  // 存储路径
    int validPathCount = 0;
    printSubtreeUser(root, path, 0, username.toLocal8Bit().constData(), validPaths, validPathCount);
    // char path[256] = "";
    // char *validPaths[MAX_PATHS];  // 存储路径
    // int validPathCount = 0;
    // collectFullDomainPaths(root, validPaths, validPathCount,username);
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

void UserDetailsWindow::onDetailsButtonClicked(int i)
{
    domainDetailsWindow* newWindow=new domainDetailsWindow(db,username,domains[i],this);
    newWindow->show();
}

void UserDetailsWindow::setcolumnWidth(){
    int width=ui->tableWidget->width();
    int proportions[]={20,60,20};
    for(int i=0;i<3;i++){
        int colWidth=width*proportions[i]/100;
        ui->tableWidget->setColumnWidth(i,colWidth);
    }
}

