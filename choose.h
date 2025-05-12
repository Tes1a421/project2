#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>
#include <QMap>
#include <QListWidgetItem>

namespace Ui {
class CHOOSE;
}

class CHOOSE : public QDialog
{
    Q_OBJECT

public:
    explicit CHOOSE(QWidget *parent = nullptr);
    ~CHOOSE();

private slots:
    void onProcessButtonClicked();  // 所有工序按钮共用

    void receivedata(const QMap<QString, double> &data, int index);

    void on_deleteButton_clicked();

    void on_pushButton_36_clicked();  // 处理并保存Excel

    void on_pushButton_35_clicked();

private:
    Ui::CHOOSE *ui;
    QList<QMap<QString, double>> storedData;
    QList<QMap<QString, double>> processedData;
    int currentProcessNumber = 10;
    double calcBFxx(int row, const QMap<QString, double>& data);
    QList<QPair<QString, int>> itemNamesAndIndexes;
    QMap<QString, double> calculateData(const QMap<QString, double> &inputData);
    double calcBF20(const QMap<QString, double>& data, double BF19, double BF17, double previousG19);
    void addProcess(const QString &name);
    void saveToExcel(const QString &filename);
    void updateProcessListDisplay();
    double calcGxx(int row, const QMap<QString, double>& data);
};

#endif // CHOOSE_H
