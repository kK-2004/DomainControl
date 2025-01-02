#ifndef DATABASE_H
#define DATABASE_H
#include "headers.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFileInfo>
#include <QImage>
#include <QBuffer>
#include <QPixmap>
#include <QVector>


// 定义一个结构体来存储查询结果
struct UserInfo {
    QString username;
    QString id;
    QByteArray imagePath;
};

QT_BEGIN_NAMESPACE
class dataBase{

public:
    dataBase();
    bool userExists(const QString &username) const;
    bool validateUser(const QString &username, const QString &hashedPassword) const;
    bool registerUser(const QString &username, const QString &hashedPassword,int identity);
    bool UserChangePassword(const QString &username,const QString &hashedNewPassword);
    bool authenticationPost(const QString &username,const QString &filePath);
    QImage showPicture(const QString &username);
    QString getinfo(const QString &username,const QString &info);
    bool changeinfo(const QString &username,const QString &info,const QString &val);
    QString getNum(const QString &info,const QString &val);
    QVector<UserInfo> fetchUserInfoByCondition(const QString &column, const QVariant &value);
    bool deleteUser(const QString &username);
    ~dataBase();
private:
    QSqlDatabase db;
    bool checkAndCreateUserTable();
};
QT_END_NAMESPACE


#endif // DATABASE_H
