#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "FCwindow.hpp" // these are the includes needed for the FC page

int subWidth = 150;

FCWindow::FCWindow() : QWidget()
{
  setWindowTitle("Water Quality Monitor");

  // setting the intial layout and the file menu

  createWidgets();
  arrangeWidgets();

}

void FCWindow::setDL(const QString &file) { filename = file; } // setting the data location

void FCWindow::createWidgets()
{
  blank = new QLabel("Load Data And Configure Vairables To View Graph"); // placeholder for when graph isn't initially loaded

  loadButton = new QPushButton("Load Data");
  loadButton->setFixedWidth(subWidth);
  
  PFASLabel = new QLabel("PFAS type:");
  PFASLabel->setFixedWidth(subWidth);
  PFASBox = new QComboBox(this);
  PFASBox->setFixedWidth(subWidth);

  locLabel = new QLabel("Location:");
  locLabel->setFixedWidth(subWidth);
  locBox = new QComboBox(this);
  locBox->setFixedWidth(subWidth);
  
  loadGButton = new QPushButton("Load Graph");
  loadGButton->setFixedWidth(subWidth);
  
  healthInfo = new QPushButton("Health\nInformation");
  healthInfo->setEnabled(false);
  healthInfo->setToolTip("");
  healthInfo->setFixedWidth(subWidth);

  envInfo = new QPushButton("Environmental\nInformation");
  envInfo->setEnabled(false);
  envInfo->setToolTip("");
  envInfo->setFixedWidth(subWidth);

  safetyLvl = new QPushButton("Safety Levels");
  safetyLvl->setToolTip("");
  safetyLvl->setEnabled(false); // these previous lines make the buttons unclickeable and adds information to the button's tooltips
  safetyLvl->setFixedWidth(subWidth);

  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadCSV()));
  connect(PFASBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(filterPFAS(const QString &))); // this further specifies user preference and populates the location box
  connect(locBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(filterLoc(const QString &))); // this sets the users preference of location filtering
  connect(loadGButton, SIGNAL(clicked()), this, SLOT(loadGraph()));

}

void FCWindow::arrangeWidgets()
{
    mainLayout = new QHBoxLayout(this);
    subLayout = new QVBoxLayout();

    subLayout->addWidget(loadButton);
    subLayout->addWidget(PFASLabel);
    subLayout->addWidget(PFASBox);
    subLayout->addWidget(locLabel);
    subLayout->addWidget(locBox);
    subLayout->addWidget(loadGButton);
    subLayout->addWidget(healthInfo);
    subLayout->addWidget(envInfo);
    subLayout->addWidget(safetyLvl);

    subLayout->addStretch(); // Adds a stretchable space to absorb extra vertical space

    mainLayout->addLayout(subLayout);
    mainLayout->addWidget(blank);

    setLayout(mainLayout);
}

