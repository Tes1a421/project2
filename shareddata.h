#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <QString>
#include <QList>
#include <QMap>
#include <QPair>

class SharedData
{
public:
    static SharedData& instance()
    {
        static SharedData instance;
        return instance;
    }

    QList<QMap<QString, double>> inputDataList;
    QList<QMap<QString, double>> calculatedResultList;
    QList<QPair<QString, int>> processList;

    void clearAll()
    {
        inputDataList.clear();
        calculatedResultList.clear();
        processList.clear();
    }

private:
    SharedData() {}
    SharedData(const SharedData&) = delete;
    SharedData& operator=(const SharedData&) = delete;
};

#endif // SHAREDDATA_H
