#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>
#include <QMap>
#include <QList>
#include <QString>

namespace Ui { class CHOOSE; }

struct ProcessEntry           // 仅在头文件暴露，供调试或其他模块查看
{
    QString name;
    int     number;           // 10、20、30…
    QMap<QString,double> inputData;
    QMap<QString,double> calculatedData;
};



class CHOOSE : public QDialog
{
    Q_OBJECT
public:
    explicit CHOOSE(QWidget *parent = nullptr);
    ~CHOOSE() override;
    /* 计算一整条工序 (G/BF)；任何模块都可直接调用 */
    QMap<QString,double> calculateData(const QMap<QString,double>& in);
    static QMap<QString,double> calculateDataStatic(const QMap<QString,double>& d);

private slots:
    void onProcessButtonClicked();   // 左侧所有工序按钮
    void on_deleteButton_clicked();  // 删除当前工序
    void on_pushButton_36_clicked(); // 导出 Excel
    void on_pushButton_35_clicked(); // 打开 CALC（如果有）

private:
    void          addProcess(const QString &name);
    void          receivedata(const QMap<QString,double>& data,int idx);


    // --- 公式核心在 process_calc.cpp ---
    static double calcGxx(int oddRow,const QMap<QString,double>& d);
    static double calcBFxx(int evenRow,const QMap<QString,double>& d);

    void saveToExcel(const QString& file);

private:
    Ui::CHOOSE *ui;
};

#endif // CHOOSE_H
