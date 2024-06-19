#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableView>
#include <QTreeView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileSystemModel>
#include <QLabel>
#include <QHeaderView>
#include <QComboBox>
#include <QStackedWidget>
#include "CalculationSize.h"
#include "filebrowserdatamodel.h"
#include "Chart.h"
#include "SubjectObs.h"

class Widget : public QWidget
{
    Q_OBJECT

private slots:
    void selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void chooseStrategy(int choose);

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    // Модель файловой системы
    FileBrowserDataModel *tableModel;
    QTableView *tableView;
    // Модель таблицы
    QFileSystemModel *dirModel;
    QTreeView *treeView;
    // Столбчатая диаграмма
    QChart *barChart;
    QChartView *barChartView;
    // Круговая диаграмма
    QChart *pieChart;
    QChartView *pieChartView;

    QHBoxLayout *hLayout;
    QHBoxLayout *hLayoutComboBoxs;
    QVBoxLayout *vLayout;

    QLabel *statusBar;
    QLabel *labelChooseStategy;
    QLabel *labelChooseDispaly;

    QComboBox *chooseStategyBox;
    QComboBox *chooseDispalyBox;

    QString currentPath;
    SizeCalculator *currentStrategy;
    //CalculationSizeForFolders *folderStategy;
    //CalculationSizeForFileTypes *typeStrategy;

    Subject subj;
    TableModelAdapter *tableAdapter;
    BarChartAdapter *barChartAdapter;
    PieChartAdapter *pieChartAdapter;
};

#endif // WIDGET_H
