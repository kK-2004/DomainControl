#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registwindow.h"
#include "userwindow.h"
#include "managerwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

MainWindow::MainWindow(dataBase &db,QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),db(db){
    ui->setupUi(this);
    setFixedSize(width(), height());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_registerButton1_clicked() {
    RegistWindow *registWindow = nullptr;
    if (!registWindow) {
        registWindow = new RegistWindow(db,this);
        connect(registWindow, &RegistWindow::destroyed, [&]() { registWindow = nullptr; });
    }
    registWindow->show();
    this->hide();
}

void MainWindow::on_loginButton_clicked() {
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "请输入用户名！");
        return;
    }
    if (password.isEmpty()){
        QMessageBox::warning(this,"Input Error","密码不能为空！");
        return;
    }
    // 用户名必须是6位以内的数字
    if (!username.contains(QRegularExpression("^[0-9]{1,6}$"))) {
        QMessageBox::warning(this, "Input Error", "用户名必须是6位以内的数字！");
        return;
    }
    ////////////////先判断用户是否存在，再验证密码

    // 检查用户名是否存在
    if(db.userExists(username)){
        // 用户存在, 继续验证密码
        if(db.validateUser(username, hashedPassword)){
            QMessageBox::information(this, "Success", "登录成功!");
            // 登录后续 0：普通用户 1：管理员
            if(db.getinfo(username,"identity")=="用户"){
                UserWindow *userWindow = new UserWindow(db, username,nullptr);
                // 关闭当前登录界面
                this->close();
                userWindow->show();  // 显示用户界面
            }else{
                //////////////////处理管理员登录后续
                ManagerWindow *managerwindow = new ManagerWindow(db,username,nullptr);
                this->close();
                managerwindow->show();
            }

        } else {
            // 用户存在但密码错误
            QMessageBox::warning(this, "Error", "密码错误！");
        }
    } else {
        // 用户不存在
        QMessageBox::warning(this, "Error", "用户名不存在！");
    }

}

//回车确定
void MainWindow::on_passwordLineEdit_returnPressed()
{
    on_loginButton_clicked();
}

