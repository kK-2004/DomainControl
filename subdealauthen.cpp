#include "subdealauthen.h"
#include "ui_subdealauthen.h"
#include "dealauthen.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

SubDealAuthenWindow::SubDealAuthenWindow(dataBase &db,const UserInfo &userinfo,const QString &managername,QWidget *parent)
    :QMainWindow(parent), ui(new Ui::SubDealAuthenWindow), db(db),userinfo(userinfo),dealauthenwindow(qobject_cast<DealAuthenWindow*>(parent)){
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    setFixedSize(width(), height());
    showInfo();
}

SubDealAuthenWindow::~SubDealAuthenWindow(){
};

void SubDealAuthenWindow::showInfo(){
    QString id=userinfo.id;
    ui->idShow->setText(id);
    // 将 QString 转换为 QByteArray（假设是 UTF-8 编码）
    QByteArray imageData = userinfo.imagePath;  // 转换为 QByteArray

    // 从二进制数据加载图片
    QPixmap pixmap;
    if (pixmap.loadFromData(imageData)) {
        // 加载成功，设置图片到 QLabel
        ui->imageShow->setPixmap(pixmap.scaled(ui->imageShow->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->imageShow->setScaledContents(true);  // 启用缩放内容
    } else {
        // 加载失败
        qDebug() << "Failed to load image from binary data.";
    }
}

void SubDealAuthenWindow::on_passButton_clicked()
{
    QString username=userinfo.username;
    if(db.changeinfo(username,"authentication","1")){
        QMessageBox::information(this,"SUCCESS","已同意该用户的实名认证");
        this->close();
    }else{
        QMessageBox::warning(this,"Fail","数据错误");
    }
}


void SubDealAuthenWindow::on_rejectButton_clicked()
{
    QString username=userinfo.username;
    if(db.changeinfo(username,"authentication","3")){
        QMessageBox::information(this,"SUCCESS","已拒绝该用户的实名认证");
        this->close();
    }else{
        QMessageBox::warning(this,"Fail","数据错误");
    }
}

void SubDealAuthenWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    dealauthenwindow->updateData();
}

