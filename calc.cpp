#include "calc.h"
#include "ui_calc.h"
#include "shareddata.h"
#include <QDebug>

CALC::CALC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CALC)
{
    ui->setupUi(this);
    displaySharedData();
}

CALC::~CALC()
{
    delete ui;
}

void CALC::displaySharedData()
{
    const auto &inputList = SharedData::instance().inputDataList;
    const auto &resultList = SharedData::instance().calculatedResultList;
    const auto &processList = SharedData::instance().processList;

    QString display;

    for (int i = 0; i < inputList.size(); ++i) {
        display += QString("工序：%1 (%2)\n").arg(processList[i].first).arg(processList[i].second);
        display += "输入数据：\n";
        for (const auto &key : inputList[i].keys()) {
            display += QString("  %1: %2\n").arg(key).arg(inputList[i][key]);
        }

        display += "计算结果：\n";
        for (const auto &key : resultList[i].keys()) {
            display += QString("  %1: %2\n").arg(key).arg(resultList[i][key]);
        }

        display += "\n";
    }

    ui->textBrowser->setText(display);
}
