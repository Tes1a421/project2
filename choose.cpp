// === choose.cpp ===
#include "choose.h"
#include "ui_choose.h"
#include "input.h"
#include "shareddata.h"
#include "process_calc.h"
#include <QFileInfo>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>

#include <QUrl>

#include "xlsxdocument.h"     // <-- 已有就保留
// using namespace QXlsx;     // 如果选“省前缀”写法就取消注释
// ----------------------------------------
// 全局容器（保持和 SharedData 内容同步）
static QList<ProcessEntry> g_processes;

// ----------------------------------------
CHOOSE::CHOOSE(QWidget *parent) : QDialog(parent), ui(new Ui::CHOOSE)
{
    ui->setupUi(this);

    // 绑定所有工序按钮到一个槽
    for (auto *btn : findChildren<QPushButton*>()) {
        const QString id = btn->objectName();
        if (id.contains("deleteButton") || id.contains("pushButton_36")
            || id.contains("pushButton_35") || id.contains("pushButton_34"))
            continue;
        connect(btn,&QPushButton::clicked,this,&CHOOSE::onProcessButtonClicked);
    }
}
CHOOSE::~CHOOSE() { delete ui; }

// ----------------------------------------
void CHOOSE::onProcessButtonClicked()
{
    auto *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    // 新增工序条目
    addProcess(btn->text());

    // 打开输入窗口
    INPUT *dlg = new INPUT;
    if (!g_processes.isEmpty())
        dlg->setPreviousResults(g_processes.last().calculatedData);
    connect(dlg,&INPUT::dataEntered,this,
            [this](const QMap<QString,double>& data){
                receivedata(data, g_processes.size()-1);
            });
    dlg->show();
}

// ----------------------------------------
void CHOOSE::addProcess(const QString &name)
{
    const int seq = (g_processes.size()+1)*10;      // 10,20,30…
    SharedData::instance().processList.append({name,seq});

    ProcessEntry pe;  pe.name=name;  pe.number=seq;
    g_processes.append(pe);

    ui->listWidget->addItem(QString("%1. %2").arg(seq).arg(name));
}

// ----------------------------------------
void CHOOSE::receivedata(const QMap<QString,double> &data,int idx)
{
    if (idx<0 || idx>=g_processes.size()) return;

    g_processes[idx].inputData = data;

    QMap<QString,double> merged = data;
    if (idx>0) merged.unite(g_processes[idx-1].calculatedData);   // 耦合

    g_processes[idx].calculatedData = calculateData(merged);

    // 也同步回 SharedData（可选）
    SharedData::instance().inputDataList.append(data);
    SharedData::instance().calculatedResultList.append(g_processes[idx].calculatedData);
}

// ----------------------------------------
// 计算整条工序 (19-37)
QMap<QString,double> CHOOSE::calculateData(const QMap<QString,double> &in)
{
    QMap<QString,double> res;
    QMap<QString,double> all = in;           // 拷贝一份
    for(int odd=19; odd<=37; odd+=2)
        buildProcessWeights(odd,all,in);     // 生成 AE…BE , 放入 all

    // 接着算 BF、G … 里都用 all


    // 先算 BF 偶数行，让后续 G 可以直接用
    for (int r=20; r<=38; r+=2)
        res[QString("BF%1").arg(r)] = calcBFxx(r, in);

    // 合并一下，给 G 使用
    QMap<QString,double> mix = in;  mix.unite(res);

    for (int r=19; r<=37; r+=2)
        res[QString("G%1").arg(r)] = calcGxx(r, mix);

    return res;
}

// ----------------------------------------
double CHOOSE::calcGxx(int oddRow,const QMap<QString,double>& d) { return ::calcGxx(oddRow,d); }
double CHOOSE::calcBFxx(int evenRow,const QMap<QString,double>& d){ return ::calcBFxx(evenRow,d); }

// ----------------------------------------
// 删除当前工序
void CHOOSE::on_deleteButton_clicked()
{
    const int row = ui->listWidget->currentRow();
    if (row<0 || row>=g_processes.size()) return;

    g_processes.removeAt(row);
    SharedData::instance().processList.removeAt(row);
    if (row < SharedData::instance().inputDataList.size())
        SharedData::instance().inputDataList.removeAt(row);
    if (row < SharedData::instance().calculatedResultList.size())
        SharedData::instance().calculatedResultList.removeAt(row);

    // 重建列表并重新编号
    ui->listWidget->clear();
    for (int i=0;i<g_processes.size();++i) {
        g_processes[i].number = (i+1)*10;
        SharedData::instance().processList[i].second = g_processes[i].number;
        ui->listWidget->addItem(QString("%1. %2")
            .arg(g_processes[i].number).arg(g_processes[i].name));
    }
}

// ----------------------------------------
// 导出 Excel  (只导出 G19-G37，与固定名称)
void CHOOSE::on_pushButton_36_clicked()
{
    if (g_processes.isEmpty()) {
        QMessageBox::information(this,tr("提示"),tr("没有可导出的数据"));
        return;
    }
    const QString file = QDir::toNativeSeparators(
                QCoreApplication::applicationDirPath()+"/process_results.xlsx");
    saveToExcel(file);

    if (QMessageBox::question(this,tr("导出完成"),
        tr("已导出到：\n%1\n\n现在打开所在文件夹？").arg(file),
        QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(file).absolutePath()));
    }
}

// ----------------------------------------
void CHOOSE::saveToExcel(const QString &file)
{
    static const QStringList kLabels = {
        tr("同轴度"), tr("半径波动"), tr("圆柱度"), tr("平面度"), tr("垂直度"),
        tr("轮廓度"), tr("椭圆度"), tr("平行度"), tr("齿形")
    };
    static const QStringList kGkeys = { "G19","G21","G23","G25","G27",
                                        "G29","G31","G33","G35","G37" };

    QXlsx::Document xlsx;
    xlsx.write(1,1,tr("序号"));
    xlsx.write(1,2,tr("工序名称"));
    xlsx.write(1,3,tr("误差名称 : G(μm)"));

    for (int i=0;i<g_processes.size();++i){
        const auto &pe = g_processes[i];
        xlsx.write(i+2,1,pe.number);
        xlsx.write(i+2,2,pe.name);

        QStringList pairs;
        for (int k=0;k<kGkeys.size();++k){
            const QString &gKey = kGkeys[k];
            if (k >= kLabels.size()) break;           // G37 对应“齿形”
            const QString label = kLabels[k];
            const QString val = pe.calculatedData.contains(gKey)
                    ? QString::number(pe.calculatedData[gKey])
                    : QStringLiteral("N/A");
            pairs << label + ":" + val;
        }
        xlsx.write(i+2,3,pairs.join("; "));
    }
    xlsx.saveAs(file);
}

// ----------------------------------------
void CHOOSE::on_pushButton_35_clicked()
{
    /* 若后续需要，可在此 new CALC(this)->show(); */
}

/* --- 唯一静态实现 --- */
QMap<QString,double> CHOOSE::calculateDataStatic(const QMap<QString,double>& d)
{
    CHOOSE tmp;          // 借用成员版本
    return tmp.calculateData(d);
}
