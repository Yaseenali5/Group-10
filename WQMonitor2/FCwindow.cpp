#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "FCwindow.hpp"

FCWindow::FCWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Water Quality Monitor");
    mainlayout = new QVBoxLayout(this);

    createWidgets();
    createToolbar();
}

void FCWindow::updateFile(WQTableModel& t)
{
  proxy = new QSortFilterProxyModel(this);
  proxy->setSourceModel(&t);
  resultBox->clear();
}

void FCWindow::populateResultsBox(const QString& loc)
{
    location = loc;
    
    int DLcol = 5;
    int locCol = 3;

    QSet<QString> uPFAS;
    

    clearWidgets();

    // Clear the resultBox before populating it again
    resultBox->clear();

    proxy->setFilterKeyColumn(DLcol+1);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterFixedString("perfluoro");

    // Collect unique labels for the current PFAS
    for (int row = 0; row < proxy->rowCount(); ++row)
    {
        QModelIndex loc_ = proxy->index(row, locCol);
        QString locLab = proxy->data(loc_).toString();            
        
        if (locLab == location) {
            QModelIndex index = proxy->index(row, DLcol);
            QString lab = proxy->data(index).toString();
            uPFAS.insert(lab); // Only unique labels will be added
        }
    }



    proxy->setFilterKeyColumn(DLcol);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterFixedString("");

    // Add the unique labels to the resultBox
    resultBox->addItems(uPFAS.values());
    checkItemCt();

}

void FCWindow::createWidgets()
{    
    resultLabel = new QLabel(tr("Choose PFAS:"));
    resultBox = new QComboBox(this);
    
    loadGButton = new QPushButton(tr("Load Graph"));
    healthInfo = new QPushButton(tr("Health Information"));
    envInfo = new QPushButton(tr("Environmental Information"));
    
    // Set up healthRisks and monImp tooltips and enable states
    healthInfo->setEnabled(false);
    healthInfo->setToolTip("");
    
    envInfo->setEnabled(false);
    envInfo->setToolTip("");

    connect(loadGButton, SIGNAL(clicked()), this, SLOT(loadGraph()));

    setLayout(mainlayout);
}

void FCWindow::createToolbar()
{
    // Create a QToolBar instance
    QToolBar *toolbar = new QToolBar();
    toolbar->setStyleSheet("QToolBar {spacing: 10px;}"); // Spacing between items in the toolbar
    
    // Adding widgets to the toolbar
    toolbar->addWidget(resultLabel);
    toolbar->addWidget(resultBox);
    toolbar->addWidget(loadGButton);
    toolbar->addWidget(healthInfo);
    toolbar->addWidget(envInfo);

    // Add toolbar to the layout
    mainlayout->addWidget(toolbar);

}

void FCWindow::clearWidgets()
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

