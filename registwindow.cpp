#include "registwindow.h"
#include "ui_registwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

RegistWindow::RegistWindow(dataBase &db,QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RegistWindow), db(db) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    setFixedSize(width(), height());
}

RegistWindow::~RegistWindow() {
    delete ui;
}

void RegistWindow::on_registerButton_clicked() {
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    // 合法性验证
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "请输入用户名！");
        return;
    }

    // 用户名必须是6位以内的数字
    if (!username.contains(QRegularExpression("^[0-9]{1,6}$"))) {
        QMessageBox::warning(this, "Input Error", "用户名必须是6位以内的数字！");
        return;
    }
    if(db.userExists(username))
    {
        QMessageBox::warning(this, "Username Error", "用户名已存在！");
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "密码不能为空！");
        return;
    }

    if (confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "请确认密码！");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "Input Error", "两次输入的密码不一致！");
        return;
    }
    // 正则匹配
    // 密码规则:至少含有1个大写字母、数字、小写字母；8-16位;不能有特殊符号(!@#$%^&*)
    QRegularExpression regex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?!.*[!@#$%^&*])[a-zA-Z\\d]{8,16}$");
    if(!regex.match(password).hasMatch()) {
        QMessageBox::warning(this, "密码不合法！", "密码必须包含至少一个大写字母、一个小写字母和一个数字，长度为8到16个字符，不能包含特殊字符（!@#$%^&*）");
        return;
    }

    // 对密码进行哈希
    QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    // 使用 dataBase 对象注册用户
    if (db.registerUser(username, hashedPassword,0)) {
        QMessageBox::information(this, "Success", "注册成功!");
        QMainWindow *mainWindow = qobject_cast<QMainWindow*>(this->parent());
        if (mainWindow) {
            mainWindow->show(); // 显示主窗口
            this->close(); // 关闭注册窗口
        }
    } else {
        QMessageBox::warning(this, "Error", "注册失败!");
    }
}
