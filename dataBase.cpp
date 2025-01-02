#include "dataBase.h"

dataBase::dataBase(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    // 如果数据库文件不存在，创建一个新的空数据库
    db.open();
    // 数据库连接成功后，检查并创建表（如果表不存在）
    if (!checkAndCreateUserTable()) {
        db.close();
        qDebug() << "Failed to create or check user table. Database closed.";
    } else {
        qDebug() << "SUCCESS！用户表已存在或已创建";
    }

}
bool dataBase::checkAndCreateUserTable() {
    QSqlQuery query(db);

    // 检查表是否存在
    query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='users'");
    if (query.exec() && query.next()) {
        qDebug() << "用户表已存在";
        return true; // 表已存在，无需创建
    }

    // 如果表不存在，则创建表
    //authenticaition:0-未实名,1-已实名,2-待审核，3-审核不通过，需重新提交审核
    QString createTableQuery = R"(
        CREATE TABLE users(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            identity INTEGER NOT NULL DEFAULT 0,
            authentication INTEGER NOT NULL DEFAULT 0,
            userid TEXT,
            image_path BLOB
        );
    )";

    if (query.exec(createTableQuery)) {
        qDebug() << "用户表创建成功";

        // 插入默认管理员
        QString admin_username = "000000";
        QString admin_password = "Abc123";
        QString hashed_admin_password = QString(QCryptographicHash::hash(admin_password.toUtf8(), QCryptographicHash::Sha256).toHex());

        QSqlQuery insertQuery(db);
        insertQuery.prepare(R"(
        INSERT INTO users (username, password, identity, authentication)
        VALUES (:username, :password, :identity, :authentication)
        )");
        insertQuery.bindValue(":username", admin_username);
        insertQuery.bindValue(":password", hashed_admin_password);
        insertQuery.bindValue(":identity", 1);
        insertQuery.bindValue(":authentication", 1);

        if (insertQuery.exec()) {
            qDebug() << "默认管理员已成功插入";
        } else {
            qWarning() << "插入默认管理员失败：" << insertQuery.lastError().text();
        }
        return true;
    } else {
        qWarning() << "用户表创建失败：" << query.lastError().text();
        return false;
    }
}


bool dataBase::userExists(const QString &username) const {
    QSqlQuery query(db);
    query.prepare("SELECT id FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

bool dataBase::validateUser(const QString &username, const QString &hashedPassword) const {
    QSqlQuery query(db);
    query.prepare("SELECT id FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    return query.exec() && query.next();
}

bool dataBase::registerUser(const QString &username,const QString &hashedPassword,const int identity){
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password, identity) VALUES (:username, :password, :identity)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    query.bindValue(":identity", identity);

    if (query.exec()) {
        return true;
    } else {
        qWarning() << "注册失败：" << query.lastError().text();
        return false;
    }
}



QString dataBase::getinfo(const QString &username,const QString &info){
    QSqlQuery query(db);
    query.prepare("SELECT password, identity, authentication FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec()) {
        if (query.next()) {
            // QString password = query.value(0).toString();  // 获取密码
            // int identity = query.value(1).toInt();  // 获取身份
            if(info=="identity"){
                int identity = query.value(1).toInt();
                if(identity==0)
                {
                    return "用户";
                }else if(identity==1)
                {
                    return "管理员";
                }
            }else if(info=="authentication"){
                int authentication=query.value(2).toInt();
                if(authentication==0){
                    return "未实名";
                }else if(authentication==1){
                    return "已实名";
                }else if(authentication==2){
                    return "审核中";
                }else if(authentication==3){
                    return "审核未通过";
                }
            }else{
                return info+"信息不存在";
            }
        } else {
            // 用户名不存在
            return "User not found!";
        }
    } else {
        // 查询失败，输出错误信息
        qDebug() << "Query failed: " << query.lastError().text();
        return "Query failed!";
    }
    return "";
}

bool dataBase::changeinfo(const QString &username,const QString &info,const QString &val){
    QSqlQuery query(db);
    if(info=="authentication"){
        query.prepare("UPDATE users SET authentication = :authentication WHERE username = :username");
        query.bindValue(":authentication", val.toInt());
        query.bindValue(":username", username);
        if(query.exec()) {
            return true;
        } else {
            return false;
        }
    }else if(info=="userid"){
        query.prepare("UPDATE users SET userid = :userid WHERE username = :username");
        query.bindValue(":userid", val);
        query.bindValue(":username", username);
        if(query.exec()) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool dataBase::UserChangePassword(const QString &username,const QString &hashedNewPassword){
    QSqlQuery query(db);
    query.prepare("UPDATE users SET password = :password WHERE username = :username");
    query.bindValue(":password", hashedNewPassword);  // 绑定新密码
    query.bindValue(":username", username);  // 绑定用户名

    if(query.exec()) {
        return true;
    } else {
        return false;
    }
}

bool dataBase::authenticationPost(const QString &username,const QString &filePath){
    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower(); // 获取文件扩展名（如 png、jpg）
    QImage image(filePath);
    if (image.isNull()) {
        qDebug() << "无法加载图片";
        return false;
    }
    // 使用 QBuffer 将图像保存到 QByteArray
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    // 自动识别格式并保存
    if (image.save(&buffer, suffix.toUtf8().constData())) {
        qDebug() << "图片已保存到 QByteArray";
    } else {
        qDebug() << "保存图片失败";
        return false;
    }
    QSqlQuery query(db);
    query.prepare("UPDATE users SET image_path = :imageData WHERE username = :username");
    query.bindValue(":username", username);
    query.bindValue(":imageData", byteArray);

    if (query.exec()) {
        qDebug() << "图片已保存到数据库";
        return true;
    } else {
        qDebug() << "保存图片失败: " << query.lastError().text();
    }
    return false;
}

QImage dataBase::showPicture(const QString &username){
    QSqlQuery query(db);
    query.prepare("SELECT image_path FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        QByteArray imageData = query.value(0).toByteArray();  // 获取图片数据

        QImage image;
        if (image.loadFromData(imageData)) {
            qDebug() << "图片返回成功";
            return image;
        } else {
            qDebug() << "图片加载失败";
        }
    } else {
        qDebug() << "查询失败: " << query.lastError().text();
    }
    return QImage();
}

QString dataBase::getNum(const QString &info,const QString &val){
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM users WHERE authentication = :authentication");
    query.bindValue(":authentication", val.toInt());
    if (query.exec()&& query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Num查询失败" << query.lastError().text();
        return "";
    }
}

QVector<UserInfo> dataBase::fetchUserInfoByCondition(const QString &column, const QVariant &value){

    QVector<UserInfo> results;
    QSqlQuery query;
    query.prepare(QString("SELECT username, userid, image_path FROM users WHERE %1 = :value").arg(column));
    query.bindValue(":value", value);

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return results;
    }

    // 遍历查询结果
    while (query.next()) {
        UserInfo user;
        user.username = query.value("username").toString();
        user.id = query.value("userid").toString();
        user.imagePath = query.value("image_path").toByteArray();
        results.append(user);
    }
    return results;
}

bool dataBase::deleteUser(const QString &username){
    QSqlQuery query;
    query.prepare("DELETE FROM USERS WHERE username = :username");
    query.bindValue(":username",username);
    if(query.exec()){
        return true;
    }else{
        return false;
    }
}

dataBase::~dataBase(){
    db.close();
    qDebug() << "SUCCESS！数据库已断开";
};
