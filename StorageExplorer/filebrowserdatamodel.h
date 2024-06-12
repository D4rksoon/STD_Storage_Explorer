#ifndef FILEBROWSERDATAMODEL_H
#define FILEBROWSERDATAMODEL_H

#include <QObject>
#include <QAbstractTableModel>

class tableData
{
public:
    tableData(QString nm = "Name", QString sz = "Size", QString prc = "Percent")
    {
        name = nm;
        size = sz;
        prcent = prc;
    }
    QString name;
    QString size;
    QString prcent;
};

class FileBrowserDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    FileBrowserDataModel(QObject *parent = nullptr, QList<tableData> table = QList<tableData>());

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    // Перечисление названий столбцов таблицы
    enum NameColumn {
        NAME = 0,
        SIZE,    // = 1
        PERCENT  // = 2
    };
    QList<tableData> dataModel;
};

#endif // FILEBROWSERDATAMODEL_H
