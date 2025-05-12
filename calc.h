#ifndef CALC_H
#define CALC_H

#include <QWidget>

namespace Ui {
class CALC;
}

class CALC : public QWidget
{
    Q_OBJECT

public:
    explicit CALC(QWidget *parent = nullptr);
    ~CALC();

private:
    Ui::CALC *ui;
    void displaySharedData();
};

#endif // CALC_H
