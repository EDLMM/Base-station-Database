#include "clientend.h"
//-------------------------------------
//-------------------------------------
ClientAccess::ClientAccess()
{   this->username="none";
    this->password="none";

    udb = QSqlDatabase::addDatabase("QODBC");
    udb.setDatabaseName(QString("DRIVER={SQL SERVER};"
                               "SERVER=%1;" //服务器名称
                               "DATABASE=%2;"//数据库名
                               "UID=%3;"           //登录名
                               "PWD=%4;"        //密码
                               )
                       .arg("localhost")
                       .arg("experiment")
                       .arg("qtuser")
                       .arg("123456")
                       );

    if (!udb.isValid()){
        qDebug() << udb.lastError().text();
        return;
    }
    if(!udb.open()){
        qDebug()<<"fail to open!";
    }
    qDebug()<<"database open success!";
    QSqlQuery query(udb); //查询Card表并输出，测试能否正常操作数据库
    query.exec("SELECT * FROM users");
    while(query.next())
    {
        qDebug()<<query.value(0).toString();// <<query.value(1).toString();
    }

}

bool ClientAccess::Register(QString username,QString password)
{
//    QString h_password=QString(QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Sha3_256));
    QString qinsert;
    qinsert = QString("INSERT INTO users (username,password)\
                       values ('%1','%2');").arg(username).arg(password);
    udb.exec(qinsert);
    if(udb.lastError().isValid()){
        qDebug()<<"user register failed.";
        return false;
    }else{
        qDebug()<<"user register succeed.";
        return true;
    }
}

int ClientAccess::LoginValid(QString username, QString password)
{
    QSqlQuery query=udb.exec(QString("SELECT * FROM users WHERE username='%1' ").arg(username));//
    //find user
    query.next();
    if(query.value(0).toString()==username){
        qDebug()<<"user found";
    }else {
        qDebug()<< "no such user";
//        this->releaseDebug = QString("%1").arg(query.lastError().text());//.arg(query.value(0).toString())
        return 0;
    }
    //match password
//    QString h_password=QString(QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Sha3_256));

    if(password ==query.value(1).toString()){
        qDebug()<<"password match";
        //load user information to local client
        this->username=username;
        this->password=password;
        return 2;
    }else{
        return 1;
    }
}

bool ClientAccess::execSQL(QString sentence)
{
    udb.exec(sentence);
    qDebug()<<sentence<<" execution state:";
    if(udb.lastError().isValid()){
        qDebug()<<"fail";
        return false;
    }else{
        qDebug()<<"succeed.";
        return true;
    }
}


QList<QString> ClientAccess::SearchColumn\
    (QString tablename,int isunique,QString type,QString attribute)
{
    qDebug()<<"running search column";
    QList<QString> r;
    QString dis;
    if(isunique==1) {
        dis=QString("DISTINCT");
    }else{
        dis=QString("");
    }

    QString q;
    if("all"==type.toLatin1()){
        q=QString("SELECT %1 %2 FROM %3;").arg(dis).arg(attribute).arg(tablename);
    }
    QSqlQuery query(udb);
    qDebug()<<"running "<<q;
    query.exec(q);
    while(query.next()){
//        qDebug()<<"r get "<<query.value(0).toString();
        r.append(query.value(0).toString());
    }
    return r;
}

QVector<double> ClientAccess::SearchColumn\
    (QString tablename,QString type,QString attribute,QString searchvalue,QString S,QString E)
{
    qDebug()<<"running search column";
    QVector<double> r;

    QString q;
    if("timestamp"==type.toLatin1()){
        q=QString("select %1 from %4 where STARTTIME>= '%2' and STARTTIME <= '%3' and order by STATRTTIME, %1;").arg(attribute).arg(S).arg(E).arg(tablename);
    }
    QSqlQuery query(udb);
    qDebug()<<"running "<<q;
    query.exec(q);
    while(query.next()){
//        qDebug()<<"r get "<<query.value(0).toString();
        r.append(query.value(0).toDouble());
    }
    return r;
}

/*
QString ClientAccess::loadCSV(QString filename)
{
    int succount=0;
    int failcount=0;
    QString response;
    //default file is "TOEFL核心词汇21天突破.CSV"
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {

        int lineindex = 0;                     // file line counter
        QTextStream in(&file);                 // read to text stream

        while (!in.atEnd()) {

            // read one line from textstream(separated by "\n")
            QString fileLine = in.readLine();

            // parse the read line into separate pieces(tokens) with "," as the delimiter
            QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
            qDebug()<<lineToken;
            if(0==AddWord(lineToken.at(0),"superuser")){
                failcount++;
            }else{
                succount++;
            }
            lineindex++;
        }
        file.close();
        response=QString("load csv file successfully\nsucceed in %1 words\nfail at %2 words").arg(succount).arg(failcount);
    }else{
        response=QString("open csv file fail");
        qDebug()<<response;
    }
    return response;
}
*/
ClientAccess::~ClientAccess()
{

}
