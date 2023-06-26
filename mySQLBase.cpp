#include "mySQLBase.h"

QMap<int , QString> IntToTypeMap {
    {0,"userID"} , {1,"userName"} , {2,"nickName"} , {3,"sex"} , {4,"mobileNumber"}
    , {5,"email"} , {6,"password"} , {7,"points"} , {8,"VIPRank"} , {9,"imagePathName"}
};

QMap<int ,QString> IncomeType{
    {0,"其他"} , {1,"兼职"} , {2,"理财"} , {3,"礼金"} , {4 ,"工资"}
};

QMap<int ,QString> ExpensesType{
    {0,"其他"} , {1,"餐饮"} , {2,"购物"} , {3,"交通"} ,{4,"运动"} , {5,"住房"}
};

QMap<int ,QString> CurrencyType{
    {0,"人民币"} , {1,"美元"} , {2,"欧元"} , {3,"英镑"} , {4,"日元"}
};

//使用自定义 connectionName 创建连接
void mySQLBase::createConnectionByName(const QString &connectionName , QSqlDatabase &newdb){

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectionName);

    // 数据库连接需要设置的信息
    db.setHostName("localhost"); // 数据库服务器IP，用的是本地电脑
    db.setDatabaseName("ceshi");// 数据库名
    db.setUserName("root");// 用户名
    db.setPassword("4567890zxcasd");// 密码
    db.setPort(3306);// 端口号

    // 连接数据库判断
    bool ok = db.open();
    if (ok){
        newdb = db;
    } else {
        qDebug() << "database connect is fail";
    //如果连接失败，要关闭程序
    }
}

//创建新的表单
void mySQLBase::creatTableUsers(QSqlDatabase db){   
    QSqlQuery query(db);
    query.exec("create table Users(userID varchar(8) primary key ,\
               userName varchar(20) not null unique ,nickName varchar(20) null ,\
               sex varchar(10) null ,mobileNumber varchar(11) null ,\
               email varchar(30) null ,password varchar(16) not null ,\
               points int null ,VIPRank int null ,imagePathName varchar(60) null)");

}

void mySQLBase::creatTableBills(QSqlDatabase db){
    QSqlQuery query(db);
    query.exec("create table Bills(billID varchar(10) not null unique ,userID varchar(8) not null ,incomeAndExpenses char(2) not null ,\
                currency varchar(4) not null ,type varchar(6) not null ,priece double not null ,billdate DATETIME not null ,\
                content varchar(100) null)");
}

//向数据库中插入一条数据记录，名称绑定的方式实现
void mySQLBase::insertUser(QSqlDatabase db ,const QString &userID ,const QString &userName ,const QString &password ,
                           const QString nickName ,
                          const QString &sex ,const QString &mobileNumber ,
                          const QString &email ,
                          int points ,int VIPRank ,const QString &imagePathName){

    QSqlQuery query(db);
    query.prepare("INSERT  IGNORE INTO Users (userID, userName, nickName, sex, mobileNumber, email, password, points, VIPRank, imagePathName) \
VALUES (:userID, :userName, :nickName, :sex, :mobileNumber, :email, :password, :points, :VIPRank, :imagePathName)");

            query.bindValue(":userID",userID);
            query.bindValue(":userName",userName);
            query.bindValue(":nickName",nickName);
            query.bindValue(":sex",sex);
            query.bindValue(":mobileNumber",mobileNumber);
            query.bindValue(":email",email);
            query.bindValue(":password",password);
            query.bindValue(":points",points);
            query.bindValue(":VIPRank",VIPRank);
            query.bindValue(":imagePathName",imagePathName);

            query.exec();

}

