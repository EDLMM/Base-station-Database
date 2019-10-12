#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent, ClientAccess *CA) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    this->CA=CA;
    this->KPImodel = new QSqlQueryModel;
    this->cosKPImodel = new CustomTableModel;
    this->CELLmodel = new QSqlQueryModel;
    this->DIYmodel = new QSqlQueryModel;

    //预置下拉菜单
    QList<QString> allCellID=CA->SearchColumn("tbCell",1,"all","SECTOR_ID");
    QList<QString> allENodeBCELL=CA->SearchColumn("tbCell",1,"all","ENODEBID");
    QList<QString> allENodeBKPI=CA->SearchColumn("tbKPI",1,"all","ENODEB_NAME");
    QList<QString> PRBENodeBKPI=CA->SearchColumn("tbPRB",1,"all","ENODEB_NAME");
    for (int i=0;i<allENodeBKPI.size();i++)
    {
        ui->comboBoxKPI->addItem(allENodeBKPI.at(i));
    }
    for (int i=0;i<allCellID.size();i++)
    {
        ui->comboBoxCell->addItem(allCellID.at(i));

    }
    for (int i=0;i<allENodeBCELL.size();i++)
    {
        ui->comboBoxNode->addItem(allENodeBCELL.at(i));
    }
    for (int i=0;i<PRBENodeBKPI.size();i++)
    {
        ui->comboBoxKPI_2->addItem(PRBENodeBKPI.at(i));
    }
    ui->comboBoxKPI_p->addItem(QString("RRC建立成功率qf"));
    ui->comboBoxKPI_p->addItem(QString("ERAB建立成功率2"));
    ui->comboBoxKPI_p->addItem(QString("ERAB掉线率"));
    ui->comboBoxKPI_p->addItem(QString("无线接通率ay"));
    ui->comboBoxKPI_p->addItem(QString("无线掉线率"));

    ui->comboBoxKPI_DAY->addItem(QString("2016-07-17"));
    ui->comboBoxKPI_DAY->addItem(QString("2016-07-18"));
    ui->comboBoxKPI_DAY->addItem(QString("2016-07-19"));

    for (int i=0;i<100;i++)
    {
        ui->comboBoxKPI_p_2->addItem(QString("AVG_%1").arg(i));
    }

    ui->comboBoxKPI->setEditable(true);
    ui->comboBoxCell->setEditable(true);
    ui->comboBoxNode->setEditable(true);
    ui->comboBoxKPI_p->setEditable(true);
    ui->comboBoxKPI_p_2->setEditable(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProcess()));//QProgressDialog *pd

    hideComponents();
}
void MainWindow::loadTale2Model(QSqlQueryModel * model,QString tableName)
{
    model->clear();
    if (CA->udb.isValid())
    {
        CA->udb.open();
        if (CA->udb.isOpen())
        {
            QSqlQuery searchQuery(CA->udb);
            searchQuery.prepare(QString("SELECT * FROM %1;").arg(tableName));
            searchQuery.exec();
            if(searchQuery.isActive())
            {
                model->setQuery(searchQuery);
            } else {
                qDebug() << "query is not active";
            }
        } else {
            qDebug() << "DB is not open";
        }
    } else {
        qDebug() << "DB is not valid";
    }
}

void MainWindow::viewTable(QSqlQueryModel * model,QString tableName,QTableView *tbview)
{
    tbview->setModel(model);
    //隔行变色
    tbview->setAlternatingRowColors(true);
    tbview->resizeColumnsToContents();
}


