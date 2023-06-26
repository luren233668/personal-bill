#ifndef DAYI_MYSQLBASE_H
#define DAYI_MYSQLBASE_H

#include <QtSql/QSqlDatabase>
#include<QDebug>
#include <QSqlQuery>
#include<vector>
#include<string>
#include<qmap.h>
#include"mainwindow.h"

class mySQLBase {
public:
    void createConnectionByName(const QString &connectionName , QSqlDatabase &newdb);
    //使用自定义 connectionName 创建连接
    //要用一个QSqlDateBase类型的数据引用传递用于接收
    //写法为:
    //mySQLBase 对象名;
    //QSqlDatabase 变量名 ;
    //对象名.createConnectionByName("ConnectionName" , 变量名);
    //在连接数据库失败以后应该关闭程序,补到函数体内

    void creatTableUsers(QSqlDatabase db);
    //创建Users表单
    //用与user有关的操作前可以都写一遍
    //如果已有则会自动忽略

    void creatTableBills(QSqlDatabase db);
    //创建Bills表单
    //用与bill有关的操作前可以都写一遍
    //如果已有则会自动忽略

    void insertUser(QSqlDatabase db ,const QString &userID ,const QString &userName ,const QString &password ,
                                    const QString nickName = "" ,
                                   const QString &sex  = "" ,const QString &mobileNumber  = "" ,
                                   const QString &email  = "" ,
                                   int points  = 0,int VIPRank  = 0,const QString &imagePathName = "" );
    // 增加用户数据操作
    //要求必须有唯一ID，唯一用户名和密码，其他默认为空
    //填充顺序依次为QSqlDatabase db，ID，用户名，密码，昵称，性别，电话号，邮箱，积分，vip等级，头像图片地址
    //如果此用户已存在，则自动忽略

    void insertBill(QSqlDatabase db ,const QString &billID ,const QString &userID ,bool incomeAndExpenses ,
                    int type ,double priece ,const QString &billdate ,const QString &content  = "",int currency  = 0);
    // 增加账单数据操作
    //要求必须有对应用户的ID，收入支出和对应类型（用int值传递），金额 ，账单创建时间
    //填充顺序依次为QSqlDatabase db，账单ID，用户ID，收入支出的判断， 类型对应的int值，金额 ，日期 ，备注和货币种类
    //备注最长100字，默认为空
    //货币种类默认为人民币

    void queryAllUsers(QSqlDatabase db);
    //全查操作，查询并直接打印出所有用户的数据记录
    //应该只有管理员账户可以使用

    std::vector<std::vector<QString>> queryAllUsers_V(QSqlDatabase db);
    //全查操作的数组版本，返回类型为Vector<Vector<Qstring>>
    //返回一个包含用户的全部信息的数组组成的数组
    //使用方式为test[i][j]
    //第二维的数组内部顺序依次为ID，用户名，密码，昵称，性别，电话号，邮箱，积分，vip等级，头像图片地址
    //第一维的数组的长度用int(test.size())来取

    void queryAllBills(QSqlDatabase db);
    //全查操作，查询并直接打印出所有用户的全部账单数据记录
    //应该只有管理员账户可以使用

    std::vector<std::vector<QString>> queryAllBills_V(QSqlDatabase db);
    //全查操作的数组版本，返回类型为Vector<Vector<Qstring>>
    //返回一个包含用户的全部信息的数组组成的数组
    //使用方式为test[i][j]
    //第二维的数组内部顺序依次为账单ID，用户ID，收入还是支出，货币种类， 类型，金额 ，日期 ，备注
    //第一维的数组的长度用int(test.size())来取

    std::vector<QString> findUserByID_V(QSqlDatabase db , const QString &userID);
    //通过用户ID查找一名用户的全部信息
    //返回类型为Vector<Qstring>，有十位元素
    //顺序依次为ID，用户名，密码，昵称，性别，电话号，邮箱，积分，vip等级，头像图片地址
    //示例：
    //std::vector<QString> test = newDateBase.findUserByID_V(db, "2856526");
    //for (int i = 0; i <=9; i++) {
    //qDebug() << test[i];
    // }

    std::vector<std::vector<QString>> findBillsByUID_V(QSqlDatabase db , const QString &userID);
    //通过用户ID查找一名用户的全部账单
    //返回类型为Vector<Vector<Qstring>>
    //即一个由 包含一个账单全部信息的数组组成的数组
    //示例:
    //    std::vector<std::vector<QString>> test = newDateBase.findBillsByID_V(db, "214586");
    //for (int i = 0; i <int(test.size()); i++) {
    //    for (int j = 0;j <= 6; j++) {
    //        qDebug() << test[i][j];
    //    }
    //}

