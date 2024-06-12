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

    statusBar = new QLabel("Выбранный путь: ");
    labelChooseStategy = new QLabel("Стратегия: ");
    labelChooseDispaly = new QLabel("Вид отображения: ");

    tableView = new QTableView();
    tableView->setModel(tableModel);
    tableView->setShowGrid(false); // отключение сетки
    tableView->setColumnWidth(0, 310); // значение по умолчанию 125

    treeView = new QTreeView();
    treeView->setModel(dirModel);
    treeView->setMinimumWidth(400);
    treeView->header()->resizeSection(0, 400);

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

    hLayout->addWidget(treeView);
    hLayout->addWidget(tableView);
    hLayout->setStretchFactor(tableView, 1);

    //    QLabel * label1 = new QLabel("Table");
    //    QLabel * label2 = new QLabel("Pie");
    //    QLabel * label3 = new QLabel("Bar");

    //    QStackedWidget *stackedWidget = new QStackedWidget;
    //    stackedWidget->addWidget(label1);
    //    stackedWidget->addWidget(label2);
    //    stackedWidget->addWidget(label3);

    vLayout->addLayout(hLayoutComboBoxs);
    vLayout->addWidget(statusBar);
    vLayout->addLayout(hLayout);
    //    vLayout->addWidget(stackedWidget);

    QItemSelectionModel *selectionModel = treeView->selectionModel();

    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &Widget::selectionChangedSlot);

    connect(chooseStategyBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &Widget::chooseStrategy);

    //    connect(chooseDispalyBox, SIGNAL(activated(int)), stackedWidget, SLOT(setCurrentIndex(int)));

}

Widget::~Widget()
{}

void Widget::selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    QModelIndex index = treeView->selectionModel()->currentIndex();
    if(index.isValid()){
        currentPath = dirModel->filePath(index);
        this->statusBar->setText("Выбранный путь : " + currentPath);
        tableModel->setDataModel(fillingData(currentStrategy->Calculation(currentPath)));
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
        tableModel->setDataModel(fillingData(currentStrategy->Calculation(currentPath)));
        qDebug() << currentPath;
    }
}