void mySQLBase::insertBill(QSqlDatabase db,const QString &billID , const QString &userID, bool incomeAndExpenses,
                           int type, double priece, const QString &billdate, const QString &content, int currency){
    QSqlQuery query(db);
    query.prepare("INSERT  IGNORE INTO Bills(billID, userID, incomeAndExpenses, currency, type, priece, billdate, content)\
VALUES (:billID, :userID, :incomeAndExpenses, :currency ,:type, :priece, :billdate, :content)");

    query.bindValue(":billID",billID);
    query.bindValue(":userID",userID);
    if(!incomeAndExpenses){
        query.bindValue(":incomeAndExpenses","收入");
    }else{
        query.bindValue(":incomeAndExpenses","支出");
    }
    query.bindValue(":currency",CurrencyType.value(currency));
    if(!incomeAndExpenses){
        query.bindValue(":type",IncomeType.value(type));
    }else{
        query.bindValue(":type",ExpensesType.value(type));
    }
    query.bindValue(":priece",priece);
    query.bindValue(":billdate",billdate);
    query.bindValue(":content",content);

    query.exec();
}

//执行SQL语句的方式，查询所有的数据记录
void mySQLBase::queryAllUsers(QSqlDatabase db) {
    QString sql = "SELECT * FROM Users" ;
    QSqlQuery query(db);
    query.exec(sql);

    while (query.next()) {                             //遍历查询结果
        qDebug() << QString("Id: %1, Username: %2, Nickname: %3, Sex: %4, Mobilenumber: %5, Email: %6, Password: %7, Points: %8, VIPRank: %9, ImagePathName: %10")
                    .arg(query.value(IntToTypeMap.value(0)).toInt())
                    .arg(query.value(IntToTypeMap.value(1)).toString())
                    .arg(query.value(IntToTypeMap.value(2)).toString())
                    .arg(query.value(IntToTypeMap.value(3)).toString())
                    .arg(query.value(IntToTypeMap.value(4)).toString())
                    .arg(query.value(IntToTypeMap.value(5)).toString())
                    .arg(query.value(IntToTypeMap.value(6)).toString())
                    .arg(query.value(IntToTypeMap.value(7)).toInt())
                    .arg(query.value(IntToTypeMap.value(8)).toInt())
                    .arg(query.value(IntToTypeMap.value(9)).toString());
  }
}

std::vector<std::vector<QString>> mySQLBase::queryAllUsers_V(QSqlDatabase db){
    QString sql = "SELECT * FROM Users" ;
    QSqlQuery query(db);
    query.exec(sql);

    int count = query.size();
    std::vector<std::vector<QString>> temp(count,std::vector<QString>(10));
    int j = 0;

    while (query.next()){
        for(int i = 0;i <= 9;i++){
            if(query.value(i).isNull()){
                temp[j][i] = "null";
            }else{
                temp[j][i]= query.value(i).toString();
            }
        }
        j++;
    }
    return temp;

}

void mySQLBase::queryAllBills(QSqlDatabase db){
    QString sql = "SELECT * FROM Bills" ; // 组装sql语句
    QSqlQuery query(db);                               //传入数据库连接
    query.exec(sql);                                   //执行sql语句

    while (query.next()) {                             //遍历查询结果
        qDebug() << QString("BillID: %1, UserId: %2, IncomeAndExpenses: %3, Currency: %4, Type: %5, Priece: %6, Billdate: %7, Content: %8")
                    .arg(query.value(0).toString())
                    .arg(query.value(1).toString())
                    .arg(query.value(2).toString())
                    .arg(query.value(3).toString())
                    .arg(query.value(4).toString())
                    .arg(query.value(5).toString())
                    .arg(query.value(6).toString())
                    .arg(query.value(7).toString());
  }
}

std::vector<std::vector<QString>> mySQLBase::queryAllBills_V(QSqlDatabase db){
    QString sql = "SELECT * FROM Bills" ;
    QSqlQuery query(db);
    query.exec(sql);

    int count = query.size();
    std::vector<std::vector<QString>> temp(count,std::vector<QString>(8));
    int j = 0;

    while (query.next()){
        for(int i = 0;i <= 7;i++){
            if(query.value(i).isNull()){
                temp[j][i] = "null";
            }else{
                temp[j][i]= query.value(i).toString();
            }
        }
        j++;
    }
    return temp;
}
//删除表单
void mySQLBase::deleteTable(QSqlDatabase db , const QString &TableName){
        QSqlQuery query(db);
        query.exec("drop table if exists " + TableName);
}

