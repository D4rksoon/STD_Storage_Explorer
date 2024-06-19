#ifndef CHART_H
#define CHART_H

#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <math.h>

using namespace QtCharts;

class Chart
{
public:
    void createChart(QHash<QString, float> table, QChart* chart) // шаблонный метод состойщий из drawChart | drawLegend
    {
        drawChart(table, chart);
        drawLegend(chart);
    }
private:
    virtual void drawChart(QHash<QString, float> table, QChart* chart) = 0;
    void drawLegend(QChart* chart)
    {
        //chart->setAnimationOptions(QChart::AllAnimations);
        chart->legend()->setAlignment(Qt::AlignRight);
    }
};

class PieChart : public Chart
{
    void drawChart(QHash<QString, float> table, QChart* chart)
    {
        float prcnt = 0;
        float other = 0;
        QString name;

        QPieSeries *series = new QPieSeries(chart);

        float sumValue;
        for (auto j = table.cbegin(), end = table.cend(); j != end; ++j){
            sumValue += j.value();
        }

        for (auto i = table.cbegin(), end = table.cend(); i != end; ++i){
            prcnt = ceil(i.value() / sumValue * 10000) / 100;
            if(prcnt < 2){ // Минимальный размер папки / файла
                other += prcnt;
            }
            else{
                name = i.key() + " (" + QString::number(prcnt) + " %)";
                series->append(name, prcnt);
            }
        }
        if(other){
            name = "other (" + QString::number(other) + " %)";
            series->append(name, other);
        }
        chart->removeAllSeries();
        chart->addSeries(series);
    }
};

class BarChart : public Chart
{
    void drawChart(QHash<QString, float> table, QChart* chart)
    {
        float prcnt = 0;
        float other = 0;
        QString name;

        QBarSeries *series = new QBarSeries(chart);

        float sumValue;
        for (auto j = table.cbegin(), end = table.cend(); j != end; ++j){
            sumValue += j.value();
        }

        for (auto i = table.cbegin(), end = table.cend(); i != end; ++i){
            prcnt = ceil(i.value() / sumValue * 10000) / 100;
            if(prcnt < 2){
                other += prcnt;
            }
            else{
                name = i.key() + " (" + QString::number(prcnt) + " %)";
                QBarSet *set = new QBarSet(name);
                set->append(prcnt);
                series->append(set);
            }
        }
        if(other){
            name = "other (" + QString::number(other) + " %)";
            QBarSet *otherSet = new QBarSet(name);
            otherSet->append(other);
            series->append(otherSet);
        }
        chart->removeAllSeries();
        chart->addSeries(series);
    }
};


#endif // CHART_H