void FCWindow::loadGraph()
{
    QString selectedPFAS = resultBox->currentText(); // these are to store each filter selected
    if (selectedPFAS == "") {
        return;
    }
    QString selectedLocation = location;
    
    clearWidgets();

    QChartView *chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainlayout->addWidget(chartView);

    QChart *chart = new QChart();
    QString unit1; // this stores the units used, for Y axis title
    QScatterSeries *Sseries = new QScatterSeries();
    QLineSeries *Lseries = new QLineSeries(); // a scatter series and a line series is used to show each individual data point and the line that adjoins them

 
    chartView->setToolTip(tr("Safety Threshold Information\nCompliant: < 0.01 ug/l\nWarning: between 0.01 and 0.07 ug/l\nNon-Compliant: > 0.07 ug/l"));


    Sseries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    Sseries->setMarkerSize(10.0); // formatting for the scatter chart
    int locCol = 3;
    int PFASCol = 5;
    int resCol = 9;
    int timeCol = 4;
    int unitCol = 11; // these represent column indexes for each attribute used

    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::lowest(); // these are going to be used for the graph padding, to make the data more presentable

    qint64 timestamp; // used for the processing of the sampling time
    double lastRes;   // used to store the last result, this point is used to colour the chart

    for (int row = 0; row < proxy->rowCount(); ++row)
    {
        QModelIndex locationIndex = proxy->index(row, locCol);
        QModelIndex resultIndex = proxy->index(row, resCol);
        QModelIndex timeIndex = proxy->index(row, timeCol);
        QModelIndex PFASIndex = proxy->index(row, PFASCol);
        QModelIndex unitIndex = proxy->index(row, unitCol); // these are pointers created to get the data from each cell

        QString location = proxy->data(locationIndex).toString();
        QString result = proxy->data(resultIndex).toString();
        QString time = proxy->data(timeIndex).toString();
        QString PFAS_ = proxy->data(PFASIndex).toString();
        QString unit = proxy->data(unitIndex).toString(); // these are accessing each pointer to get the data from each cell and make them into QStrings
        if (location == selectedLocation && PFAS_ == selectedPFAS) // this is to ensure that the data that is actually added to each series are the data that the user wants
        {
        bool ok;
        double resultValue = result.toDouble(&ok);

        if (ok) // this is used to verify that the result is of double type
        {
            QDateTime datetime = QDateTime::fromString(time, "yyyy-MM-ddTHH:mm:ss");
            datetime.setTimeSpec(Qt::LocalTime); // this is used to gather data from each samples sample time
            if (datetime.isValid())              // validation checking if the time data is correct
            {
            timestamp = datetime.toMSecsSinceEpoch(); // this converts the time data into a qint64, which can then be processed for the graph
            Sseries->append(timestamp, resultValue);
            Lseries->append(timestamp, resultValue);
            unit1 = unit; // this is used to store the units for the Y axis title

            if (resultValue < minValue)
                minValue = resultValue;
            if (resultValue > maxValue)
                maxValue = resultValue; // bubbling the minimum and maximum values

            lastRes = resultValue;
            }
        }
        }
    }
        // Set tooltip when hovering over points
    connect(Sseries, &QScatterSeries::clicked, this, [=](QPointF point) {

        double yValue = point.y();
        QString txt = QString("PFAS level: %1 %2").arg(yValue).arg(unit1);    
        QMessageBox *popup = new QMessageBox();
        popup->setText(txt);
        popup->setStandardButtons(QMessageBox::Ok);

        // Close the message box after 3 seconds (3000 ms)
        QTimer::singleShot(3000, popup, &QMessageBox::accept);

        popup->exec();
        
    });


    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd HH:mm:ss");
    axisX->setTitleText("Time");
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(QString("PFAS Level / %1").arg(unit1)); // XY axis initialising and formatting

    QColor indicator = indicatorColor(lastRes);
    Lseries->setColor(indicator);
    Sseries->setColor(indicator); // this make the last results value be used for colouring

    double valuePadding = (maxValue - minValue) * 0.15;

    if (valuePadding == 0) // if all values are of the same value
    {
        QPointF pt = Sseries->at(0);
        qreal y = pt.y();
        valuePadding = y * 0.15;
    }

    int numPt = Sseries->count();

    if (numPt == 1) // if there is only one data point
    {
        QDateTime min = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(timestamp - (timestamp * 0.15)));
        QDateTime max = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(timestamp + (timestamp * 0.15)));

        axisX->setRange(min, max);
    }

    axisY->setRange(minValue - valuePadding, maxValue + valuePadding); // setting the padding

    chart->addSeries(Sseries);
    chart->addSeries(Lseries);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    Sseries->attachAxis(axisX);
    Lseries->attachAxis(axisX);
    Sseries->attachAxis(axisY);
    Lseries->attachAxis(axisY); // attatching axis and series to chart

    chart->legend()->hide();
    chart->setTitle(QString("PFAS Analysis of %1").arg(selectedLocation));
    chartView->setChart(chart);

}

QColor FCWindow::indicatorColor(double result)
{   
    if (result < 0.01)
    {
    return Qt::green;
    }
    else if (0.01 < result < 0.07)
    {
    return QColor(255, 165, 0);
    }
    else
    {
    return Qt::red;
    }


    return Qt::black; // default return being black
}

void FCWindow::checkItemCt()
{
    clearWidgets(); // Clear existing widgets first

    if (resultBox->count() == 0) {
        QLabel *msg = new QLabel(tr("Location doesn't have Fluorinated Compounds\nPlease select another location"));
        mainlayout->addWidget(msg);
    } else {
        QLabel *successMsg = new QLabel(tr("Load Data And Configure Variables To View Graph"));
        mainlayout->addWidget(successMsg);
    }
}
