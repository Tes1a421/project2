#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>
#include <QMap>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class CHOOSE; }
QT_END_NAMESPACE

class CHOOSE : public QDialog
{
    Q_OBJECT
public:
    explicit CHOOSE(QWidget *parent = nullptr);
    ~CHOOSE();

    // 对外统一计算接口（供 INPUT 等调用）
    QMap<QString,double> calculateData(const QMap<QString,double>& mergedInputs);

private slots:
    void onProcessButtonClicked();
    void on_deleteButton_clicked();
    void on_pushButton_35_clicked();
    void on_pushButton_36_clicked();

private:
    void addProcess(const QString &name);
    void receivedata(const QMap<QString,double>& data,int index);
    void updateProcessListDisplay();
    void saveToExcel(const QString &file);

    // 包装 util 函数（只有一份声明，避免重复）
    double calcGxx(int oddRow,const QMap<QString,double>& data);
    double calcBFxx(int evenRow,const QMap<QString,double>& data);

    Ui::CHOOSE *ui;
};

#endif // CHOOSE_H
