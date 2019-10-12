#ifndef PLOTWID_H
#define PLOTWID_H

#include <QWidget>

namespace Ui {
class plotWid;
}

class plotWid : public QWidget
{
    Q_OBJECT

public:
    explicit plotWid(QWidget *parent = nullptr);
    ~plotWid();

private:
    Ui::plotWid *ui;
};

#endif // PLOTWID_H
