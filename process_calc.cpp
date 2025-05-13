#include ":contentReference[oaicite:3]{index=3}==========================
double weight17(const QMap<QStri:contentReference[oaicite:4]{index=4}t QString& col)
{
    // AE17-BE17  ← C9-AC9
    st:contentReference[oaicite:5]{index=5}","E","F","G","H","I","J","K","L","M",
                                 :contentReference[oaicite:6]{index=6}
                                    "X","Y","Z","AA","AB","AC"};
    static const QStringList tgt = {"AE","AF","AG","AH","AI","AJ","AK","AL","AM",
                                 :contentReference[oaicite:7]{index=7}                "AW","AX","AY","AZ","BA","BB","BC","BD","BE"};
    int k = tgt.indexOf(col);
    if(k>=0 && k<src.size()) return d.value(src[k]+"9",0.0);

    double X7 = d.value("X7",0.0), AD = d.value("AD19",0.0);
    if(col=="AZ") return X7*AD*d.value("AA19")/1000.0;
    if(col=="BA") return X7*d.value("AB19")*AD/1000.0;
    if(col=="BB") return X7*AD*d.value("AC19")/1000.0;
:contentReference[oaicite:8]{index=8}=
static double f_sub(double u,double r){
    return r*0.0 + (1-r)*(u*std::sqrt(2.0)/2.0 + (1-u)*1.0);
}
double BF_odd(int row,const QMap<QString,double>& d)
{
    auto v=[&](QString p){return d.value(p+QString::number(row),0.0);};
    double I=v("I"), L=v("L"), O=v("O"), R=v("R"), U=v("U");
    double Uprev=d.value("U"+QString::number(row-9),0.0);   // U10/U12/…

    double t1 = I*L*          f_sub(U,R);
    double t2 = (1-I*L)*( I*U*f_sub(1.0,R)               // R*0+(1-R)*1
                        + (1-I*U)*( U*0+(1-U)*f_sub(1.0,R) ) );
    return t1+t2+O;
}

// === BF 偶数行 20/22/… =======================================================
double BF_even(int row,const QMap<QString,double>& d,
               bool hasPrev,const QMap<QString,double>& prev)
{
    double bfOdd = d.value("BF"+QString::number(row-1),0.0);
    double gRef  = hasPrev
                 ? prev.value("G"+QString::number(row-1),0.0)   // 上一工序 Godd
                 : d.value ("G"+QString::number(row-1),0.0);    // 本工序 Godd
    return std::pow(bfOdd*gRef,2.0);
}

// === G 奇数行 19/21/…  (闭式解) =============================================
double G_odd(int row,const QMap<QString,double>& d)
{
    static const QStringList err = {"C","D","E","F","G","H","I","J","K","L","M",
                                    "N","O","P","Q","R","S","T","U","V","W",
                                    "X","Y","Z","AA","AB","AC"};
    static const QStringList w17 = {"AE","AF","AG","AH","AI","AJ","AK","AL","AM",
                                    "AN","AO","AP","AQ","AR","AS","AT","AU","AV",
                                    "AW","AX","AY","AZ","BA","BB","BC","BD","BE"};
    double sum = 0.0;
    for(int i=0;i<err.size();++i){
        double s = d.value(err[i]+"9",0.0);
        double w = d.value(w17[i]+QString::number(row+1),0.0);   // AE20..BE20
        sum += s*w;
    }
    // 额外三列
    for(auto col: {"AZ","BA","BB"}){
        double s = d.value("X9",0.0);
        double w = d.value(QString(col)+QString::number(row+1),0.0);
        sum += s*w;
    }
    double A      = sum * ( d.value("O"+QString::number(row),0.0) + 1.0 );
    double bfOdd  = d.value("BF"+QString::number(row),0.0);
    double denom  = 1.0 - bfOdd*bfOdd;
    return denom<=0 ? 0.0 : std::sqrt( A / denom );
}
