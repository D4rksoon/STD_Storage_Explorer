#ifndef STORAGEEXPLOREROBSERVER_H
#define STORAGEEXPLOREROBSERVER_H

#include "filebrowserdatamodel.h"
#include "Chart.h"

class IAdapterObserver
{
public:
    virtual void updateDisplay(QHash<QString, float> table) = 0;
};

class TableModelAdapter : public IAdapterObserver
{
public:
    TableModelAdapter(FileBrowserDataModel *p) : tableModel(p){}
    ~TableModelAdapter() { delete tableModel; }
    void updateDisplay(QHash<QString, float> table)
    {
        QList<tableData> listData;
        float sumValue;
        for (auto j = table.cbegin(), end = table.cend(); j != end; ++j){
            sumValue += j.value();
        }
        for (auto i = table.cbegin(), end = table.cend(); i != end; ++i){
            QString name = i.key();
            QString size = QString::number(long(i.value() / 1024)) + " KB";
            QString percent;
            float prcnt = ceil(i.value() / sumValue * 10000) / 100;
            if(prcnt < 0.01 and prcnt != 0){
                percent = "<0.01 %";
            }
            else{
                percent = QString::number(prcnt) + " %";
            }

            listData.append(tableData(name, size, percent));
        }
        tableModel->setDataModel(listData);
    }

private:
    FileBrowserDataModel *tableModel;

};

class PieChartAdapter : public IAdapterObserver
{
public:
    PieChartAdapter(QChart *p) : pieChart(p){}
    ~PieChartAdapter() { delete pieChart; }
    void updateDisplay(QHash<QString, float> table)
    {
        PieChart chartModel;
        chartModel.createChart(table, pieChart);
    }
private:
    QChart *pieChart;


};

class BarChartAdapter : public IAdapterObserver
{
public:
    BarChartAdapter(QChart *p) : barChart(p){}
    ~BarChartAdapter() { delete barChart; }
    void updateDisplay(QHash<QString, float> table)
    {
        BarChart chartModel;
        chartModel.createChart(table, barChart);
    }
private:
    QChart *barChart;

};

#endif // STORAGEEXPLOREROBSERVER_H
