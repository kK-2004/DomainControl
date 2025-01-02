#include "domainOverview.h"
#include "ui_domainOverview.h"
#include "userdomainGet.h"
#include "domainDetails.h"
#include "categoryCount.h"

extern Node* root;
domainOverviewWindow::domainOverviewWindow(dataBase &db,QWidget *parent)
    :QMainWindow(parent), ui(new Ui::domainOverviewWindow), db(db){
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    ui->domainNumShow->setEnabled(false);
    updateData();
}

domainOverviewWindow::~domainOverviewWindow(){}


void domainOverviewWindow::updateData(){
    ui->tableWidget->clear();
    setcolumnWidth();
    char path[256] = "";
    char allvalidPaths[MAX_PATHS][MAX_PATH_LEN];  // 存储路径
    int allvalidPathCount = 0;
    printSubtreeUser(root, path, 0, "manager", allvalidPaths, allvalidPathCount);
    ui->domainNumShow->setText(QString::number(allvalidPathCount));
    //存到类成员
    for (int i = 0; i < allvalidPathCount; i++) {
        char temp[MAX_PATH_LEN];
        // 交换 validPaths[i] 和 validPaths[validPathCount - i - 1]
        std::strcpy(temp, allvalidPaths[i]);
        std::strcpy(allvalidPaths[i], allvalidPaths[allvalidPathCount - i - 1]);
        std::strcpy(allvalidPaths[allvalidPathCount - i - 1], temp);
    }
    domains.clear();
    for(int i=0;i<allvalidPathCount;i++){
        domains.append(QString::fromLocal8Bit(allvalidPaths[i]));
    }
    if(ui->searchChoose->currentText()=="总览"){
        ui->frame_3->setEnabled(false);
        ui->frame_3->setVisible(false);

        ui->tableWidget->setVisible(true);
        ui->tableWidget->setEnabled(true);

        if(allvalidPathCount){
            // 设置表格的行数和列数
            ui->tableWidget->setRowCount(allvalidPathCount);
            ui->tableWidget->setColumnCount(3);  // 3 列 (序号, 域名,查看按钮)
            //禁止水平滚动
            ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            // 禁止调整列宽
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
            // 设置表头
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "序号" << "域名"<<"操作");
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
            for (int i = 0; i < allvalidPathCount; i++) {
                QTableWidgetItem *idxItem = new QTableWidgetItem(QString::number(i+1));  // 序号
                QTableWidgetItem *domainItem = new QTableWidgetItem(allvalidPaths[i]);  // 域名
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
                domainItem->setFlags(domainItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
                // 设置文字居中
                idxItem->setTextAlignment(Qt::AlignCenter);
                domainItem->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i, 0, idxItem);  // 设置序号列
                ui->tableWidget->setItem(i, 1, domainItem);  // 设置用户名列
                ui->tableWidget->setCellWidget(i, 2, detailsButton);// 设置处理列
                // 调整列宽以适应内容
                setcolumnWidth();
                // 将每个按钮与槽函数连接，并传递行号
                connect(detailsButton, &QPushButton::clicked, this, [=]() {
                    onDetailsButtonClicked(i);
                });
            }
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
            QTableWidgetItem *infoItem = new QTableWidgetItem("还没有属于用户注册域名哦~");
            // 设置单元格不可编辑
            infoItem->setFlags(infoItem->flags() & ~Qt::ItemIsEditable);  // 设置不可编辑
            // 设置文字居中
            infoItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(0, 0, infoItem);
            ui->tableWidget->setColumnWidth(0,800);
        }
    }else{
        int res[15]={0};
        categoryCount(root, res);
        categoryCount2(root,res);
        // 隐藏表格控件
        ui->tableWidget->setVisible(false);
        ui->tableWidget->setEnabled(false);

        ui->frame_3->setEnabled(true);
        ui->frame_3->setVisible(true);

        // 显示 15 个按钮
        QGridLayout* layout = new QGridLayout();  // 创建网格布局
        layout->setHorizontalSpacing(10);  // 设置按钮之间的水平间距
        layout->setVerticalSpacing(10);    // 设置按钮之间的垂直间距

        for (int i = 0; i < 15; ++i) {
            QPushButton* button = new QPushButton(QString("%1: %2").arg(categoryToString(static_cast<category>(i))).arg(res[i]));
            button->setStyleSheet(
                "QPushButton {"
                "    background: transparent;"  // 背景透明
                "    color: black;"             // 字体颜色黑色
                "    border: none;"             // 可选：移除按钮边框
                "    border-radius: 0px;"
                "    font-size:12px;"
                "} "
                "QPushButton:hover {"
                "    background: rgba(0, 0, 0, 0.1);" // 鼠标悬停时稍微显示一个半透明背景
                "} "
                );

            // 按钮按每行5个排列
            int row = i / 5;  // 行号
            int col = i % 5;  // 列号
            layout->addWidget(button, row, col);  // 在网格布局中添加按钮

            // 连接信号槽（这里示例连接到一个通用槽函数，可以根据需要修改）
            connect(button, &QPushButton::clicked, this, [=]() {
                onCategoryButtonClicked(i, res[i]);  // 假设你有这个槽函数来处理按钮点击
            });
        }
        ui->frame_3->setLayout(layout);  // 将网格布局设置到 QFrame 上
    }


}

void domainOverviewWindow::setcolumnWidth(){
    int width=ui->tableWidget->width();
    int proportions[]={20,60,20};
    for(int i=0;i<3;i++){
        int colWidth=width*proportions[i]/100;
        ui->tableWidget->setColumnWidth(i,colWidth);
    }
}

void domainOverviewWindow::onDetailsButtonClicked(int i)
{
    domainDetailsWindow* newWindow=new domainDetailsWindow(db,"manager",domains[i],this);
    newWindow->show();
}

void domainOverviewWindow::on_searchChoose_activated(int index)
{
    updateData();
}

void domainOverviewWindow::onCategoryButtonClicked(int i, int res){
    qDebug()<<i<<res;
}
