#ifndef SUBJECTOBS_H
#define SUBJECTOBS_H

#include <vector>
#include "StorageExplorerObserver.h"
#include <QDebug>

class Subject
{

public:
    void attach(IAdapterObserver *obs)  // Добавить наблюдателя
    {
        views.push_back(obs);
    }
    void setData(QHash<QString, float> table)
    {
        hashData = table;
        notify();
    }
    void notify()  // Оповещение всех обсерверов
    {
        for(auto *item: views){
            qDebug() << "notify";
            item->updateDisplay(hashData);
        }
    }

private:
    std::vector< class IAdapterObserver* > views;
    QHash<QString, float> hashData;
};

#endif // SUBJECTOBS_H
