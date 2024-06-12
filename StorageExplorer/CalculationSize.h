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
        return table;
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

class CalculationSizeForFileTypes : public CalculationSize
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
            table = tableTypes(path, table);
        }
        return table;
    }

protected:
    QHash<QString, float> tableTypes(QString path, QHash<QString, float> table)
    {
        QDir folder(path);
        folder.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

        QFileInfoList listOfFiles = folder.entryInfoList();
        foreach (QFileInfo fileInfo, listOfFiles) {
            QString fileName(fileInfo.fileName());

            if(fileInfo.isDir()){
                table = (tableTypes(path + "/" + fileName, table));
            }
            else{
                QString suf = fileInfo.suffix();
                float s = table.value(suf);
                table.insert(suf, s + fileInfo.size());
            }
        }
        return table;
    }
};

class SizeCalculator
{
public:
    SizeCalculator(CalculationSize* method) : m(method) {}
    ~SizeCalculator() { delete m; }
    QHash<QString, float>Calculation(const QString path)
    {
        return m->Calculation(path);
    }
    void setCalculationStrategy(CalculationSize *strategy)
    {
        m = strategy;
    }
private:
    // указатель на объект
    CalculationSize* m;
};

#endif // CALCULATIONSIZE_H















