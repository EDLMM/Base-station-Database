#ifndef CLIENTEND_H
#define CLIENTEND_H
#include <QWidget>
#include <QString>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include <QCryptographicHash>
#include <QRegExpValidator>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QTcpSocket>

class ClientAccess
{
public:
    ClientAccess();
    ~ClientAccess();

    bool execSQL(QString sentence);
    bool Register(QString username,QString password);
    int LoginValid(QString username,QString password);
    QList<QString> SearchColumn(QString tablename="users",int isunique=0,QString type="all",QString attribute="");
    QVector<double> SearchColumn(QString tablename,QString type,QString attribute,QString searchvalue,QString S,QString E);
    QString execSuperSQL(QString query);
    QString loadCSV(QString filename="TOEFL核心词汇21天突破.CSV");

    QString username,password;
    QString releaseDebug;
    QSqlDatabase udb;
private:

};

#endif // CLIENTEND_H
