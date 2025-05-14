#ifndef PROCESS_CALC_H
#define PROCESS_CALC_H

#include <QMap>
#include <QString>

/* ========= 公开 API ========= */
double calcGxx (int oddRow , const QMap<QString,double>& all);   // G19,G21…
double calcBFxx(int evenRow, const QMap<QString,double>& all);   // BF20,BF22…

/* ========= 内部辅助 ========= */
void buildProcessWeights(int oddRow,
                         QMap<QString,double>& all,      // 放输出（AE…BE）
                         const QMap<QString,double>& in);// 用户手输量
#endif