void MainWindow::viewCustomTable(QSqlQueryModel * model,QString qsentence,QTableView *tbview)
{
    if (CA->udb.isValid())
    {
        CA->udb.open();
        if (CA->udb.isOpen())
        {
            QSqlQuery searchQuery(CA->udb);
            searchQuery.prepare(qsentence);
            searchQuery.exec();
            if(searchQuery.isActive())
            {
                model->setQuery(searchQuery);
                tbview->setModel(model);
                //隔行变色
                tbview->setAlternatingRowColors(true);
                tbview->resizeColumnsToContents();
            } else {
                qDebug() << "query is not active";
            }
        } else {
            qDebug() << "DB is not open";
        }
    } else {
        qDebug() << "DB is not valid";
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receivelogin()
{
    this->show();
}

void MainWindow::on_actionlistUser_triggered()
{
    viewTable(CELLmodel,"users",ui->tableView);

}

void MainWindow::hideComponents()
{
    ui->customPlot->hide();
    ui->tableView->hide();
    ui->comboBoxCell->hide();
    ui->comboBoxNode->hide();
    ui->B_node->hide();
    ui->B_cell->hide();
    ui->comboBoxKPI->hide();
    ui->comboBoxKPI_p->hide();
    ui->viewKPI->hide();
    ui->dateEdit_1->hide();
    ui->dateEdit_2->hide();
    ui->datelabel1->hide();
    ui->datelabel2->hide();
    ui->B_gen->hide();
    ui->genTips->hide();
    ui->genPara->hide();
    ui->B_gen_2->hide();
    ui->PRBlabel->hide();
    ui->comboBoxKPI_2->hide();
    ui->comboBoxKPI_p_2->hide();
    ui->dateTimeEdit_1->hide();
    ui->dateTimeEdit_2->hide();
    ui->comboBoxKPI_DAY->hide();
    ui->B_PRB_1->hide();

}

void MainWindow::setPlotView(CustomTableModel * model,QString lineName,QVector<double>x,QVector<double>y)
{
    double minY=y.at(0);
    double maxY=y.at(0);
    QVector<double>::iterator iter;
        for (iter=y.begin();iter!=y.end();iter++)
        {
            //qDebug() <<  *iter << "\0"；
            if(*iter>maxY){
                maxY=*iter;
            }
          if(*iter<minY){
              minY=*iter;
          }
        }

    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("选定范围内时间序列点");
    ui->customPlot->yAxis->setLabel(lineName);
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, x.size());
    ui->customPlot->yAxis->setRange(minY, maxY);
    ui->customPlot->replot();
    ui->customPlot->saveJpg("temp.jpg");
}
void MainWindow::on_actiontbCell_triggered()
{
    hideComponents();
    ui->tableView->show();
    ui->comboBoxCell->show();
    ui->comboBoxNode->show();
    ui->B_node->show();
    ui->B_cell->show();
    loadTale2Model(CELLmodel,"tbCell");
    viewTable(CELLmodel,"tbCell",ui->tableView);
}

void MainWindow::on_B_cell_clicked()
{
        QString sq=QString("SELECT * FROM tbCell WHERE SECTOR_ID='%1';").arg(ui->comboBoxCell->currentText());
        DIYmodel->clear();
        viewCustomTable(CELLmodel,sq,ui->tableView);
}

void MainWindow::on_B_node_clicked()
{
    QString sq=QString("SELECT * FROM tbCell WHERE ENODEBID='%1';").arg(ui->comboBoxNode->currentText());
    DIYmodel->clear();
    viewCustomTable(DIYmodel,sq,ui->tableView);
}


void MainWindow::on_actiontbKPI_triggered()
{
    hideComponents();
    ui->tableView->show();
    ui->datelabel1->show();
    ui->datelabel2->show();
    ui->comboBoxKPI->show();
    ui->comboBoxKPI_p->show();
    ui->viewKPI->show();
    ui->dateEdit_1->show();
    ui->dateEdit_2->show();
//TODO 恢复
    loadTale2Model(KPImodel,"tbKPI");
    viewTable(KPImodel,"tbKPI",ui->tableView);
}

void MainWindow::on_viewKPI_clicked()
{
    QString eb(ui->comboBoxKPI->currentText());
    QString selectP(ui->comboBoxKPI_p->currentText());
    QString start=ui->dateEdit_1->dateTime().toString("yyyy-MM-dd");
    QString end=ui->dateEdit_2->dateTime().toString("yyyy-MM-dd");

    QVector<double> x,y;

    QString q;
    q=QString("select %1 from tbKPI where STATRTTIME>= '%2' and STATRTTIME <= '%3' and ENODEB_NAME='%4' order by STATRTTIME, %1;").arg(selectP).arg(start).arg(end).arg(eb);
    QSqlQuery query(CA->udb);
    double i=0;
    qDebug()<<"running: "<<q;
    query.exec(q);
    while(query.next()){
        qDebug()<<"r get "<<query.value(0).toDouble();
        y.append(query.value(0).toDouble());
        x.append(i);
        i++;
    }
    setPlotView(cosKPImodel,selectP,x,y);
    ui->customPlot->show();
}


void MainWindow::on_actiontbC2Inew_triggered()
{
    this->hideComponents();
    ui->genTips->setText("请输入参数floor\n以生成表格tbC2Inew");
    ui->B_gen->show();
    ui->B_gen_2->show();
    ui->genTips->show();
    ui->genPara->show();
       ui->tableView->show();
}


void MainWindow::on_actiontbC2I3_triggered()
{
    this->hideComponents();
    ui->genTips->setText("请输入参数floor\n以生成表格tbC2I3");
    ui->B_gen->show();
    ui->B_gen_2->show();
    ui->genTips->show();
    ui->genPara->show();
    ui->tableView->show();

}
void MainWindow::on_actiontbPRBnew_triggered()
{
    this->hideComponents();
    ui->genTips->setText("请按按钮\n以生成表格tbPRBnew");
    ui->B_gen->show();
    ui->B_gen_2->show();
    ui->genTips->show();
    ui->tableView->show();
}
void MainWindow::on_B_gen_clicked()
{
     QString q;
     QSqlQuery query(CA->udb);
     if("请输入参数floor\n以生成表格tbC2Inew"==ui->genTips->text()){
        q=QString("exec gen_c2i %1").arg(ui->genPara->text());
    }else if("请输入参数floor\n以生成表格tbC2I3"==ui->genTips->text()){
        q=QString("exec gen_c2i %1").arg(ui->genPara->text());
    }else if("请按按钮\n以生成表格tbPRBnew"==ui->genTips->text()){
        q=QString("exec gen_c2i");
    }
    qDebug()<<"running: "<<q;
    query.exec(q);
}

void MainWindow::on_actiontbPRB_triggered()
{
    hideComponents();
    ui->tableView->show();
    ui->PRBlabel->show();
    ui->comboBoxKPI_2->show();
    ui->comboBoxKPI_p_2->show();
    ui->comboBoxKPI_DAY->show();
    ui->dateTimeEdit_1->show();
    ui->dateTimeEdit_2->show();
    ui->B_PRB_1->show();

//    "调整时间 hh:mm:ss\n开始范围和结束范围"
}
void MainWindow::on_B_gen_2_clicked()
{
    if("请输入参数floor\n以生成表格tbC2Inew"==ui->genTips->text()){
       loadTale2Model(DIYmodel,"tbC2Inew");
       viewTable(DIYmodel,"tbC2Inew",ui->tableView);
   }else if("请输入参数floor\n以生成表格tbC2I3"==ui->genTips->text()){
       loadTale2Model(DIYmodel,"tbC2I3");
       viewTable(DIYmodel,"tbC2I3",ui->tableView);
   }else if("请按按钮\n以生成表格tbPRBnew"==ui->genTips->text()){
       loadTale2Model(DIYmodel,"tbPRBnew");
       viewTable(DIYmodel,"tbPRBnew",ui->tableView);
   }
}

void MainWindow::on_B_PRB_1_clicked()
{
        QString eb(ui->comboBoxKPI_2->currentText());
        QString selectP(ui->comboBoxKPI_p_2->currentText());
        QString DATE(ui->comboBoxKPI_DAY->currentText());
        QString start=ui->dateTimeEdit_1->dateTime().toString("HH");
        QString end=ui->dateTimeEdit_2->dateTime().toString("HH");
        QString mstart=ui->dateTimeEdit_1->dateTime().toString("HH:mm");
        QString mend=ui->dateTimeEdit_2->dateTime().toString("HH:mm");

        QVector<double> x,y;
        QString tbname;
        QString q;
        if(start==end){
            tbname=QString("tbPRBnew");
            q=QString("select %1 from tbPRBnew where STATRTTIME>='%2 %3:00.000' and  STATRTTIME<='%2 %4:00.000' and ENODEB_NAME='%5 ';").arg(selectP).arg(DATE).arg(mstart).arg(mend).arg(eb);
        }else{
           tbname=QString("tbPRB");
            q=QString("select %1 from tbPRB where STATRTTIME>='%2 %3:00.000' and  STATRTTIME<='%2 %4:00.000' and ENODEB_NAME='%5 ';").arg(selectP).arg(DATE).arg(start).arg(end).arg(eb);
        }
        QSqlQuery query(CA->udb);
        double i=0;
        qDebug()<<"running: "<<q;
        query.exec(q);
        while(query.next()){
            qDebug()<<"r get "<<query.value(0).toDouble();
            y.append(query.value(0).toDouble());
            x.append(i);
            i++;
        }
        setPlotView(cosKPImodel,QString("%1_%2_%3").arg(eb).arg(selectP).arg(DATE),x,y);
        ui->customPlot->show();
}


void MainWindow::on_B_save_clicked()
{

    if(ui->viewKPI->isVisible()){
        ui->customPlot->savePng(QString("%1_%2_%3.png").arg(ui->comboBoxKPI->currentText()).arg(ui->dateEdit_1->dateTime().toString("yyyy-MM-dd")).arg(ui->dateEdit_2->dateTime().toString("yyyy-MM-dd")));
    }
    if(ui->PRBlabel->isVisible()){
        qDebug()<<"output line series of tbPRB";
        ui->customPlot->savePng(QString("%1_%2.png").arg(ui->comboBoxKPI_p_2->currentText()).arg(ui->comboBoxKPI_DAY->currentText()));
    }
    if(ui->B_cell->isVisible()){
        qDebug()<<"output table of tbCell";
        Table2ExcelByHtml(ui->tableView,QString("tbCell"));
    }
    if(ui->genTips->isVisible()){
        if("请输入参数floor\n以生成表格tbC2Inew"==ui->genTips->text()){
          Table2ExcelByHtml(ui->tableView,QString("tbC2Inew"));
       }else if("请输入参数floor\n以生成表格tbC2I3"==ui->genTips->text()){
          Table2ExcelByHtml(ui->tableView,QString("tbC2I3"));
       }else if("请按按钮\n以生成表格tbPRBnew"==ui->genTips->text()){
          Table2ExcelByHtml(ui->tableView,QString("tbPRBnew"));
       }
    }
}

//第一个参数是页面上的表格，第二个是导出文件的表头数据//QTableWidget *table
void MainWindow::Table2ExcelByHtml(QTableView *tableview,QString title)
 {
     QString fileName = QFileDialog::getSaveFileName(this, "保存",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Excel 文件(*.xls *.xlsx)");
     if (fileName!="")
     {
         QAxObject *excel = new QAxObject;
         if (excel->setControl("Excel.Application")) //连接Excel控件
         {
             excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
             excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
             QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
             workbooks->dynamicCall("Add");//新建一个工作簿
             QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
             QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

             int i,j;
             int colcount=ui->tableView->model()->columnCount();
             //QTableView 获取行数
             int rowcount=ui->tableView->model()->rowCount();

             QAxObject *cell,*col;
             //标题行
             cell=worksheet->querySubObject("Cells(int,int)", 1, 1);
             cell->dynamicCall("SetValue(const QString&)", title);
             cell->querySubObject("Font")->setProperty("Size", 18);
             //调整行高
             worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
             //合并标题行
             QString cellTitle;
             cellTitle.append("A1:");
             cellTitle.append(QChar(colcount - 1 + 'A'));
             cellTitle.append(QString::number(1));
             QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);
             range->setProperty("WrapText", true);
             range->setProperty("MergeCells", true);
             range->setProperty("HorizontalAlignment", -4108);//xlCenter
             range->setProperty("VerticalAlignment", -4108);//xlCenter
             //列标题
             for(i=0;i<colcount;i++)
             {
                 QString columnName;
                 columnName.append(QChar(i  + 'A'));
                 columnName.append(":");
                 columnName.append(QChar(i + 'A'));
                 col = worksheet->querySubObject("Columns(const QString&)", columnName);
                 col->setProperty("ColumnWidth", ui->tableView->columnWidth(i)/6);
                 cell=worksheet->querySubObject("Cells(int,int)", 2, i+1);
                 //QTableWidget 获取表格头部文字信息
//                 columnName=table->horizontalHeaderItem(i)->text();
                 //QTableView 获取表格头部文字信息
                  columnName=ui->tableView->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
                 cell->dynamicCall("SetValue(const QString&)", columnName);
                 cell->querySubObject("Font")->setProperty("Bold", true);
                 cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                 cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                 cell->setProperty("VerticalAlignment", -4108);//xlCenter
             }

            //数据区
            //QTableView 获取表格数据部分
              for(i=0;i<rowcount;i++) //行数
                 {
                     for (j=0;j<colcount;j++)   //列数
                     {
                         QModelIndex index = ui->tableView->model()->index(i, j);
                         QString strdata=ui->tableView->model()->data(index).toString();
                         worksheet->querySubObject("Cells(int,int)", i+3, j+1)->dynamicCall("SetValue(const QString&)", strdata);
                     }
                 }


             //画框线
             QString lrange;
             lrange.append("A2:");
             lrange.append(colcount - 1 + 'A');
             lrange.append(QString::number(rowcount + 2));
             range = worksheet->querySubObject("Range(const QString&)", lrange);
             range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
             range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
             //调整数据区行高
             QString rowsName;
             rowsName.append("2:");
             rowsName.append(QString::number(rowcount + 2));
             range = worksheet->querySubObject("Range(const QString&)", rowsName);
             range->setProperty("RowHeight", 20);
             workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至fileName
             workbook->dynamicCall("Close()");//关闭工作簿
             excel->dynamicCall("Quit()");//关闭excel
             delete excel;
             excel=NULL;
             if (QMessageBox::question(NULL,"完成","文件已经导出，是否现在打开？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
             {
                 QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
             }
         }
         else
         {
             QMessageBox::warning(NULL,"错误","未能创建 Excel 对象，请安装 Microsoft Excel。",QMessageBox::Apply);
         }
     }
 }

bool MainWindow::ExcelToCsvFile(const QString &excelFileName, const QString &csvFileName)
{
    if (!QFile::exists(excelFileName))
        return false;

    // 当pApplication析构的时候会将其所有相关的子对象都清理
    QScopedPointer<QAxObject> pApplication(new QAxObject());

    // 设置连接Excel控件，需要在GUI的环境下才能成功
    bool ok = pApplication->setControl("Excel.Application");
    if (!ok)
        return false;

    pApplication->dynamicCall("SetVisible(bool)", false); // false表示不显示窗体
    pApplication->setProperty("DisplayAlerts", false); // false表示不显示(输出)任何警告信息。
    QAxObject *pWorkBooks = pApplication->querySubObject("Workbooks"); // Excel工作薄(对象)
    if (pWorkBooks == 0)
        return false;

    QAxObject *pWorkBook  = pWorkBooks->querySubObject("Open(const QString &)", excelFileName); // 打开一个Excel文件
    if (pWorkBook == 0)
        return false;

    QAxObject *pSheets = pWorkBook->querySubObject("WorkSheets"); // Excel工作表集
    if (pSheets == 0)
        return false;

    QAxObject *pSheet = pSheets->querySubObject("Item(int)", 1); // 得到指定索引的工作表
    if (pSheet == 0)
        return false;

    // 另存为文件, 3: txt文件（空格分隔）| 6: csv文件（逗号分隔）
    pSheet->dynamicCall("SaveAs(const QString&, int)", QDir::toNativeSeparators(csvFileName), 6);

    pApplication->dynamicCall("Quit()");

    return true;
}
void MainWindow::updateProcess(QProgressDialog *pd)
{
    if(pd->wasCanceled()){
        timer->stop();
        delete(pd);
    }
}

void MainWindow::importFile(QString tablename)
{
    QString fileName = QFileDialog::getOpenFileName(this, "保存",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    int iC=fileName.indexOf(QString(".csv"));
    QString noB;
    QString csvName;
    qDebug()<<fileName;
    //如果不是CSV
    if(-1==iC){
        qDebug()<<"转换为CSV..";
        iC=fileName.indexOf(QString(".xls"));
        noB=fileName.mid(0,iC);
        csvName=noB+QString(".csv");
        ExcelToCsvFile(fileName, csvName);
        qDebug()<<"完成";
    }else{
        csvName=fileName;
    }
    int maxLine=0;
    QFile file(csvName);
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;
         QTextStream in(&file);
         while (!in.atEnd()) {
             QString line = in.readLine();
             maxLine++;
         }
         QProgressDialog process(this);
         process.setLabelText(tr("importing..."));
         process.setRange(0,maxLine);
         process.setModal(true);
         process.setCancelButtonText(tr("cancel"));
         process.setWindowTitle("处理进度");
//        process.exec();
        timer->start(1000);
        for (int rowS=2;rowS<maxLine&&timer->isActive();rowS+=51) {
            qDebug()<<"processing "<<rowS<<" of "<<maxLine;
            process.setValue(rowS);
            CA->execSQL(QString("bulk insert %4 from '%1' with (fieldterminator=',', rowterminator='\n', FIRSTROW =%2, LASTROW=%3,FIRE_TRIGGERS )").arg(csvName).arg(QString::number ( rowS,10 )).arg(QString::number ( rowS+50,10 )).arg(tablename));
        }
}

void MainWindow::on_actiontbCell_3_triggered()
{
    importFile("tbCell");
}

void MainWindow::on_actiontbKPI_2_triggered()
{
    importFile("tbKPI");
}

void MainWindow::on_actiontbPRB_2_triggered()
{
    importFile("tbPRB");
}

void MainWindow::on_actiontbMROData_2_triggered()
{
    importFile("tbMROData");
}
