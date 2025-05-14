#ifndef INPUT_H
#define INPUT_H

#include <QWidget>
#include <QMap>


namespace Ui {
class INPUT;
}

class INPUT : public QWidget
{
    Q_OBJECT

public:
    explicit INPUT(QWidget *parent = nullptr);
    ~INPUT();
    void setPreviousResults(const QMap<QString,double>& d);
    QMap<QString, double> getData();        //获取数据
    void displayResults(const QMap<QString, double>& results);
    QMap<QString, double> calculateData(const QMap<QString, double>& inputData);
signals:
    void dataEntered(QMap<QString, double> data);   //信号：数据输入完成 
private slots:
    void on_concernButtom_clicked();

    void on_cancelButton_clicked();

    void syncX7ToXYZ9(const QString& txt);


private:
    Ui::INPUT *ui;
private:
    QMap<QString, double> previousResults;

};

#endif // INPUT_H
