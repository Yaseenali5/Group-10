#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "POPwindow.hpp" // these are the includes needed for the POP page

POPWindow::POPWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Water Quality Monitor");
    mainlayout = new QVBoxLayout(this);
    POPs = new QVector<QString>();    
    POPs->append("PCB");
    POPs->append("DDT");
    POPs->append("Aldrin");

    createWidgets();
    createToolbar();
}

void POPWindow::updateFile(WQTableModel& t)
{
  proxy = new QSortFilterProxyModel(this);
  proxy->setSourceModel(&t);
  resultBox->clear();
}

void POPWindow::populateResultsBox(const QString& loc)
{
    location = loc;
    
    int DLcol = 5;
    int locCol = 3;

    QSet<QString> upops;
    

    clearWidgets();

    // Clear the resultBox before populating it again
    resultBox->clear();

    // Iterate through the list of POPs and apply the filter for each one
    for (int t = 0; t < POPs->size(); ++t) {
        proxy->setFilterKeyColumn(DLcol);
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxy->setFilterFixedString(POPs->at(t));

        // Collect unique labels for the current POP
        for (int row = 0; row < proxy->rowCount(); ++row)
        {
            QModelIndex loc_ = proxy->index(row, locCol);
            QString locLab = proxy->data(loc_).toString();            
            
            if (locLab == location) {
              QModelIndex index = proxy->index(row, DLcol);
              QString lab = proxy->data(index).toString();
              upops.insert(lab); // Only unique labels will be added
            }
        }
    }


    proxy->setFilterKeyColumn(DLcol);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterFixedString("");

    // Add the unique labels to the resultBox
    resultBox->addItems(upops.values());
    checkItemCt();

}

void POPWindow::createWidgets()
{    
    resultLabel = new QLabel("Choose POP:");
    resultBox = new QComboBox(this);
    
    loadGButton = new QPushButton("Load Graph");
    healthRisks = new QPushButton("Health Risks");
    monImp = new QPushButton("Monitoring Importance");
    
    // Set up healthRisks and monImp tooltips and enable states
    healthRisks->setEnabled(false);
    
    monImp->setEnabled(false);
    monImp->setToolTip("Monitoring the levels of Persistent Organic Pollutants (POPs) is essential for protecting both human health and the environment. These pollutants can accumulate in the food chain, leading to serious health problems. Regular monitoring helps identify contamination sources, track pollutant trends over time, and ensure compliance with regulations designed to limit exposure. It also enables early detection of hotspots, providing critical information to guide environmental interventions and safeguard communities.");

    connect(loadGButton, SIGNAL(clicked()), this, SLOT(loadGraph()));
    connect(resultBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(POPHealthRisks(const QString &)));

    setLayout(mainlayout);
}

void POPWindow::createToolbar()
{
    // Create a QToolBar instance
    QToolBar *toolbar = new QToolBar();
    toolbar->setStyleSheet("QToolBar {spacing: 10px;}"); // Spacing between items in the toolbar
    
    // Adding widgets to the toolbar
    toolbar->addWidget(resultLabel);
    toolbar->addWidget(resultBox);
    toolbar->addWidget(loadGButton);
    toolbar->addWidget(healthRisks);
    toolbar->addWidget(monImp);

    // Add toolbar to the layout
    mainlayout->addWidget(toolbar);

}

void POPWindow::clearWidgets()
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