void FCWindow::populatePFAS() // this basically filters the proxy for the searchTerm and then populates all the unique results in the PFAS box
{
  int DDcol = 6;
  QString searchTerm = QString("perfluoro");
  proxy->setFilterKeyColumn(DDcol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(searchTerm);

  QSet<QString> uniquePFAS;
  for (int row = 0; row < proxy->rowCount(); ++row)
  {
    QModelIndex index = proxy->index(row, DDcol-1);
    QString lab = proxy->data(index).toString();
    uniquePFAS.insert(lab);
  }

  PFASBox->addItems(uniquePFAS.values());
}

QColor FCWindow::indicatorColor(double result) // this is the function which determines if the last result is compliant or not, using the traffic light system
{
  if (result < 0.02) {
    return Qt::green;
  } else if (0.02 < result < 0.1) {
    return QColor(255, 165, 0);
  } else {
    return Qt::red;
  }

  return Qt::black; // default return being black
}

void FCWindow::removeRightmostWidget()
{
    // Get the number of items in the layout
    int itemCount = mainLayout->count();

    // If there are any items in the layout
    if (itemCount > 1) {
        // Get the last item in the layout
        QLayoutItem *lastItem = mainLayout->itemAt(itemCount - 1);

        // Check if the item is a widget
        if (lastItem && lastItem->widget()) {
            QWidget *lastWidget = lastItem->widget();
            mainLayout->removeWidget(lastWidget);  // Remove the widget from the layout
            lastWidget->deleteLater();             // Delete the widget (optional)
        }
    }
}

//below this commment are slots

void FCWindow::loadCSV()
{
  if (filename.length() == 0) // if no file is selected
  {
    QMessageBox::critical(this, "Cannot Load CSV File", "Please select a CSV file");
    return;
  }

  try // using tablemodel to read csv
  {
    tablemodel.updateFromFile(filename);
  }
  catch (const std::exception &error) // if there is any errors
  {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  proxy = new QSortFilterProxyModel(this);
  proxy->setSourceModel(&tablemodel);

  populatePFAS();
}

void FCWindow::filterPFAS(const QString &pfas) // this filters the proxy model for the specific type of PFAS the user wants to see
{
  locBox->clear(); // clears the previous location box data
  int DLcol = 5;
  int locCol = 3;

  proxy->setFilterKeyColumn(DLcol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(pfas);
  QSet<QString> loc;
  for (int row = 0; row < proxy->rowCount(); ++row)
  {
    QModelIndex index = proxy->index(row, locCol);
    QString lab = proxy->data(index).toString();
    loc.insert(lab);
  }

  locBox->addItems(loc.values()); 
}

void FCWindow::filterLoc(const QString &location) // this filters for the specific sampling point the user wants to see the graph of
{

  int locCol = 3;

  proxy->setFilterKeyColumn(locCol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(location); // this is the final filter, now the graph is ready to be plotted
}

void FCWindow::loadGraph()
{
  removeRightmostWidget(); // removes old chart / blank label

  chartView = new QChartView(); // initialising the ChartView
  chartView->setRenderHint(QPainter::Antialiasing);
  mainLayout->addWidget(chartView);

  QChart *chart = new QChart();
  QString unit1; // this stores the units used, for Y axis title
  QScatterSeries *Sseries = new QScatterSeries();
  QLineSeries *Lseries = new QLineSeries(); // a scatter series and a line series is used to show each individual data point and the line that adjoins them

  Sseries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  Sseries->setMarkerSize(10.0); // formatting for the scatter chart

  int locCol = 3;
  int DLCol = 5;
  int resCol = 9;
  int timeCol = 4;
  int unitCol = 11; // these represent column indexes for each attribute used

  QString selectedLocation = locBox->currentText();
  QString selectedDL = PFASBox->currentText(); // these are to store each filter selected

  double minValue = std::numeric_limits<double>::max();
  double maxValue = std::numeric_limits<double>::lowest(); // these are going to be used for the graph padding, to make the data more presentable

  qint64 timestamp; // used for the processing of the sampling time
  double lastRes;   // used to store the last result, this point is used to colour the chart


  for (int row = 0; row < proxy->rowCount(); ++row)
  {
    QModelIndex locationIndex = proxy->index(row, locCol);
    QModelIndex resultIndex = proxy->index(row, resCol);
    QModelIndex timeIndex = proxy->index(row, timeCol);
    QModelIndex DLIndex = proxy->index(row, DLCol);
    QModelIndex unitIndex = proxy->index(row, unitCol); // these are pointers created to get the data from each cell

    QString location = proxy->data(locationIndex).toString();
    QString result = proxy->data(resultIndex).toString();
    QString time = proxy->data(timeIndex).toString();
    QString DL_ = proxy->data(DLIndex).toString();
    QString unit = proxy->data(unitIndex).toString(); // these are accessing each pointer to get the data from each cell and make them into QStrings

    if (location == selectedLocation && DL_ == selectedDL) // this is to ensure that the data that is actually added to each series are the data that the user wants
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
      // std::cout << resultValue << ", " << time.toStdString() << ", " << location.toStdString() << ", " << DL_.toStdString() << ", " << unit.toStdString() << "\n";
    }
  }

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
  Lseries->attachAxis(axisY); // adding axis and series to chart

  chart->legend()->hide();
  chart->setTitle(QString("PFAS Analysis of %1").arg(selectedLocation));
  chartView->setChart(chart);

}