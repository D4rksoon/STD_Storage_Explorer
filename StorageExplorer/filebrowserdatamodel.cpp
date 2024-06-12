#include "filebrowserdatamodel.h"

FileBrowserDataModel::FileBrowserDataModel(QObject *parent, QList<tableData> table)
    : QAbstractTableModel(parent)
{
    dataModel = table;
}

int FileBrowserDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return dataModel.count();
}

int FileBrowserDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return PERCENT + 1; // 2 + 1
}

QVariant FileBrowserDataModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || dataModel.count() <= index.row() || (role != Qt::DisplayRole && role != Qt::EditRole)){
        return QVariant();
    }
    switch (index.column()) {
    case NAME:
        return dataModel[index.row()].name;
        break;
    case SIZE:
        return dataModel[index.row()].size;
        break;
    case PERCENT:
        return dataModel[index.row()].prcent;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant FileBrowserDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case NAME:
            return trUtf8("Имя");
        case SIZE:
            return trUtf8("Размер");
        case PERCENT:
            return trUtf8("В процентах");
        }
    }

    return QVariant();
}

bool FileBrowserDataModel::setDataModel(QList<tableData> newDataModel)
{
    dataModel = newDataModel;
    emit layoutChanged();

    return true;
}
