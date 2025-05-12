//待定
#include "widget.h"
#include "ui_widget.h"
#include "choose.h"
//#include "datalayer.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //隐藏当前页面
    this->hide();
    //显示轴套类页面
    CHOOSE *c = new CHOOSE;
    c->show();
}
