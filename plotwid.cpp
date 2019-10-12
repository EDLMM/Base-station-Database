#include "plotwid.h"
#include "ui_plotwid.h"

plotWid::plotWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plotWid)
{
    ui->setupUi(this);
}

plotWid::~plotWid()
{
    delete ui;
}
