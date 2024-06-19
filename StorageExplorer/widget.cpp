#include "widget.h"
#include <QDebug>
QList<tableData> fillingData(QHash<QString, float> table)
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
    return listData;
}

Widget::Widget(QWidget *parent)
    : QWidget{parent}
{
    currentStrategy->setCalculationStrategy(new CalculationSizeForFolders);
    currentPath = "";

    this->setGeometry(100, 100, 1000, 500);
    QFont font;
    font.setPointSize(9);
    setFont(font);

    vLayout = new QVBoxLayout(this);
    hLayout = new QHBoxLayout();
    hLayoutComboBoxs = new QHBoxLayout();

    tableModel = new FileBrowserDataModel(this);
    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirModel->setRootPath("C:/");

    tableView = new QTableView(this);
    tableView->setModel(tableModel);
    tableView->setShowGrid(false); // отключение сетки
    tableView->setColumnWidth(0, 310); // значение по умолчанию 125

    treeView = new QTreeView(this);
    treeView->setModel(dirModel);
    treeView->setMinimumWidth(400);
    treeView->header()->resizeSection(0, 400);

    barChartClass = new BarChart();
    barChart = new QChart();

    barChartView = new QChartView(this);
    barChartView->setChart(barChart);

    pieChartClass = new PieChart();
    pieChart = new QChart();

    pieChartView = new QChartView(this);
    pieChartView->setChart(pieChart);

    statusBar = new QLabel("Выбранный путь: ");
    labelChooseStategy = new QLabel("Стратегия: ");
    labelChooseDispaly = new QLabel("Вид отображения: ");

    chooseStategyBox = new QComboBox();
    chooseStategyBox->addItem("by Folders");
    chooseStategyBox->addItem("by TypeFiles");
    chooseDispalyBox = new QComboBox();
    chooseDispalyBox->addItem("Table View");
    chooseDispalyBox->addItem("Pie Chart");
    chooseDispalyBox->addItem("Bar Chart");

    hLayoutComboBoxs->addStretch(1);
    hLayoutComboBoxs->addWidget(labelChooseStategy);
    hLayoutComboBoxs->addWidget(chooseStategyBox);
    hLayoutComboBoxs->addWidget(labelChooseDispaly);
    hLayoutComboBoxs->addWidget(chooseDispalyBox);
    hLayoutComboBoxs->addStretch(1);

    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(tableView);
    stackedWidget->addWidget(pieChartView);
    stackedWidget->addWidget(barChartView);

    hLayout->addWidget(treeView);
    hLayout->addWidget(stackedWidget);
    hLayout->setStretchFactor(stackedWidget, 1);

    vLayout->addLayout(hLayoutComboBoxs);
    vLayout->addWidget(statusBar);
    vLayout->addLayout(hLayout);

    QItemSelectionModel *selectionModel = treeView->selectionModel();

    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &Widget::selectionChangedSlot);

    connect(chooseStategyBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &Widget::chooseStrategy);

    connect(chooseDispalyBox, qOverload<int>(&QComboBox::activated), stackedWidget, &QStackedWidget::setCurrentIndex);
}

Widget::~Widget()
{
    delete barChartClass;
    delete pieChartClass;
    delete tableModel;
}

void Widget::selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    QModelIndex index = treeView->selectionModel()->currentIndex();
    if(index.isValid()){
        currentPath = dirModel->filePath(index);
        this->statusBar->setText("Выбранный путь : " + currentPath);
        QHash<QString, float> table = currentStrategy->Calculation(currentPath);
        tableModel->setDataModel(fillingData(table));

        pieChartClass->createChart(table, pieChart);

        barChartClass->createChart(table, barChart);
    }
}

void Widget::chooseStrategy(int choose)
{
    if(choose == 0){
        currentStrategy->setCalculationStrategy(new CalculationSizeForFolders);
        qDebug() << "by Folders";
    }
    else if(choose == 1){
        currentStrategy->setCalculationStrategy(new CalculationSizeForFileTypes);
        qDebug() << "by Types";
    }
    if(currentPath != ""){
        QHash<QString, float> table = currentStrategy->Calculation(currentPath);

        tableModel->setDataModel(fillingData(table));

        pieChartClass->createChart(table, pieChart);

        barChartClass->createChart(table, barChart);

        qDebug() << currentPath;
    }
}
