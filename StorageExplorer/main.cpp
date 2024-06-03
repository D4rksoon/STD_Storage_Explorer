#include <QCoreApplication>
#include "CalculationSize.h"
#include <QDebug>

void printHashTable(auto table)
{
    if(table.size() != 0){
        qDebug() << "Key    Value";
        for (auto i = table.cbegin(), end = table.cend(); i != end; ++i){
            if(i.value() > 0.01){
                qDebug() << qPrintable(i.key()) << ": " << i.value() << "%";
            }
            else if(i.value() != 0){
                qDebug() << qPrintable(i.key()) << ": " << "< 0.01 %";
            }
        }
        qDebug() << "\n";
    }
    else{
        qWarning() << "Table is empty\n";
    }
}

void printHashTable2(auto table)
{
    if(table.size() != 0){
        qDebug() << "Key    Value";
        for (auto i = table.cbegin(), end = table.cend(); i != end; ++i){
            if(i.value() < 0.01 and i.value() != 0){
                qDebug() << qPrintable(i.key()) << ": " << "< 0.01 %";
            }
            else{
                qDebug() << qPrintable(i.key()) << ": " << i.value() << "%";
            }
        }
        qDebug() << "\n";
    }
    else{
        qWarning() << "Table is empty\n";
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // Тесты /test1, /test2, /test3, /test4
    QString path = "../StorageExplorer/";

    SizeCalculator* calcFolders = new SizeCalculator(new CalculationSizeForFolders);
    SizeCalculator* calcTypes = new SizeCalculator(new CalculationSizeForFileTypes);
    printHashTable2(calcFolders->Calculation(path));
    printHashTable2(calcTypes->Calculation(path));


    delete calcFolders;
    delete calcTypes;
    return a.exec();
}









