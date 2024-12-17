#include "LWindow.hpp"
#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <iostream>

LWindow::LWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Water Quality Monitor");
    mainlayout = new QVBoxLayout(this);

    createWidgets();
    createToolbar();
}

void LWindow::populateWTBox(const QString& loc)
{
    location = loc;
    
    int WTcol = 12;
    int locCol = 3;
    QSet<QString> WTs;
    
    clearWidgets();

    // Clear the WTBox before populating it again
    WTBox->clear();

    for (int row = 0; row < proxy->rowCount(); ++row)
    {
        QModelIndex loc_ = proxy->index(row, locCol);
        QString locLab = proxy->data(loc_).toString();            
        
        if (locLab == location) {
            QModelIndex index = proxy->index(row, WTcol);
            QString lab = proxy->data(index).toString();
            WTs.insert(lab); // Only unique labels will be added
        }
    }

    proxy->setFilterKeyColumn(WTcol);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterFixedString("");

    // Add the unique labels to the WTBox
    WTBox->addItems(WTs.values());
    checkItemCt();

}

void LWindow::updateFile(WQTableModel& t)
{
  proxy = new QSortFilterProxyModel(this);
  proxy->setSourceModel(&t);

}

void LWindow::createWidgets()
{   
    WTBox = new QComboBox(this);
    LTBox = new QComboBox(this);

    WTLab = new QLabel("Water Type:");
    LTLab = new QLabel("Litter Type:");

    loadGButton = new QPushButton("Load Graph");

    LTBox->addItem("Bathing Water Profile : Other Litter (incl. plastics)");
    LTBox->addItem("TarryResidus");
    LTBox->addItem("SewageDebris");
    
    setLayout(mainlayout);

    connect(loadGButton, SIGNAL(clicked()), this, SLOT(loadGraph()));

}

void LWindow::createToolbar()
{
    // Create a QToolBar instance
    QToolBar *toolbar = new QToolBar();
    toolbar->setStyleSheet("QToolBar {spacing: 7px;}"); // Spacing between items in the toolbar
    
    toolbar->addWidget(WTLab);
    toolbar->addWidget(WTBox);
    toolbar->addWidget(LTLab);
    toolbar->addWidget(LTBox);
    toolbar->addWidget(loadGButton);

    // Add toolbar to the layout
    mainlayout->addWidget(toolbar);   
}

void LWindow::loadGraph()
{
    QString selectedWT = WTBox->currentText();
    QString selectedLT = LTBox->currentText();

    if (selectedWT.isEmpty() || selectedLT.isEmpty()) {
        checkItemCt();
        return;
    }

    clearWidgets(); // Clear existing widgets before creating the chart

    // Create a bar set for scores
    QBarSet *scoreSet = new QBarSet("Scores");

    // Initialize a map to hold the frequency count for scores 0-3
    QVector<int> scoreCounts(4, 0); // Index 0 -> score 0, index 3 -> score 3

    // Column indices for the dataset
    int locCol = 3;  // Location column
    int WTcol = 12;  // Water type column
    int resCol = 9;  // Result (score) column
    int LTcol = 5;  // Litter type column

    proxy->setFilterKeyColumn(locCol);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterFixedString(location);

    // Count the frequency of each score for the selected litter type and water type
    for (int row = 0; row < proxy->rowCount(); ++row)
    {
        QModelIndex locIndex = proxy->index(row, locCol);
        QModelIndex WTIndex = proxy->index(row, WTcol);
        QModelIndex LTIndex = proxy->index(row, LTcol);
        QModelIndex scoreIndex = proxy->index(row, resCol);

        QString locLabel = proxy->data(locIndex).toString();
        QString WTLabel = proxy->data(WTIndex).toString();
        QString LTLabel = proxy->data(LTIndex).toString();
        int score = proxy->data(scoreIndex).toInt();

        // Filter by selected water type
        if (WTLabel == selectedWT && LTLabel == selectedLT)
        {
            scoreCounts[score]++; // Increment the count for the respective score
        }

    }

    // Check if all counts are zero (i.e., no valid data)
    bool hasData = false;
    for (int count : scoreCounts) {
        if (count > 0) {
            hasData = true;
            break;
        }
    }

    // If there's no data (all counts are zero), show the "No data" message
    if (!hasData) {
        QLabel *tmp = new QLabel("No litter type found in location/water type\nReconfigure variables");
        clearWidgets();
        mainlayout->addWidget(tmp);
        return;
    }

    // Populate the bar set with the frequency counts
    *scoreSet << scoreCounts[0] << scoreCounts[1] << scoreCounts[2] << scoreCounts[3];

    // Create the bar series and add the bar set
    QBarSeries *series = new QBarSeries();
    series->append(scoreSet);

    // Create the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Scores for Selected Litter Type and Water Type");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Configure axes
    QStringList categories;
    categories << "Score 0" << "Score 1" << "Score 2" << "Score 3";

    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
    xAxis->append(categories);
    chart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 10); // Adjust the range based on your dataset
    yAxis->setTitleText("Frequency");
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    // Create compliance indicator label
    QLabel *complianceLabel = new QLabel(this);
    int goodScores = scoreCounts[0] + scoreCounts[1];  // Count of 0s and 1s
    int badScores = scoreCounts[2] + scoreCounts[3];   // Count of 2s and 3s

    // Check if goodScores are within 25% of badScores
    if (goodScores > badScores) {
        // More good scores than bad
        complianceLabel->setText("Compliant");
        complianceLabel->setStyleSheet("QLabel {color: green;}");
    } else if (goodScores >= badScores * 0.9 && goodScores <= badScores * 1.1) {
        // Good scores are within a 25% range of bad scores
        complianceLabel->setText("Warning");
        complianceLabel->setStyleSheet("QLabel {color: orange;}");
    } else {
        // More bad scores than good
        complianceLabel->setText("Non-Compliant");
        complianceLabel->setStyleSheet("QLabel {color: red;}");
    }

    // Add compliance label to the layout
    mainlayout->addWidget(complianceLabel);

    // Add chart view to the layout
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add chart view to the layout
    mainlayout->addWidget(chartView);
}



void LWindow::checkItemCt()
{
    clearWidgets(); // Clear existing widgets first

    if (WTBox->count() == 0) {
        QLabel *msg = new QLabel("Location doesn't have POPs\nPlease select another location");
        mainlayout->addWidget(msg);
    } else {
        QLabel *successMsg = new QLabel("Load Data And Configure Variables To View Graph");
        mainlayout->addWidget(successMsg);
    }
}

void LWindow::clearWidgets()
{
    // Safely delete all widgets except the toolbar
    for (int i = mainlayout->count() - 1; i >= 0; --i) {
        QLayoutItem *item = mainlayout->itemAt(i);
        if (QWidget *widget = item->widget()) {
            if (qobject_cast<QToolBar *>(widget)) {
                continue; // Skip the toolbar
            }
            mainlayout->removeWidget(widget);
            widget->hide();         // Hide widget immediately
            widget->deleteLater();  // Schedule deletion safely
        } else if (QLayout *childLayout = item->layout()) {
            mainlayout->removeItem(item);
            delete childLayout;
        }
    }
}
