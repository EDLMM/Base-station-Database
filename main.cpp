#include "mainwindow.h"
#include <QtWidgets/QApplication>

#include "accountwindow.h"
#include "clientend.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientAccess CA;
    AccountWindow aw(nullptr, &CA);
    aw.show();
    MainWindow w(nullptr, &CA);
//    w.show();
    //信号与槽之间进行关联
    QObject::connect(&aw,SIGNAL(showmainwindow()),&w,SLOT(receivelogin()));
//    QObject::connect(&aw,SIGNAL(showchanllengeboard()),&cb,SLOT(receiveshow()));
//    QObject::connect(&aw,SIGNAL(cbsetup()),&cb,SLOT(setup()));

//    QObject::connect(&rw,SIGNAL(logout()),&aw,SLOT(receiveshow()));
//    QObject::connect(&rw,SIGNAL(showvocaboard()),&vb,SLOT(receiveshow()));

//    QObject::connect(&vb,SIGNAL(logout()),&aw,SLOT(receiveshow()));
//    QObject::connect(&vb,SIGNAL(showsearchwindow()),&rw,SLOT(receiveshow()));
//    QObject::connect(&vb,SIGNAL(showchanllengeboard()),&cb,SLOT(receiveshow()));

//    QObject::connect(&cb,SIGNAL(logout()),&aw,SLOT(receiveshow()));
//    QObject::connect(&cb,SIGNAL(showsearchwindow()),&rw,SLOT(receiveshow()));
//    QObject::connect(&cb,SIGNAL(showvocabularyboard()),&vb,SLOT(receiveshow()));

//    qDebug()<<"here1";
//    QObject::connect(&rw,SIGNAL(showonbt()),&olcb,SLOT(receiveshow()));
//    qDebug()<<"here2";
//    QObject::connect(&olcb,SIGNAL(endGameAndBack()),&rw,SLOT(restartTimer()));

    return a.exec();
}
