#ifndef CALCULATIONSIZE_H
#define CALCULATIONSIZE_H
#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <math.h>

class CalculationSize
{
public:
    virtual ~CalculationSize() {}

    virtual QHash<QString, float>Calculation(const QString path) = 0;

    virtual QHash<QString, float> percent(QHash<QString, float> table)
    {
        float sumValue;
        for (auto j = table.cbegin(), end = table.cend(); j != end; ++j){
            sumValue += j.value();
        }
        //qDebug() << "Size of all folders" << sumValue;
        if(sumValue != 0) {
            for (auto i = table.cbegin(), end = table.cend(); i != end; ++i){
                table[i.key()] = ceil(i.value() / sumValue * 10000) / 100;
            }
        }
        else {
            for (auto i = table.cbegin(), end = table.cend(); i != end; ++i){
                table[i.key()] = 0;
            }
        }
        return table;
    }
};

class CalculationSizeForFolders : public CalculationSize
{
public:
    QHash<QString, float>Calculation(const QString path)
    {
        QHash<QString, float> table;
        QDir folder(path);
        if(!folder.exists()){
            qWarning("Can not find directory");
        }
        else{
            table = tableFolders(path);
        }
        return percent(table);
    }

protected:
    float folderSize(QString path)
    {
        float totalSize;
        QDir folder(path);
        folder.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot); // Что будет показываться
        QFileInfoList listOfFiles = folder.entryInfoList(); // Лист файлов в папке

        foreach ( QFileInfo fileInfo, listOfFiles ) {
            if ( fileInfo.isDir() )
                totalSize += folderSize( path + "/" + fileInfo.fileName() );
            else
                totalSize += fileInfo.size();
        }
        return totalSize;
    }
    QHash<QString, float>tableFolders(const QString path)
    {
        float otherFolderSize = 0;
        float currentFolderSize = 0;
        QDir folder(path);

        QHash<QString, float> table;

        folder.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        QFileInfoList listOfFiles = folder.entryInfoList();

        foreach (QFileInfo fileInfo, listOfFiles) {
            QString fileName(fileInfo.fileName());

            if(fileInfo.isDir()){
                otherFolderSize = folderSize(path + "/" + fileName);
                table.insert(fileName, otherFolderSize);
            }
            else{
                currentFolderSize += fileInfo.size();
            }
        }
        table.insert("currentFolder", currentFolderSize);
        return table;
    }

};
















#endif // CALCULATIONSIZE_H