void POPWindow::POPHealthRisks(const QString &pop)
{ // this is used to fill in the roll-over pop-ups with appropriate information
  if (pop.contains("PCB", Qt::CaseInsensitive))
  {
    healthRisks->setToolTip("Polychlorinated biphenyls (PCBs) are a type of POP. Exposure to PCBs can lead to a variety of health problems, including cancer, reproductive problems, immune system problems, and neurological problems.");
  }
  else if (pop.contains("DDT", Qt::CaseInsensitive))
  {
    healthRisks->setToolTip("DDT, a persistent organic pollutant, has been linked to a range of health issues. Long-term exposure can lead to neurological disorders, reproductive problems, and increased cancer risk. Additionally, DDT has detrimental effects on the environment, especially for birds of prey, causing thinning of eggshells and reproductive failure.");
  }
  else if (pop.contains("Aldrin", Qt::CaseInsensitive))
  {
    healthRisks->setToolTip("Aldrin, a potent insecticide and a POP, poses significant health risks. Exposure can lead to neurological disorders, \nincluding tremors and seizures. It can also affect the liver and kidneys. Additionally, aldrin is highly toxic to aquatic life and can disrupt ecosystems.");
  }
}

void POPWindow::loadGraph()
{
    QString selectedUPOP = resultBox->currentText(); // these are to store each filter selected
    if (selectedUPOP == "") {
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

    if (selectedUPOP.contains("PCB", Qt::CaseInsensitive) || selectedUPOP.contains("PCB", Qt::CaseInsensitive)) {
        chartView->setToolTip("Safety Threshold Information\nCompliant: < 0.1 ug/l\nWarning: between 0.1 and 0.5 ug/l\nNon-Compliant: > 0.5 ug/l");
    }
    else{ 
        chartView->setToolTip("Safety Threshold Information\nCompliant: < 0.03 ug/l\nWarning: between 0.03 and 0.1 ug/l\nNon-Compliant: > 0.1 ug/l");
    }

    Sseries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    Sseries->setMarkerSize(10.0); // formatting for the scatter chart
    
    int locCol = 3;
    int UPOPCol = 5;
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
        QModelIndex UPOPIndex = proxy->index(row, UPOPCol);
        QModelIndex unitIndex = proxy->index(row, unitCol); // these are pointers created to get the data from each cell

        QString location = proxy->data(locationIndex).toString();
        QString result = proxy->data(resultIndex).toString();
        QString time = proxy->data(timeIndex).toString();
        QString UPOP_ = proxy->data(UPOPIndex).toString();
        QString unit = proxy->data(unitIndex).toString(); // these are accessing each pointer to get the data from each cell and make them into QStrings
        if (location == selectedLocation && UPOP_ == selectedUPOP) // this is to ensure that the data that is actually added to each series are the data that the user wants
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
        QString txt = QString("POP level: %1 %2").arg(yValue).arg(unit1);    
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
    axisY->setTitleText(QString("POP Level / %1").arg(unit1)); // XY axis initialising and formatting

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
    chart->setTitle(QString("Persistent Organic Pollutant Analysis of %1").arg(selectedLocation));
    chartView->setChart(chart);

}

QColor POPWindow::indicatorColor(double result)
{
    QString tmp = resultBox->currentText();
    if (tmp.contains("PCB", Qt::CaseInsensitive) || tmp.contains("DDT", Qt::CaseInsensitive)) // this is due to the fact that PCB and DDT safety levels are the same, eve nif the units are different in some readings, due to waters density = 1 kg/l
    {
        if (result < 0.1)
        {
        return Qt::green;
        }
        else if (0.1 < result < 0.5)
        {
        return QColor(255, 165, 0);
        }
        else
        {
        return Qt::red;
        }
    }
    else if (tmp.contains("Aldrin", Qt::CaseInsensitive))
    {
        if (result < 0.03)
        {
        return Qt::green;
        }
        else if (0.03 < result < 0.1)
        {
        return Qt::yellow;
        }
        else
        {
        return Qt::red;
        }
    }

    return Qt::black; // default return being black
}

void POPWindow::checkItemCt()
{
    clearWidgets(); // Clear existing widgets first

    if (resultBox->count() == 0) {
        QLabel *msg = new QLabel("Location doesn't have POPs\nPlease select another location");
        mainlayout->addWidget(msg);
    } else {
        QLabel *successMsg = new QLabel("Load Data And Configure Variables To View Graph");
        mainlayout->addWidget(successMsg);
    }
}