    void findUserByID_P(QSqlDatabase db , const QString &userID);
    //通过用户ID查找一名用户的全部信息
    //不返回，直接打印输出
    //示例：对象名.findUserByID_P(db, "2856526");

    void findBillsByUID_P(QSqlDatabase db , const QString &userID);
    //通过用户ID查找一名用户的全部账单
    //不返回，直接打印输出
    //示例：对象名.findBillsByID_P(db, "2856526");

    std::vector<QString> findUserByName_V(QSqlDatabase db , const QString &userName);
    //通过用户名查找一名用户的全部信息
    //返回类型为Vector<Qstring>
    //使用方法同ID

    void findUserByName_P(QSqlDatabase db , const QString &userName);
    //通过用户Name查找一名用户的全部信息
    //不返回，直接打印输出
    //使用方法同ID

    std::vector<QString> findBillByBID_V(QSqlDatabase db , const QString &billID);
    //通过账单ID查找指定的账单
    //返回包含一个账单全部信息的数组
    //使用方法同上

    void findBillByBID_P(QSqlDatabase db , const QString &billID);
    //通过账单ID查找指定的账单
    //不返回，直接打印输出
    //使用方法同上

    QString getUserDataByID(QSqlDatabase db, const QString &userID, const QString &typeName);
    //通过用户ID查找指定类型的数据
    //typeName为字符串
    //可用"sex" "nickname"等

    QString getUserDataByName(QSqlDatabase db, const QString &userName, const QString &typeName);
    //通过用户名查找指定类型的数据
    //typeName为字符串
    //可用"sex" "nickname"等

    std::vector<std::vector<QString>> getUsersByData(QSqlDatabase db, const QString &typeName, const QString &typeValue);
    //查找对应的数据值相符的用户
    //返回值为包含有符合条件的用户的全部数据的数组的数组
    //typeName和typeValue为字符串
    //可用"sex" "nickname""男女"等

    QString getBillData(QSqlDatabase db, const QString &billID, const QString &typeName);
    //通过账单ID查找指定类型的数据
    //typeName为字符串
    //可用"billdata" "priece"等

    void deleteTable(QSqlDatabase db ,const QString &TableName);
    //删除指定的表单
    //要求TableName必须为字符串，即"Users"或"Bills"
    //应该只有管理员账户可以使用
    //如果不存在则自动忽略
    //十分危险，会导致全部成为野指针
    //在再次使用其他操作前
    //必须重新创建表单

    void clearTable(QSqlDatabase db ,const QString &TableName);
    //清空指定的表单
    //要求TableName必须为字符串，即"Users"或"Bills"
    //应该只有管理员账户可以使用
    //如果不存在则自动忽略

    void deleteUserByID(QSqlDatabase db,const QString &userID);
    //删除指定的用户，通过ID指定
    //应该只有管理员账户可以使用

    void deleteUserByName(QSqlDatabase db,const QString &userName);
    //删除指定的用户，通过Name指定
    //应该只有管理员账户可以使用

    void deleteBillByBID(QSqlDatabase db,const QString &billID);
    //删除指定的账单，用BillID指定

    void deleteBillsByUID(QSqlDatabase db,const QString &userID);
    //删除一名用户的全部账单，用UserID指定

    void deleteBillByDate(QSqlDatabase db, const QString &billDate);
    //通过指定的日期删除账单
    //日期格式为"XXXX-XX-XX"
    //可通过"2022-05-XX"形式来删除2022年5月的所有账单
    //此版为全用户搜索

    void deleteBillByDate_User(QSqlDatabase db, const QString &billDate ,const QString &userID);
    //通过指定的日期删除账单
    //日期格式为"XXXX-XX-XX"
    //可通过"2022-05-XX"形式来删除2022年5月的所有账单
    //此版为指定用户搜索

    void updateUserByID(QSqlDatabase db,const QString &userID, const QString &typeName , const QString &valueNew);
    //修改指定类型的用户数据，通过ID查找
    //typeName是要修改的数据的类别
    //必须是"password" "sex" 等
    //valueNew是修改后的值
    //注意都是字符串

    void updateUserByName(QSqlDatabase db,const QString &userName, const QString &typeName , const QString &valueNew);
    //修改指定类型的用户数据，通过用户名查找
    //使用方法同上

    void updateBillByBID(QSqlDatabase db, const QString &billID, const QString &typeName, const QString &valueNew);
    //修改指定类型的账单数据，通过billID查找
    //使用方法同上
    //类型和货币用"int值"来传，对应的在qmap里
    //收支不能改

    void showTotalTable(QSqlDatabase db);
    //左连接
    //显示所有用户的所有的统合信息
    //包括了用户信息和账单信息
};

#endif //DAYI_MYSQLBASE_H
