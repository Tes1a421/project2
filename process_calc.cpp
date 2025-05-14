#include "process_calc.h"
#include <QtMath>
#include <QDebug>

/* ------------- 快捷访问 ------------- */
static inline double v(const QMap<QString,double>& m,
                       const QString& k,double def=0.0)
{ return m.value(k,def); }

/* ------------- I/L/R/U… → AE…BE ------------- */
void buildProcessWeights(int oddRow,
                         QMap<QString,double>& all,
                         const QMap<QString,double>& in)
{
    /* ===（1）收集本行开关量 === */
    auto key=[&](QString col){return col+QString::number(oddRow);};
    double I=v(in,key("I")), L=v(in,key("L"));
    double R=v(in,key("R")), U=v(in,key("U"));

    /* ===（2）示范只算 AE，其余 AF…BE 请照公式继续写 === */
    double AE = I*L*( R*0 + (1-R)*(U*qSqrt(2.)/2. + (1-U)*1.) )
              + (1-I*L)*( I*U*(R*0+(1-R)*1.)
              + (1-I*U)*( U*0+(1-U)*(R*0+(1-R)*1.) )
              + v(in,key("O")) );  // O19,O21… 也在 in 里
    //double AF =(I*L*(ABS(R-U)*1)+(1-I*L)*(I*0+(1-I)*(ABS(R-U)*1+(1-ABS(R-U))*0))+O)*COS(Y/180*PI())
    //double AG =
    all[QString("AE%1").arg(oddRow+1)] = AE;

    /* ……其余 AF~BE 依次写进去 …… */
}

/* ------------- 合成不确定度 G19,G21… ------------- */
double calcGxx(int oddRow,const QMap<QString,double>& allIn)
{
    /* 拷贝一份可写 map，如有缺失 AE…BE 就自动生成 */
    QMap<QString,double> all = allIn;
    if(!all.contains(QString("AE%1").arg(oddRow+1)))
        buildProcessWeights(oddRow,all,allIn);

    double sum=0.;
    QStringList cols={"AE","AF","AG","AH","AI","AJ","AK","AL","AM","AN",
                      "AO","AP","AQ","AR","AS","AT","AU","AV","AW","AX",
                      "AY","AZ","BA","BB","BC","BD","BE"};
    for(const auto& c:cols)
        sum+=v(all,QString("%1%2").arg(c).arg(oddRow+1));

    double O = v(all,QString("O%1").arg(oddRow));
    double BF= v(all,QString("BF%1").arg(oddRow+1));

    return qSqrt(sum*(O+1.)+BF);
}

/* ------------- BF20,BF22… ------------- */
double calcBFxx(int evenRow,const QMap<QString,double>& all)
{
    switch(evenRow){
    case 20:{          // BF20 = (BF19*工序1!G19)^2
        return qPow( v(all,"BF19") * v(all,"G19"), 2.0 );
    }
    case 22:{          // BF22 = 工序1!G21^2 * BF21^2
        return qPow( v(all,"G21"),2.0 ) * qPow( v(all,"BF21"),2.0 );
    }
    /* 继续补 24/26… */
    default:return 0.;
    }
}
