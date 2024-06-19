#include "widget.h"
#include <QDebug>

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

    barChart = new QChart();
    barChartView = new QChartView(this);
    barChartView->setChart(barChart);

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

    tableAdapter = new TableModelAdapter(tableModel);
    pieChartAdapter = new PieChartAdapter(pieChart);
    barChartAdapter = new BarChartAdapter(barChart);

    subj.attach(tableAdapter);
    subj.attach(pieChartAdapter);
    subj.attach(barChartAdapter);

    QItemSelectionModel *selectionModel = treeView->selectionModel();

    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &Widget::selectionChangedSlot);

    connect(chooseStategyBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &Widget::chooseStrategy);

    connect(chooseDispalyBox, qOverload<int>(&QComboBox::activated), stackedWidget, &QStackedWidget::setCurrentIndex);
}

Widget::~Widget()
{
    delete tableModel;
    delete tableAdapter;
    delete pieChartAdapter;
    delete barChartAdapter;
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
        subj.setData(table);
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
        subj.setData(table);
        qDebug() << currentPath;
    }
}
