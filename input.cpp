//做数据输入页面的图形化
#include "input.h"
#include "ui_input.h"
#include "shareddata.h"
#include "choose.h"
#include <QDebug>

INPUT::INPUT(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::INPUT)
{
    ui->setupUi(this);

    connect(ui->X7, &QLineEdit::textChanged, this,  &INPUT::syncX7ToXYZ9);
    syncX7ToXYZ9(ui->X7->text());
    // 设置 C9 - AC9 初始值为 5
    QStringList c9_ac9 = {"C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "AA", "AB", "AC"};
    for (const QString &col : c9_ac9) {
        QString name = QString("%1%2").arg(col).arg(9);  // 比如 C9
        QLineEdit *edit = this->findChild<QLineEdit*>(name);
        if (edit) {
            edit->setText("5");  // 设置默认值为 5
        }
    }
    QStringList ae_be = {
        "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL", "AM", "AN",
        "AO", "AP", "AQ", "AR", "AS", "AT", "AU", "AV", "AW", "AX",
        "AY", "AZ", "BA", "BB", "BC", "BD", "BE"
    };
    for (const QString &col : ae_be) {
        QString name = QString("%1%2").arg(col).arg(17);
        QLineEdit *edit = this->findChild<QLineEdit*>(name);
        if (edit) edit->setText("5");
    }

    QStringList aeToBeCols = {
        "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL", "AM", "AN",
        "AO", "AP", "AQ", "AR", "AS", "AT", "AU", "AV", "AW", "AX",
        "AY", "AZ", "BA", "BB", "BC", "BD", "BE"
    };


    ui->E4->setText("22");
    ui->X7->setText("1");

    // 初始化 I 列 (19-37 奇数行)
    ui->I19->setChecked(false);
    ui->I21->setChecked(false);
    ui->I23->setChecked(false);
    ui->I25->setChecked(false);
    ui->I27->setChecked(false);
    ui->I29->setChecked(false);
    ui->I31->setChecked(false);
    ui->I33->setChecked(false);
    ui->I35->setChecked(false);
    ui->I37->setChecked(true);

    // 初始化 O 列 (19-37 奇数行)
    ui->O19->setChecked(false);
    ui->O21->setChecked(false);
    ui->O23->setChecked(false);
    ui->O25->setChecked(false);
    ui->O27->setChecked(false);
    ui->O29->setChecked(false);
    ui->O31->setChecked(false);
    ui->O33->setChecked(false);
    ui->O35->setChecked(false);
    ui->O37->setChecked(false);

    // 初始化 L 列 (19-37 奇数行)
    ui->L19->setChecked(false);
    ui->L21->setChecked(false);
    ui->L23->setChecked(false);
    ui->L25->setChecked(false);
    ui->L27->setChecked(false);
    ui->L29->setChecked(false);
    ui->L31->setChecked(false);
    ui->L33->setChecked(false);
    ui->L35->setChecked(false);
    ui->L37->setChecked(false);

    // 初始化 R 列 (19-37 奇数行)
    ui->R19->setChecked(false);
    ui->R21->setChecked(false);
    ui->R23->setChecked(false);
    ui->R25->setChecked(false);
    ui->R27->setChecked(false);
    ui->R29->setChecked(false);
    ui->R31->setChecked(false);
    ui->R33->setChecked(false);
    ui->R35->setChecked(false);
    ui->R37->setChecked(false);

    // 初始化 U 列 (19-37 奇数行)
    ui->U19->setChecked(false);
    ui->U21->setChecked(false);
    ui->U23->setChecked(false);
    ui->U25->setChecked(false);
    ui->U27->setChecked(false);
    ui->U29->setChecked(false);
    ui->U31->setChecked(false);
    ui->U33->setChecked(false);
    ui->U35->setChecked(false);
    ui->U37->setChecked(false);

    // AA列 (19-37奇数行)
    ui->AA19->setText("0");
    ui->AA21->setText("0");
    ui->AA23->setText("0");
    ui->AA25->setText("0");
    ui->AA27->setText("0");
    ui->AA29->setText("0");
    ui->AA31->setText("0");
    ui->AA33->setText("0");
    ui->AA35->setText("0");
    ui->AA37->setText("0");

    // AB列 (19-37奇数行)
    ui->AB19->setText("0");
    ui->AB21->setText("0");
    ui->AB23->setText("0");
    ui->AB25->setText("0");
    ui->AB27->setText("0");
    ui->AB29->setText("0");
    ui->AB31->setText("0");
    ui->AB33->setText("0");
    ui->AB35->setText("0");
    ui->AB37->setText("0");

    // AC列 (19-37奇数行)
    ui->AC19->setText("0");
    ui->AC21->setText("0");
    ui->AC23->setText("0");
    ui->AC25->setText("0");
    ui->AC27->setText("0");
    ui->AC29->setText("0");
    ui->AC31->setText("0");
    ui->AC33->setText("0");
    ui->AC35->setText("0");
    ui->AC37->setText("0");

    // X列 (19-37奇数行)
    ui->X19->setText("0");
    ui->X21->setText("0");
    ui->X23->setText("0");
    ui->X25->setText("0");
    ui->X27->setText("0");
    ui->X29->setText("0");
    ui->X31->setText("0");
    ui->X33->setText("0");
    ui->X35->setText("0");
    ui->X37->setText("0");

    // Y列 (19-37奇数行)
    ui->Y19->setText("0");
    ui->Y21->setText("0");
    ui->Y23->setText("0");
    ui->Y25->setText("0");
    ui->Y27->setText("0");
    ui->Y29->setText("0");
    ui->Y31->setText("0");
    ui->Y33->setText("0");
    ui->Y35->setText("0");
    ui->Y37->setText("0");

    // Z列 (19-37奇数行)
    ui->Z19->setText("0");
    ui->Z21->setText("0");
    ui->Z23->setText("0");
    ui->Z25->setText("0");
    ui->Z27->setText("0");
    ui->Z29->setText("0");
    ui->Z31->setText("0");
    ui->Z33->setText("0");
    ui->Z35->setText("0");
    ui->Z37->setText("0");

}
// 获取数据的方法
QMap<QString, double> INPUT::getData()
{
    QMap<QString, double> data;

    // AE17-BE17 字段名
    QStringList ae_be = {
        "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL", "AM", "AN",
        "AO", "AP", "AQ", "AR", "AS", "AT", "AU", "AV", "AW", "AX",
        "AY", "AZ", "BA", "BB", "BC", "BD", "BE"
    };

    // AE20:BE36 的字段名（行 20 到 36）
    QStringList aeToBeCols = {
        "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL", "AM", "AN",
        "AO", "AP", "AQ", "AR", "AS", "AT", "AU", "AV", "AW", "AX",
        "AY", "AZ", "BA", "BB", "BC", "BD", "BE"
    };
    QStringList special = {"X7", "E4"};

    // 读取 AE17-BE17
    for (const QString &col : ae_be) {
        QString name = QString("%1%2").arg(col).arg(17);
        QLineEdit *edit = this->findChild<QLineEdit*>(name);
        if (edit) data[name] = edit->text().toDouble();
    }

    // 读取 X7, E4,
    for (const QString &name : special) {
        QLineEdit *edit = this->findChild<QLineEdit*>(name);
        if (edit) data[name] = edit->text().toDouble();
    }

    for (int row = 20; row <= 36; row += 2) {
        for (const QString &col : aeToBeCols) {
            QString name = QString("%1%2").arg(col).arg(row);
            QLineEdit *edit = this->findChild<QLineEdit*>(name);
            if (edit) data[name] = edit->text().toDouble();
        }
    }

    // 读取C9到AC9的数据
    data["C9"] = ui->C9->text().toDouble();
    data["D9"] = ui->D9->text().toDouble();
    data["E9"] = ui->E9->text().toDouble();
    data["F9"] = ui->F9->text().toDouble();
    data["G9"] = ui->G9->text().toDouble();
    data["H9"] = ui->H9->text().toDouble();
    data["I9"] = ui->I9->text().toDouble();
    data["J9"] = ui->J9->text().toDouble();
    data["K9"] = ui->K9->text().toDouble();
    data["L9"] = ui->L9->text().toDouble();
    data["M9"] = ui->M9->text().toDouble();
    data["N9"] = ui->N9->text().toDouble();
    data["O9"] = ui->O9->text().toDouble();
    data["P9"] = ui->P9->text().toDouble();
    data["Q9"] = ui->Q9->text().toDouble();
    data["R9"] = ui->R9->text().toDouble();
    data["S9"] = ui->S9->text().toDouble();
    data["T9"] = ui->T9->text().toDouble();
    data["U9"] = ui->U9->text().toDouble();
    data["V9"] = ui->V9->text().toDouble();
    data["W9"] = ui->W9->text().toDouble();
    double x7 = ui->X7->text().toDouble();
    data["X9"] = x7;
    data["Y9"] = x7;
    data["Z9"] = x7;

    data["AA9"] = ui->AA9->text().toDouble();
    data["AB9"] = ui->AB9->text().toDouble();
    data["AC9"] = ui->AC9->text().toDouble();

    // ===== I 列 (19-37 奇数行) =====
    data["I19"] = ui->I19->isChecked() ? 1 : 0;
    data["I21"] = ui->I21->isChecked() ? 1 : 0;
    data["I23"] = ui->I23->isChecked() ? 1 : 0;
    data["I25"] = ui->I25->isChecked() ? 1 : 0;
    data["I27"] = ui->I27->isChecked() ? 1 : 0;
    data["I29"] = ui->I29->isChecked() ? 1 : 0;
    data["I31"] = ui->I31->isChecked() ? 1 : 0;
     data["I33"] = ui->I33->isChecked() ? 1 : 0;
     data["I35"] = ui->I35->isChecked() ? 1 : 0;
     data["I37"] = ui->I37->isChecked() ? 1 : 0;

       // ===== O 列 (19-37 奇数行) =====
       data["O19"] = ui->O19->isChecked() ? 1 : 0;
       data["O21"] = ui->O21->isChecked() ? 1 : 0;
       data["O23"] = ui->O23->isChecked() ? 1 : 0;
       data["O25"] = ui->O25->isChecked() ? 1 : 0;
       data["O27"] = ui->O27->isChecked() ? 1 : 0;
       data["O29"] = ui->O29->isChecked() ? 1 : 0;
       data["O31"] = ui->O31->isChecked() ? 1 : 0;
       data["O33"] = ui->O33->isChecked() ? 1 : 0;
       data["O35"] = ui->O35->isChecked() ? 1 : 0;
       data["O37"] = ui->O37->isChecked() ? 1 : 0;

       // ===== L 列 (19-37 奇数行) =====
       data["L19"] = ui->L19->isChecked() ? 1 : 0;
       data["L21"] = ui->L21->isChecked() ? 1 : 0;
       data["L23"] = ui->L23->isChecked() ? 1 : 0;
       data["L25"] = ui->L25->isChecked() ? 1 : 0;
       data["L27"] = ui->L27->isChecked() ? 1 : 0;
       data["L29"] = ui->L29->isChecked() ? 1 : 0;
       data["L31"] = ui->L31->isChecked() ? 1 : 0;
       data["L33"] = ui->L33->isChecked() ? 1 : 0;
       data["L35"] = ui->L35->isChecked() ? 1 : 0;
       data["L37"] = ui->L37->isChecked() ? 1 : 0;

       // ===== R 列 (19-37 奇数行) =====
       data["R19"] = ui->R19->isChecked() ? 1 : 0;
       data["R21"] = ui->R21->isChecked() ? 1 : 0;
       data["R23"] = ui->R23->isChecked() ? 1 : 0;
       data["R25"] = ui->R25->isChecked() ? 1 : 0;
       data["R27"] = ui->R27->isChecked() ? 1 : 0;
       data["R29"] = ui->R29->isChecked() ? 1 : 0;
       data["R31"] = ui->R31->isChecked() ? 1 : 0;
       data["R33"] = ui->R33->isChecked() ? 1 : 0;
       data["R35"] = ui->R35->isChecked() ? 1 : 0;
       data["R37"] = ui->R37->isChecked() ? 1 : 0;

       // ===== U 列 (19-37 奇数行) =====
       data["U19"] = ui->U19->isChecked() ? 1 : 0;
       data["U21"] = ui->U21->isChecked() ? 1 : 0;
       data["U23"] = ui->U23->isChecked() ? 1 : 0;
       data["U25"] = ui->U25->isChecked() ? 1 : 0;
       data["U27"] = ui->U27->isChecked() ? 1 : 0;
       data["U29"] = ui->U29->isChecked() ? 1 : 0;
       data["U31"] = ui->U31->isChecked() ? 1 : 0;
       data["U33"] = ui->U33->isChecked() ? 1 : 0;
       data["U35"] = ui->U35->isChecked() ? 1 : 0;
       data["U37"] = ui->U37->isChecked() ? 1 : 0;

       // AA列 (19-37奇数行)
          data["AA19"] = ui->AA19->text().toDouble();
          data["AA21"] = ui->AA21->text().toDouble();
          data["AA23"] = ui->AA23->text().toDouble();
          data["AA25"] = ui->AA25->text().toDouble();
          data["AA27"] = ui->AA27->text().toDouble();
          data["AA29"] = ui->AA29->text().toDouble();
          data["AA31"] = ui->AA31->text().toDouble();
          data["AA33"] = ui->AA33->text().toDouble();
          data["AA35"] = ui->AA35->text().toDouble();
          data["AA37"] = ui->AA37->text().toDouble();

          // AB列 (19-37奇数行)
          data["AB19"] = ui->AB19->text().toDouble();
          data["AB21"] = ui->AB21->text().toDouble();
          data["AB23"] = ui->AB23->text().toDouble();
          data["AB25"] = ui->AB25->text().toDouble();
          data["AB27"] = ui->AB27->text().toDouble();
          data["AB29"] = ui->AB29->text().toDouble();
          data["AB31"] = ui->AB31->text().toDouble();
          data["AB33"] = ui->AB33->text().toDouble();
          data["AB35"] = ui->AB35->text().toDouble();
          data["AB37"] = ui->AB37->text().toDouble();

          // AC列 (19-37奇数行)
          data["AC19"] = ui->AC19->text().toDouble();
          data["AC21"] = ui->AC21->text().toDouble();
          data["AC23"] = ui->AC23->text().toDouble();
          data["AC25"] = ui->AC25->text().toDouble();
          data["AC27"] = ui->AC27->text().toDouble();
          data["AC29"] = ui->AC29->text().toDouble();
          data["AC31"] = ui->AC31->text().toDouble();
          data["AC33"] = ui->AC33->text().toDouble();
          data["AC35"] = ui->AC35->text().toDouble();
          data["AC37"] = ui->AC37->text().toDouble();

          // X列 (19-37奇数行)
          data["X19"] = ui->X19->text().toDouble();
          data["X21"] = ui->X21->text().toDouble();
          data["X23"] = ui->X23->text().toDouble();
          data["X25"] = ui->X25->text().toDouble();
          data["X27"] = ui->X27->text().toDouble();
          data["X29"] = ui->X29->text().toDouble();
          data["X31"] = ui->X31->text().toDouble();
          data["X33"] = ui->X33->text().toDouble();
          data["X35"] = ui->X35->text().toDouble();
          data["X37"] = ui->X37->text().toDouble();

          // Y列 (19-37奇数行)
          data["Y19"] = ui->Y19->text().toDouble();
          data["Y21"] = ui->Y21->text().toDouble();
          data["Y23"] = ui->Y23->text().toDouble();
          data["Y25"] = ui->Y25->text().toDouble();
          data["Y27"] = ui->Y27->text().toDouble();
          data["Y29"] = ui->Y29->text().toDouble();
          data["Y31"] = ui->Y31->text().toDouble();
          data["Y33"] = ui->Y33->text().toDouble();
          data["Y35"] = ui->Y35->text().toDouble();
          data["Y37"] = ui->Y37->text().toDouble();

          // Z列 (19-37奇数行)
          data["Z19"] = ui->Z19->text().toDouble();
          data["Z21"] = ui->Z21->text().toDouble();
          data["Z23"] = ui->Z23->text().toDouble();
          data["Z25"] = ui->Z25->text().toDouble();
          data["Z27"] = ui->Z27->text().toDouble();
          data["Z29"] = ui->Z29->text().toDouble();
          data["Z31"] = ui->Z31->text().toDouble();
          data["Z33"] = ui->Z33->text().toDouble();
          data["Z35"] = ui->Z35->text().toDouble();
          data["Z37"] = ui->Z37->text().toDouble();

    return data;
}
INPUT::~INPUT()
{
    delete ui;
}

void INPUT::on_concernButtom_clicked()
{
    QMap<QString,double> in   = getData();   // 用户实际输入
    QMap<QString,double> full = in;          // 拓展为计算全集
    full.unite(previousResults);

    SharedData::instance().inputDataList.append(in);

    /* 直接用静态函数，不必 new CHOOSE tmp; */
    QMap<QString,double> res = CHOOSE::calculateDataStatic(full);

    SharedData::instance().calculatedResultList.append(res);
    displayResults(res);        // 刷 G19-G37 到 QTextBrowser

    emit dataEntered(in);
    //close();                    // 如要停在当前页可去掉
}


void INPUT::setPreviousResults(const QMap<QString,double>& d){ previousResults=d; }


void INPUT::displayResults(const QMap<QString, double>& results)
{
    for (int row = 19; row <= 37; row += 2) {
        QString key = QString("G%1").arg(row);  // G19, G21, ..., G37
        double value = results.value(key, 0.0); // 从结果中取出对应数值

        QTextBrowser* browser = this->findChild<QTextBrowser*>(key);
        if (browser) {
            browser->setText(QString::number(value, 'f', 4));
        }

    }
}

QMap<QString, double> INPUT::calculateData(const QMap<QString, double>& inputData)
{
    // 可选：委托 choose.cpp 中的 CHOOSE::calculateData，如果合理
    CHOOSE temp;
    return temp.calculateData(inputData);
}
void INPUT::on_cancelButton_clicked()
{
    close();
}

void INPUT::syncX7ToXYZ9(const QString& txt)
{
    ui->X9->setText(txt);
    ui->Y9->setText(txt);
    ui->Z9->setText(txt);
}
//