//清空表单
void mySQLBase::clearTable(QSqlDatabase db, const QString &TableName){
    QSqlQuery query(db);
    query.exec("delete from " + TableName);
}

//查询指定的用户数据记录
std::vector<QString> mySQLBase::findUserByID_V(QSqlDatabase db , const QString &userID){
    QString sql = "SELECT * FROM Users WHERE userID='" + userID + "'";
    QSqlQuery query(db);
    query.exec(sql);

    std::vector<QString> temp;
    temp.resize(10);
    int i = 0;
    query.first();
    while (i <= 9){
        if(query.value(IntToTypeMap.value(i)).isNull()){
            temp[i] = "null";
        }else{
            temp[i] = query.value(IntToTypeMap.value(i)).toString();
        }
        i++;
    }
    return temp;
}

std::vector<std::vector<QString>> mySQLBase::findBillsByUID_V(QSqlDatabase db , const QString &userID){
    QString sql = "SELECT * FROM Bills WHERE userID='" + userID + "'";
    QSqlQuery query(db);
    query.exec(sql);

    int count = query.size();
    std::vector<std::vector<QString>> temp(count,std::vector<QString>(8));
    int j = 0;

    while (query.next()){
        for(int i = 0;i <= 7;i++){
            if(query.value(i).isNull()){
                temp[j][i] = "null";
            }else{
                temp[j][i]= query.value(i).toString();
            }
        }
        j++;
    }
    return temp;
}

void mySQLBase::findUserByID_P(QSqlDatabase db, const QString &userID){
    QString sql = "SELECT * FROM Users WHERE userID='" + userID + "'";
    QSqlQuery query(db);
    query.exec(sql);

    query.first();
    qDebug() << QString("Id: %1, Username: %2, Nickname: %3, Sex: %4, Mobilenumber: %5, Email: %6, Password: %7, Points: %8, VIPRank: %9, ImagePathName: %10")
                .arg(query.value(IntToTypeMap.value(0)).toInt())
                .arg(query.value(IntToTypeMap.value(1)).toString())
                .arg(query.value(IntToTypeMap.value(2)).toString())
                .arg(query.value(IntToTypeMap.value(3)).toString())
                .arg(query.value(IntToTypeMap.value(4)).toString())
                .arg(query.value(IntToTypeMap.value(5)).toString())
                .arg(query.value(IntToTypeMap.value(6)).toString())
                .arg(query.value(IntToTypeMap.value(7)).toInt())
                .arg(query.value(IntToTypeMap.value(8)).toInt())
                .arg(query.value(IntToTypeMap.value(9)).toString());
}

void mySQLBase::findBillsByUID_P(QSqlDatabase db, const QString &userID){
    QString sql = "SELECT * FROM Bills WHERE userID='" + userID + "'";
    QSqlQuery query(db);
    query.exec(sql);

    while (query.next()) {                             //遍历查询结果
        qDebug() << QString("BillID: %1, UserId: %2, IncomeAndExpenses: %3, Currency: %4, Type: %5, Priece: %6, Billdate: %7, Content: %8")
                    .arg(query.value(0).toString())
                    .arg(query.value(1).toString())
                    .arg(query.value(2).toString())
                    .arg(query.value(3).toString())
                    .arg(query.value(4).toString())
                    .arg(query.value(5).toString())
                    .arg(query.value(6).toString())
                    .arg(query.value(7).toString());
  }
}

std::vector<QString> mySQLBase::findBillByBID_V(QSqlDatabase db, const QString &billID){
    QString sql = "SELECT * FROM Bills WHERE billID='" + billID + "'";
    QSqlQuery query(db);
    query.exec(sql);

    std::vector<QString> temp;
    temp.resize(8);
    int i = 0;
    query.first();
    while (i <= 7){
        if(query.value(i).isNull()){
            temp[i] = "null";
        }else{
            temp[i] = query.value(i).toString();
        }
        i++;
    }
    return temp;
}

