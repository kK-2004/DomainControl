// #include "domainDetails.h"
// #include "insertDomain.h"
// #include "domainOverview.h"
// #include "ui_categoryDetails.h"
// #include "deleteDomain.h"
// #include <QMessageBox>
// #include <QSqlQuery>
// #include <QSqlError>
// #include <QRegularExpression>
// #include "saveDomain.h"

// extern Node* root;
// categoryDetailsWindow::categoryDetailsWindow(dataBase &db,const QString &username,const QString &domain,QWidget *parent)
//     :QMainWindow(parent), ui(new Ui::categoryDetailsWindow), db(db),username(username),domain(domain){
//     ui->setupUi(this);

//     // //如果username不是"manager":
//     // if(strcmp(username.toLocal8Bit().constData(),"manager")!=0){
//     //     if(db.getinfo(username,"identity")=="用户"){
//     //         this->window=dynamic_cast<UserWindow *>(parent);
//     //     }else if(db.getinfo(username,"identity")=="管理员"){
//     //         qDebug()<<"this";
//     //         this->window=dynamic_cast<UserDetailsWindow *>(parent);
//     //     }
//     // }else{
//     //     this->window=dynamic_cast<domainOverviewWindow *>(parent);
//     // }
//     // ui->domainShow->setReadOnly(true);
//     // ui->domainShow->setFocusPolicy(Qt::NoFocus);
//     // setFixedSize(width(), height());
//     // ui->confirmEdit->setPlaceholderText("为保护域名安全，请在这里输入你要删除的域名！");
//     // ui->domainShow->setText(domain);
// }
// categoryDetailsWindow::~categoryDetailsWindow(){
// };

// void categoryDetailsWindow::on_deleteButton_clicked()
// {
//     QString confirmInput=ui->confirmEdit->text();
//     if(confirmInput!=""){
//         //判断是否相等
//         if(confirmInput==domain){
//             //执行删除
//             if(deleteDomain(root,domain.toLocal8Bit().constData())){
//                 saveTreeToFile(root,"tree.txt");
//                 QMessageBox::information(this,"SUCCESS","删除成功！");
//                 this->close();
//             }else{
//                 QMessageBox::information(this,"fail","失败！");
//             }
//         }else{
//             QMessageBox::warning(this,"Warning","要删除的域名不匹配！");
//         }
//     }else{
//         QMessageBox::warning(this,"Warning","请输入域名以确认删除！");
//     }
// }


// void categoryDetailsWindow::on_insertButton_clicked()
// {
//     QString insertDomain=ui->insertInput->text();
//     QRegularExpression main("^[\\w]+$");
//     if(main.match(insertDomain).hasMatch()){
//         QRegularExpression regex("[\\w]+");
//         // 使用 globalMatch 获取所有匹配
//         QRegularExpressionMatchIterator it = regex.globalMatch(domain);
//         int depth = 0; // 计数器
//         QString firstdomain;
//         while (it.hasNext()) {
//             QRegularExpressionMatch match = it.next();
//             if(depth==0){
//                 firstdomain=match.captured(0);
//             }
//             depth++;
//         }
//         if(insertNode(root,depth,firstdomain,insertDomain,username)){
//             saveTreeToFile(root,"tree.txt");
//             QMessageBox::information(this,"SUCCESS","插入成功！");
//             this->close();
//         }else{
//             QMessageBox::warning(this,"Warning","插入失败！");
//         }
//     }else{
//         QMessageBox::warning(this,"Warning","非法输入，一次只能插入一个域名");
//     }

// }


// void categoryDetailsWindow::closeEvent(QCloseEvent *event)
// {

//     if (qobject_cast<UserWindow*>(window)) {
//         UserWindow *userWindow = dynamic_cast<UserWindow *>(window);
//         userWindow->updateData();
//     } else if (qobject_cast<UserDetailsWindow*>(window)) {
//         UserDetailsWindow *userdetailsWindow = dynamic_cast<UserDetailsWindow *>(window);
//         userdetailsWindow->updateData();
//     } else if (qobject_cast<domainOverviewWindow*>(window)){
//         domainOverviewWindow *domainoverviewWindow = dynamic_cast<domainOverviewWindow *>(window);
//         domainoverviewWindow ->updateData();
//     }
//     event->accept();  // 确保关闭事件被接受
// }
