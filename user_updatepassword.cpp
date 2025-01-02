#include "user_updatepassword.h"
#include "managerwindow.h"
#include "authentication.h"
#include "ui_user_updatepassword.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

PasswordWindow::PasswordWindow(dataBase &db,const QString &username,QWidget *parent)
    :QMainWindow(nullptr), ui(new Ui::PasswordWindow), db(db),username(username){
    if(db.getinfo(username,"identity")=="用户"){
        this->parent=dynamic_cast<UserWindow *>(parent);
    }else if(db.getinfo(username,"identity")=="管理员"){
        this->parent=dynamic_cast<ManagerWindow *>(parent);
    }
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    this->setFixedSize(1080, 720);
    ui->frame_4->setFrameShape(QFrame::Box);
    ui->frame_4->setFrameShadow(QFrame::Raised);
    ui->pushButton_pass->setEnabled(false);
    ui->pushButton_authen->setCursor(Qt::PointingHandCursor);
}

PasswordWindow::~PasswordWindow(){}


void PasswordWindow::on_back_clicked()
{
    this->close();
    parent->show();
}


void PasswordWindow::on_submitButton_clicked()
{
    QString oriword=ui->oriWord->text();
    QString newword=ui->newWord->text();
    QString newwordConfirm=ui->newWordConfirm->text();

    //验证原密码是否正确
    QString hashedOriword=QString(QCryptographicHash::hash(oriword.toUtf8(), QCryptographicHash::Sha256).toHex());
    if (oriword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "请输入用户名！");
        return;
    }
    if (newword.isEmpty()){
        QMessageBox::warning(this,"Input Error","密码不能为空！");
        return;
    }
    if (newwordConfirm.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "请确认密码！");
        return;
    }
    if (newword != newwordConfirm) {
        QMessageBox::warning(this, "Input Error", "两次输入的密码不一致！");
        return;
    }

    if(db.userExists(username)){
        // 用户存在, 继续验证密码
        if(db.validateUser(username, hashedOriword)){
            // 正则匹配
            // 密码规则:至少含有1个大写字母、数字、小写字母；8-16位;不能有特殊符号(!@#$%^&*)
            QRegularExpression regex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?!.*[!@#$%^&*])[a-zA-Z\\d]{8,16}$");
            if(!regex.match(newword).hasMatch()) {
                QMessageBox::warning(this, "密码不合法！", "密码必须包含至少一个大写字母、一个小写字母和一个数字，长度为8到16个字符，不能包含特殊字符（!@#$%^&*）");
                return;
            }
            // 对密码进行哈希
            QString hashedNewPassword = QString(QCryptographicHash::hash(newword.toUtf8(), QCryptographicHash::Sha256).toHex());
            if(db.UserChangePassword(username,hashedNewPassword)){
                QMessageBox::information(this, "Success", "修改成功!请重新登录");
                MainWindow *mainwindow=new MainWindow(db,nullptr);
                this->close();
                mainwindow->show();
            }else{
                QMessageBox::information(this, "Fail", "修改失败！");
            }
        }else{
            // 用户存在但密码错误
            QMessageBox::warning(this, "Error", "原密码错误！");
        }
    }else{
        QMessageBox::warning(this,"Data not found","数据错误！");
    }

}


void PasswordWindow::on_quitButton_clicked()
{
    MainWindow *mainwindow = new MainWindow(db,nullptr);
    this->close();
    mainwindow->show();
}


void PasswordWindow::on_pushButton_authen_clicked()
{
    AuthenticationWindow *authentication=new AuthenticationWindow(db,username,parent);
    this->close();
    authentication->show();
}

