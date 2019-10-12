#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
//#include <QApplication>
//#include <QMainWindow>
#include "customtablemodel.h"
#include <QTableView>
#include "clientend.h"
#include "plotwid.h"

#include <QTableWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QAxObject>

#include <QProgressDialog>

#include <QDir>
#include <QScopedPointer>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, ClientAccess *CA=nullptr);
    ~MainWindow();

    void loadTale2Model(QSqlQueryModel * model,QString tableName);
    void viewTable(QSqlQueryModel * model,QString tableName,QTableView *tbview);
    void viewCustomTable(QSqlQueryModel * model,QString qsentence,QTableView *tbview);
    void hideComponents();
    void setPlotView(CustomTableModel * model,QString lineName,QVector<double>x,QVector<double>y);

    void Table2ExcelByHtml(QTableView *table,QString title);
    bool ExcelToCsvFile(const QString &excelFileName, const QString &csvFileName);
private slots:
    void receivelogin();
    void on_actionlistUser_triggered();
    void on_actiontbCell_triggered();
    void on_actiontbKPI_triggered();
    void on_viewKPI_clicked();
    void on_B_cell_clicked();
    void on_B_node_clicked();

    void on_B_save_clicked();

    void on_actiontbC2Inew_triggered();

    void on_B_gen_clicked();

    void on_actiontbC2I3_triggered();

    void on_actiontbPRBnew_triggered();

    void on_B_gen_2_clicked();

    void on_actiontbPRB_triggered();

    void on_B_PRB_1_clicked();

    void on_actiontbCell_3_triggered();

    void on_actiontbKPI_2_triggered();

    void on_actiontbPRB_2_triggered();

    void on_actiontbMROData_2_triggered();

    void importFile(QString tablename);

    void updateProcess(QProgressDialog *pd);

signals:
//    void awshow();
    void quit();

private:
     QTimer *timer ;

    Ui::MainWindow *ui;
    ClientAccess *CA;
    QSqlQueryModel *CELLmodel ;
    QSqlQueryModel *KPImodel ;
    QSqlQueryModel *DIYmodel ;

    CustomTableModel *cosKPImodel;

    //折线图
    QLineSeries *series;
    QChartView *chartView;
    QChart *chart;
    plotWid *viewP;

};



#endif // MAINWINDOW_H
