// === choose.cpp ===
#include "choose.h"
#include "ui_choose.h"
#include "input.h"
#include "xlsxdocument.h"
#include "xlsxworkbook.h"
#include <QDebug>
#include <cmath>
#include "shareddata.h"
#include "process_calc.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>
#include <QUrl>

struct ProcessEntry {
    QString name;
    int number;
    QMap<QString,double> inputData;
    QMap<QString,double> calculatedData;
};
static QList<ProcessEntry> processEntries;

// -----------------------------------------------------------------------------
CHOOSE::CHOOSE(QWidget *parent) : QDialog(parent), ui(new Ui::CHOOSE)
{
    ui->setupUi(this);
    for(auto *btn: findChildren<QPushButton*>()){
        QString id=btn->objectName();
        if(id.contains("deleteButton")||id.contains("pushButton_36")||id.contains("pushButton_35")||id.contains("pushButton_34"))
            continue;
        connect(btn,&QPushButton::clicked,this,&CHOOSE::onProcessButtonClicked);
    }
}
CHOOSE::~CHOOSE(){ delete ui; }

// -----------------------------------------------------------------------------
void CHOOSE::onProcessButtonClicked()
{
    auto *btn=qobject_cast<QPushButton*>(sender());
    if(!btn) return;
    addProcess(btn->text());

    INPUT *dlg=new INPUT;
    if(!processEntries.isEmpty())
        dlg->setPreviousResults(processEntries.last().calculatedData);

    connect(dlg,&INPUT::dataEntered,this,[=](const QMap<QString,double>& in){
        receivedata(in,processEntries.size()-1);
    });
    dlg->show();
}

// -----------------------------------------------------------------------------
void CHOOSE::addProcess(const QString &name)
{
    int idx=SharedData::instance().processList.size();
    int num=(idx+1)*10;
    SharedData::instance().processList.append({name,num});
    ui->listWidget->addItem(QString("%1. %2").arg(num).arg(name));

    ProcessEntry pe; pe.name=name; pe.number=num;
    processEntries.append(pe);
}

// -----------------------------------------------------------------------------
void CHOOSE::receivedata(const QMap<QString,double>& data,int idx)
{
    if(idx<0||idx>=processEntries.size()) return;
    processEntries[idx].inputData=data;
    QMap<QString,double> merged=data;
    if(idx>0) merged.unite(processEntries[idx-1].calculatedData);
    processEntries[idx].calculatedData=calculateData(merged);
}

// -----------------------------------------------------------------------------
// 计算一条工序(19-37)全部数据 ---------------------------
QMap<QString,double> CHOOSE::calculateData(const QMap<QString,double>& in)
{
    bool hasPrev = in.contains("prev_flag");     // 我们在 receivedata() 手动塞一个标记
    QMap<QString,double> prev = hasPrev ? processEntries.last().calculatedData
                                        : QMap<QString,double>();

    QMap<QString,double> res = in;               // 先拷贝，方便查值
    // —— BF 奇数
    for(int r=19;r<=37;r+=2) res["BF"+QString::number(r)] = BF_odd(r,res);
    // —— G 奇数（用闭式解，不依赖 BF 偶数）
    for(int r=19;r<=37;r+=2) res["G" +QString::number(r)] = G_odd(r,res);
    // —— BF 偶数
    for(int r=20;r<=38;r+=2)
        res["BF"+QString::number(r)] = BF_even(r,res,hasPrev,prev);

    // 把权重行 17 也存进去（以后 20 行权重如有真公式再补）
    for(QString col: {"AE","AF","AG","AH","AI","AJ","AK","AL","AM","AN",
                      "AO","AP","AQ","AR","AS","AT","AU","AV","AW","AX",
                      "AY","AZ","BA","BB","BC","BD","BE"})
        res[col+"17"] = weight17(in,col);

    return res;
}