void mySQLBase::findBillByBID_P(QSqlDatabase db, const QString &billID){
    QString sql = "SELECT * FROM Bills WHERE billID='" + billID + "'";
    QSqlQuery query(db);
    query.exec(sql);

    query.first();
    qDebug() << QString("BillID: %1, UserId: %2, IncomeAndExpenses: %3, Currency: %4, Type: %5, Priece: %6, Billdate: %7, Content: %8")
                .arg(query.value(0).toString())
                .arg(query.value(1).toString())
                .arg(query.value(2).toString())
                .arg(query.value(3).toString())
                .arg(query.value(4).toString())
                .arg(query.value(5).toString())
                .arg(query.value(6).toString())
                .arg(query.value(7).toString());
}

std::vector<QString> mySQLBase::findUserByName_V(QSqlDatabase db, const QString &userName){
    QString sql = "SELECT * FROM Users WHERE userName='" + userName + "'";
    QSqlQuery query(db);
    query.exec(sql);

    std::vector<QString> temp;
    temp.resize(10);
    int i = 0;
    query.first();
    while (i <= 9){
        if(query.value(IntToTypeMap.value(i)).isNull()){
            temp[i] = "null";
        }else{
            temp[i] = query.value(IntToTypeMap.value(i)).toString();
        }
        i++;
    }
    return temp;
}

void mySQLBase::findUserByName_P(QSqlDatabase db, const QString &userName){
    QString sql = "SELECT * FROM Users WHERE userName='" + userName + "'";
    QSqlQuery query(db);
    query.exec(sql);

    query.first();
    qDebug() << QString("Id: %1, Username: %2, Nickname: %3, Sex: %4, Mobilenumber: %5, Email: %6, Password: %7, Points: %8, VIPRank: %9, ImagePathName: %10")
                .arg(query.value(IntToTypeMap.value(0)).toInt())
                .arg(query.value(IntToTypeMap.value(1)).toString())
                .arg(query.value(IntToTypeMap.value(2)).toString())
                .arg(query.value(IntToTypeMap.value(3)).toString())
                .arg(query.value(IntToTypeMap.value(4)).toString())
                .arg(query.value(IntToTypeMap.value(5)).toString())
                .arg(query.value(IntToTypeMap.value(6)).toString())
                .arg(query.value(IntToTypeMap.value(7)).toInt())
                .arg(query.value(IntToTypeMap.value(8)).toInt())
                .arg(query.value(IntToTypeMap.value(9)).toString());
}

QString mySQLBase::getUserDataByID(QSqlDatabase db, const QString &userID, const QString &typeName){
    QString sql = "SELECT " + typeName + " FROM Users WHERE userID=:userID";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":userID", userID);
    query.exec();

    query.first();
    if(query.value(0).isNull()){
        return "null";
    }else {
        return query.value(0).toString();
    }
}

QString mySQLBase::getUserDataByName(QSqlDatabase db, const QString &userName, const QString &typeName){
    QString sql = "SELECT " + typeName + " FROM Users WHERE userName=:userName";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":userName", userName);
    query.exec();

    query.first();
    if(query.value(0).isNull()){
        return "null";
    }else {
        return query.value(0).toString();
    }
}

std::vector<std::vector<QString>> mySQLBase::getUsersByData(QSqlDatabase db, const QString &typeName, const QString &typeValue){
    QString sql = "SELECT * FROM Users WHERE " + typeName + "=:value";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":value", typeValue);
    query.exec();

    int count = query.size();
    std::vector<std::vector<QString>> temp(count,std::vector<QString>(10));
    int j = 0;

    while (query.next()){
        for(int i = 0;i <= 9;i++){
            if(query.value(i).isNull()){
                temp[j][i] = "null";
            }else{
                temp[j][i]= query.value(i).toString();
            }
        }
        j++;
    }
    return temp;
}

