#include "authentication.h"
#include "managerwindow.h"
#include "ui_authentication.h"
#include "user_updatepassword.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

AuthenticationWindow::AuthenticationWindow(dataBase &db, const QString &username, QWidget *parent)
    : QMainWindow(nullptr), ui(new Ui::AuthenticationWindow), db(db), username(username) {
    ui->setupUi(this);
    this->setFixedSize(1080, 720);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    if(db.getinfo(username,"identity")=="用户"){
        this->parent=dynamic_cast<UserWindow *>(parent);
    }else if(db.getinfo(username,"identity")=="管理员"){
        this->parent=dynamic_cast<ManagerWindow *>(parent);
    }
    ui->frame_4->setFrameShape(QFrame::Box);
    ui->frame_4->setFrameShadow(QFrame::Raised);
    ui->pushButton_authen->setEnabled(false);
    ui->pushButton_pass->setCursor(Qt::PointingHandCursor);

    ui->postinfoShow->setEnabled(false);
}

AuthenticationWindow::~AuthenticationWindow(){
};


void AuthenticationWindow::on_back_4_clicked()
{
    this->close();
    UserWindow *newWindow = new UserWindow(db,username,nullptr);
    newWindow->show();
}


void AuthenticationWindow::on_pushButton_pass_clicked()
{
    PasswordWindow* passwordwindow=new PasswordWindow(db,username,parent);
    this->close();
    passwordwindow->show();

}


void AuthenticationWindow::on_postinfoButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("Images (*.png *.jpg *.bmp)"));
    QFileInfo fileInfo(filePath);
    if(db.authenticationPost(username,filePath)){
        ui->postinfoButton->setText("已上传");
        ui->postinfoShow->setText(fileInfo.fileName());
    }else{
        QMessageBox::warning(this,"Fail","上传失败");
    }
}


void AuthenticationWindow::on_pushButton_clicked()
{
    QString id=ui->idInput->text();
    if(ui->lisenceChoose->currentText()=="居民身份证"){
        QRegularExpression regex("^\\d{17}[\\dxX]$");
        if(regex.match(id).hasMatch()){
            if(ui->postinfoButton->text()=="已上传"){
                //更改数据库中用户审核状态
                db.changeinfo(username,"authentication","2");
                db.changeinfo(username,"userid",id);
                QMessageBox::information(this,"SUCCESS","提交成功，等待管理员验证！");
                on_back_4_clicked();
            }else{
                QMessageBox::information(this,"Waring","请上传证件照片！");
            }
        }else{
            QMessageBox::warning(this,"Fail","请输入正确的居民身份证！");
        }
    }else{//采用其他证件验证规则
        QRegularExpression regex("^[\\d]+$");
        if(regex.match(id).hasMatch()){
            if(ui->postinfoButton->text()=="已上传"){
                //更改数据库中用户审核状态
                db.changeinfo(username,"authentication","2");
                db.changeinfo(username,"userid",id);
                QMessageBox::information(this,"SUCCESS","提交成功，等待管理员验证！");
                on_back_4_clicked();
            }else{
                QMessageBox::information(this,"Waring","请上传证件照片！");
            }
        }else{
            QMessageBox::warning(this,"Fail","请输入正确的证件号！");
        }
    }
}


void AuthenticationWindow::on_quitButton_clicked()
{
    MainWindow *mainwindow = new MainWindow(db,nullptr);
    this->close();
    mainwindow->show();
}