// -----------------------------------------------------------------------------
// Excel export helper triggered by pushButton_36
void CHOOSE::on_pushButton_36_clicked()
{
    if(processEntries.isEmpty()) return;
    saveToExcel("process_results.xlsx");
}
// -----------------------------------------------------------------------------
// 只导出：序号、工序名称，以及 “固定误差名称 : G 值” 列
void CHOOSE::saveToExcel(const QString &file)
{
    // 固定的 10 个标签，对应 G19,G21,…,G35，G37
    static const QStringList kLabels = {
        QStringLiteral("圆度"),
        QStringLiteral("同轴度"),
        QStringLiteral("半径波动"),
        QStringLiteral("圆柱度"),
        QStringLiteral("平面度"),
        QStringLiteral("垂直度"),
        QStringLiteral("轮廓度"),
        QStringLiteral("椭圆度"),
        QStringLiteral("平行度"),
        QStringLiteral("齿形")
        // 如需第 10 行 E37/G37，在此再写一个字符串
    };

    const QStringList gKeys = { "G19","G21","G23","G25","G27",
                                "G29","G31","G33","G35","G37" };   // 与标签一一对应

    QXlsx::Document xlsx;
    xlsx.write(1,1,"序号");
    xlsx.write(1,2,"工序名称");
    xlsx.write(1,3,"误差名称 : G 值 (μm)");

    for (int i = 0; i < processEntries.size(); ++i) {
        const auto &pe = processEntries[i];
        xlsx.write(i+2, 1, pe.number);
        xlsx.write(i+2, 2, pe.name);

        QStringList pairs;
        for (int k = 0; k < gKeys.size(); ++k) {
            const QString &g = gKeys[k];
            const QString &label = kLabels[k];          // 固定名称

            QString gVal = pe.calculatedData.contains(g)
                         ? QString::number(pe.calculatedData.value(g))
                         : "N/A";

            pairs << QString("%1:%2").arg(label, gVal);
        }
        xlsx.write(i+2, 3, pairs.join("; "));
    }
    xlsx.saveAs(file);

    // 弹窗并可打开文件夹
    if (QMessageBox::question(this, tr("导出完成"),
        tr("已导出到:\n%1\n\n现在打开所在文件夹？")
            .arg(QDir::toNativeSeparators(file)),
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(
            QUrl::fromLocalFile(QFileInfo(file).absolutePath()));
    }
}
// -----------------------------------------------------------------------------
double CHOOSE::calcGxx(int oddRow,const QMap<QString,double>& d)
{
    return ::calcGxx(oddRow,d);   // 调到 process_calc.cpp 全局实现
}

double CHOOSE::calcBFxx(int evenRow,const QMap<QString,double>& d)
{
    return ::calcBFxx(evenRow,d); // 同上
}
// ------------------------- 删除当前工序 -------------------------
void CHOOSE::on_deleteButton_clicked()
{
    int row = ui->listWidget->currentRow();
    if (row < 0 || row >= processEntries.size())
        return;

    // 从本地容器与 SharedData 同步删除
    processEntries.removeAt(row);
    SharedData::instance().processList.removeAt(row);
    if (row < SharedData::instance().inputDataList.size())
        SharedData::instance().inputDataList.removeAt(row);
    if (row < SharedData::instance().calculatedResultList.size())
        SharedData::instance().calculatedResultList.removeAt(row);

    // 重新编号 & 重绘列表
    ui->listWidget->clear();
    for (int i = 0; i < processEntries.size(); ++i) {
        processEntries[i].number   = (i + 1) * 10;
        SharedData::instance().processList[i].second = processEntries[i].number;
        ui->listWidget->addItem(
            QString("%1. %2").arg(processEntries[i].number)
                              .arg(processEntries[i].name));
    }
}
// ------------------------- 打开 CALC 页面（占位） -------------------------
void CHOOSE::on_pushButton_35_clicked()
{
    /* 如果你有 CALC 对话框就 new CALC(this)->show();
       否则保持空函数只为解决链接错误 */
}