QString mySQLBase::getBillData(QSqlDatabase db, const QString &billID, const QString &typeName){
    QString sql = "SELECT " + typeName + " FROM Bills WHERE billID=:billID";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":billID", billID);
    query.exec();

    query.first();
    if(query.value(0).isNull()){
        return "null";
    }else {
        return query.value(0).toString();
    }
}

//删除指定用户
void mySQLBase::deleteUserByID(QSqlDatabase db, const QString &userID){
    QSqlQuery query(db);
        query.prepare("DELETE FROM Users WHERE userID=:userID");
        query.bindValue(":userID", userID);
        query.exec();
}

void mySQLBase::deleteUserByName(QSqlDatabase db, const QString &userName){
    QSqlQuery query(db);
        query.prepare("DELETE FROM Users WHERE userName=:userName");
        query.bindValue(":userName", userName);
        query.exec();
}

void mySQLBase::deleteBillByBID(QSqlDatabase db, const QString &billID){
    QSqlQuery query(db);
        query.prepare("DELETE FROM Bills WHERE billID=:billID");
        query.bindValue(":billID", billID);
        query.exec();
}

void mySQLBase::deleteBillsByUID(QSqlDatabase db, const QString &userID){
    QSqlQuery query(db);
        query.prepare("DELETE FROM Bills WHERE userID=:userID");
        query.bindValue(":userID", userID);
        query.exec();
}

void mySQLBase::deleteBillByDate(QSqlDatabase db, const QString &billDate){
    QString sql = "SELECT billID,billdate FROM Bills" ;
    QSqlQuery query(db);
    query.exec(sql);

    int count = query.size();
    std::vector<std::vector<QString>> temp(count,std::vector<QString>(2));
    int j = 0;

    while (query.next()){
        temp[j][0] = query.value(0).toString();
        temp[j][1] = query.value(1).toString();
        j++;
    }
    QString p1 = billDate.mid(0,4);
    QString p2 = billDate.mid(5,2);
    QString p3 = billDate.mid(8,2);
    if(p1 == "XXXX"){
        clearTable(db, "Bills");
    }else if(p2 == "XX"){
        QString use_p = billDate.mid(0,4);
        for(int i = 0; i < int(temp.size()); i++){
        QString tempDate = temp[i][1].mid(0, 4);
        if(tempDate == use_p){
            deleteBillByBID(db,temp[i][0]);
            }
        }
    }else if(p3 == "XX"){
        QString use_p = billDate.mid(0,7);
        for(int i = 0; i < int(temp.size()); i++){
        QString tempDate = temp[i][1].mid(0, 7);
        if(tempDate == use_p){
            deleteBillByBID(db,temp[i][0]);
            }
        }
    }else{
        QString use_p = billDate.mid(0,10);
        for(int i = 0; i < int(temp.size()); i++){
        QString tempDate = temp[i][1].mid(0, 10);
        if(tempDate == use_p){
            deleteBillByBID(db,temp[i][0]);
            }
        }
    }
}

void mySQLBase::deleteBillByDate_User(QSqlDatabase db, const QString &billDate, const QString &userID){
    QString sql = "SELECT billID,billdate FROM Bills WHERE userID =:userID" ;
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":userID",userID);
    query.exec();

    int count = query.size();
    std::vector<std::vector<QString>> temp(count,std::vector<QString>(2));
    int j = 0;

    while (query.next()){
        temp[j][0] = query.value(0).toString();
        temp[j][1] = query.value(1).toString();
        j++;
    }
    QString p1 = billDate.mid(0,4);
    QString p2 = billDate.mid(5,2);
    QString p3 = billDate.mid(8,2);
    if(p1 == "XXXX"){
        clearTable(db, "Bills");
    }else if(p2 == "XX"){
        QString use_p = billDate.mid(0,4);
        for(int i = 0; i < int(temp.size()); i++){
        QString tempDate = temp[i][1].mid(0, 4);
        if(tempDate == use_p){
            deleteBillByBID(db,temp[i][0]);
            }
        }
    }else if(p3 == "XX"){
        QString use_p = billDate.mid(0,7);
        for(int i = 0; i < int(temp.size()); i++){
        QString tempDate = temp[i][1].mid(0, 7);
        if(tempDate == use_p){
            deleteBillByBID(db,temp[i][0]);
            }
        }
    }else{
        QString use_p = billDate.mid(0,10);
        for(int i = 0; i < int(temp.size()); i++){
        QString tempDate = temp[i][1].mid(0, 10);
        if(tempDate == use_p){
            deleteBillByBID(db,temp[i][0]);
            }
        }
    }
}

//修改用户数据
void mySQLBase::updateUserByID(QSqlDatabase db, const QString &userID, const QString &typeName, const QString &valueNew){
    QString sql = "update Users set " + typeName + "=:newvalue WHERE userID=:id";
    QSqlQuery query(db);
       query.prepare(sql);
       query.bindValue(":id", userID);
       if(typeName == "points" || typeName =="VIPRank"){
           query.bindValue(":newvalue", valueNew.toInt());
       }else{
           query.bindValue(":newvalue", valueNew);
       }
       query.exec();
}

void mySQLBase::updateUserByName(QSqlDatabase db, const QString &userName, const QString &typeName, const QString &valueNew){
    QString sql = "update Users set " + typeName + "=:newvalue WHERE userName=:name";
    QSqlQuery query(db);
       query.prepare(sql);
       query.bindValue(":name", userName);
       if(typeName == "points" || typeName =="VIPRank"){
           query.bindValue(":newvalue", valueNew.toInt());
       }else{
           query.bindValue(":newvalue", valueNew);
       }
       query.exec();
}

void mySQLBase::updateBillByBID(QSqlDatabase db, const QString &billID, const QString &typeName, const QString &valueNew){
    QString sql = "update Bills set " + typeName + "=:newvalue WHERE billID=:id";
    QSqlQuery query(db);
       query.prepare(sql);
       query.bindValue(":id", billID);
       if(typeName == "currency"){
          query.bindValue(":newvalue", CurrencyType.value(valueNew.toInt()));
       }else if(typeName == "type"){
           std::vector<QString> temp = findBillByBID_V(db, billID);
           if(temp[2] == "收入"){
               query.bindValue(":newvalue", IncomeType.value(valueNew.toInt()));
           }else{
               query.bindValue(":newvalue", ExpensesType.value(valueNew.toInt()));
           }
       }else {
            query.bindValue(":newvalue",valueNew);
    }
       query.exec();
}

void mySQLBase::showTotalTable(QSqlDatabase db){
    QString sql = "SELECT*FROM users LEFT JOIN bills on users.userid=bills.userid";
    QSqlQuery query(db);
    query.exec(sql);

    while (query.next()) {                             //遍历查询结果
        qDebug() << QString("Id: %1, Username: %2, Nickname: %3, Sex: %4, Mobilenumber: %5, Email: %6, Password: %7, Points: %8, VIPRank: %9, ImagePathName: %10, BillID: %11, UserId: %12, IncomeAndExpenses: %13, Currency: %14, Type: %15, Priece: %16, Billdate: %17, Content: %18")
                    .arg(query.value(IntToTypeMap.value(0)).toInt())
                    .arg(query.value(IntToTypeMap.value(1)).toString())
                    .arg(query.value(IntToTypeMap.value(2)).toString())
                    .arg(query.value(IntToTypeMap.value(3)).toString())
                    .arg(query.value(IntToTypeMap.value(4)).toString())
                    .arg(query.value(IntToTypeMap.value(5)).toString())
                    .arg(query.value(IntToTypeMap.value(6)).toString())
                    .arg(query.value(IntToTypeMap.value(7)).toInt())
                    .arg(query.value(IntToTypeMap.value(8)).toInt())
                    .arg(query.value(IntToTypeMap.value(9)).toString())
                    .arg(query.value(10).toString())
                    .arg(query.value(11).toString())
                    .arg(query.value(12).toString())
                    .arg(query.value(13).toString())
                    .arg(query.value(14).toString())
                    .arg(query.value(15).toString())
                    .arg(query.value(16).toString())
                    .arg(query.value(17).toString());
  }
}
