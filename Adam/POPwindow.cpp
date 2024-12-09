#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "POPwindow.hpp" // these are the includes needed for the POP page

POPWindow::POPWindow() : QMainWindow()
{
  setWindowTitle("Water Quality Monitor");
  setMinimumSize(1000, 500);

  // setting the intial layout and the file menu
  createMainWidget();
  createStatusBar();
  addFileMenu();
  createButtons();
  createToolbar();
}

void POPWindow::createStatusBar()
{
  fileInfo = new QLabel("Current file: <none>");
  dataInfo = new QLabel("Data not loaded");
  QStatusBar *status = statusBar();
  status->addWidget(fileInfo);
  status->addWidget(dataInfo); // sets status bar up with filename information and the information around if data is loaded or not
}

void POPWindow::addFileMenu()
{
  openAction = new QAction("&Open...", this);
  openAction->setShortcut(QKeySequence::Open);
  openAction->setStatusTip("Open a CSV file");
  connect(openAction, SIGNAL(triggered()), this, SLOT(setDataLoc())); // this activates the setDataLoc slot for picking out a file

  closeAction = new QAction("Quit", this);
  closeAction->setShortcut(QKeySequence::Close);
  closeAction->setStatusTip("Quit the application");
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close())); // this closes the program

  fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(openAction);
  fileMenu->addAction(closeAction); // adds the actions under the file menu
}

void POPWindow::setDataLoc()
{
  QString path = QFileDialog::getOpenFileName(
      this,
      "Open CSV File",
      ".",
      "CSV files (*.csv)"); // this is the pop up box used to pick out a file of type .csv

  if (path.isEmpty()) // if no file is selected
  {
    QMessageBox::critical(this, "No File Selected", "Please select a valid CSV file");
    return;
  }

  fileInfo->setText(QFileInfo(path).fileName()); // status bar updated

  if (path.length() > 0)
  {
    filename = path;
  } // filename variable is set to path, for processing
}

void POPWindow::createButtons()
{
  loadButton = new QPushButton("Load Data");
  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadCSV()));

  QStringList POPopts = {"", "PCB", "DDT", "Aldrin"}; // options for initial filtering
  POPbox = new QComboBox(this);
  POPbox->addItems(POPopts);
  connect(POPbox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(filterPOP(const QString &))); // this sets the users preference of POP, and populates the uPOP box
  connect(POPbox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(POPHealthRisks(const QString &)));
  connect(POPbox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(POPSafetyInfo(const QString &))); // these slots are used for the popups

  resultBox = new QComboBox(this);
  connect(resultBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(filterUPOP(const QString &))); // this further specifies user preference and populates the location box

  locBox = new QComboBox(this);
  connect(locBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(filterLoc(const QString &))); // this sets the users preference of location filtering

  loadGButton = new QPushButton("Load Graph");
  connect(loadGButton, SIGNAL(clicked()), this, SLOT(loadGraph()));

  healthRisks = new QPushButton("Health Risks");
  healthRisks->setEnabled(false);
  healthRisks->setToolTip("Persistent Organic Pollutants (POPs) are harmful chemicals that can accumulate in the environment and human bodies. Exposure to POPs can lead to various health issues, including cancer, reproductive problems, weakened immunity, neurological disorders, and endocrine disruption.");
  monImp = new QPushButton("Monitoring Importance");
  monImp->setEnabled(false);
  monImp->setToolTip("Monitoring the levels of Persistent Organic Pollutants (POPs) is essential for protecting both human health and the environment. These pollutants can accumulate in the food chain, leading to serious health problems. Regular monitoring helps identify contamination sources, track pollutant trends over time, and ensure compliance with regulations designed to limit exposure. It also enables early detection of hotspots, providing critical information to guide environmental interventions and safeguard communities.");
  safetyLvl = new QPushButton("Safety Levels");
  safetyLvl->setToolTip("Begin filtering to get safety level information");
  safetyLvl->setEnabled(false); // these previous lines make the buttons unclickeable and adds generic information to the popups
}

void POPWindow::createToolbar()
{
  QToolBar *toolbar = new QToolBar();
  QLabel *POPlabel = new QLabel("Choose POP:");
  QLabel *resultLabel = new QLabel("CSV Results:");
  QLabel *locLabel = new QLabel("Location:"); // initialising toolbar and requires labels

  toolbar->addWidget(loadButton); // this makes the load button at the top

  POPlabel->setAlignment(Qt::AlignVCenter);
  toolbar->addWidget(POPlabel);
  toolbar->addWidget(POPbox); // this adds the initial filter combo box

  resultLabel->setAlignment(Qt::AlignVCenter);
  toolbar->addWidget(resultLabel);
  toolbar->addWidget(resultBox); // this adds the unique POP results into a combo box

  locLabel->setAlignment(Qt::AlignVCenter);
  toolbar->addWidget(locLabel);
  toolbar->addWidget(locBox); // location filter box

  toolbar->addWidget(loadGButton); // add the load graph button

  toolbar->addSeparator(); // this is just for aesthetics

  toolbar->addWidget(healthRisks);
  toolbar->addWidget(monImp);
  toolbar->addWidget(safetyLvl); // rollover popups

  addToolBar(Qt::LeftToolBarArea, toolbar);
}

void POPWindow::createMainWidget()
{
  proxy = new QSortFilterProxyModel();
  proxy->setSourceModel(&tablemodel); // this creates the proxy object and sets it to the tablemodel, which will contain a table of all the data in csv

  QLabel *blank = new QLabel("Load Data And Configure Vairables To View Graph");
  blank->setAlignment(Qt::AlignHCenter);

  
  setCentralWidget(blank); // initial label set when graph is not present
}

void POPWindow::loadCSV()
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

  dataInfo->setText("Data Loaded!"); // this indicates if data is loaded from csv
}

void POPWindow::filterPOP(const QString &POPname)
{
  resultBox->clear();
  locBox->clear(); // clears previous filters
  int DLcol = 5;   // DL = Determinand Label, this is the index where the DL is stored

  proxy->setFilterKeyColumn(DLcol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(POPname); // this sifts through each reccord to find if the POPname is contained in the DL column

  QSet<QString> uniquePOP;
  for (int row = 0; row < proxy->rowCount(); ++row)
  {
    QModelIndex index = proxy->index(row, DLcol);
    QString lab = proxy->data(index).toString();
    uniquePOP.insert(lab);
  }

  resultBox->addItems(uniquePOP.values()); // this adds the specific uPOP in the uPOP combo box
}

void POPWindow::filterUPOP(const QString &uPOPname)
{
  locBox->clear(); // clears the previous location box data
  int DLcol = 5;
  int locCol = 3;

  proxy->setFilterKeyColumn(DLcol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(uPOPname);

  QSet<QString> loc;
  for (int row = 0; row < proxy->rowCount(); ++row)
  {
    QModelIndex index = proxy->index(row, locCol);
    QString lab = proxy->data(index).toString();
    loc.insert(lab);
  }

  locBox->addItems(loc.values()); // populating the location combo box wiht sampling points
}

void POPWindow::filterLoc(const QString &location)
{
  int locCol = 3;
  loc_ = new QString(location);

  proxy->setFilterKeyColumn(locCol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(location); // this is the final filter, now the graph is ready to be plotted
}

void POPWindow::loadGraph()
{
  setCentralWidget(new QWidget);

  QChart *chart = new QChart();
  QString unit1; // this stores the units used, for Y axis title
  QScatterSeries *Sseries = new QScatterSeries();
  QLineSeries *Lseries = new QLineSeries(); // a scatter series and a line series is used to show each individual data point and the line that adjoins them

  Sseries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  Sseries->setMarkerSize(10.0); // formatting for the scatter chart

  int locCol = 3;
  int UPOPCol = 5;
  int resCol = 9;
  int timeCol = 4;
  int unitCol = 11; // these represent column indexes for each attribute used

  QString selectedLocation = locBox->currentText();
  QString selectedUPOP = resultBox->currentText(); // these are to store each filter selected

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
      // std::cout << resultValue << ", " << time.toStdString() << ", " << location.toStdString() << ", " << UPOP_.toStdString() << ", " << unit.toStdString() << "\n";
    }
  }

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

  axisY->setTitleText(QString("POP Level / %1").arg(unit1));
  chart->addSeries(Sseries);
  chart->addSeries(Lseries);
  chart->addAxis(axisX, Qt::AlignBottom);
  chart->addAxis(axisY, Qt::AlignLeft);
  Sseries->attachAxis(axisX);
  Lseries->attachAxis(axisX);
  Sseries->attachAxis(axisY);
  Lseries->attachAxis(axisY);

  chart->legend()->hide();
  chart->setTitle("Persistent Organic Pollutant Analysis");

  QChartView *chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  setCentralWidget(chartView);
}

void POPWindow::POPHealthRisks(const QString &pop)
{ // this is used to fill in the roll-over pop-ups with appropriate information
  if (pop == "PCB")
  {
    healthRisks->setToolTip("Polychlorinated biphenyls (PCBs) are a type of POP. Exposure to PCBs can lead to a variety of health problems, including cancer, reproductive problems, immune system problems, and neurological problems.");
  }
  else if (pop == "DDT")
  {
    healthRisks->setToolTip("DDT, a persistent organic pollutant, has been linked to a range of health issues. Long-term exposure can lead to neurological disorders, reproductive problems, and increased cancer risk. Additionally, DDT has detrimental effects on the environment, especially for birds of prey, causing thinning of eggshells and reproductive failure.");
  }
  else if (pop == "Aldrin")
  {
    healthRisks->setToolTip("Aldrin, a potent insecticide and a POP, poses significant health risks. Exposure can lead to neurological disorders, including tremors and seizures. It can also affect the liver and kidneys. Additionally, aldrin is highly toxic to aquatic life and can disrupt ecosystems.");
  }
}

QColor POPWindow::indicatorColor(double result)
{
  if (POPbox->currentText() == "PCB" || POPbox->currentText() == "DDT") // this is due to the fact that PCB and DDT safety levels are the same, eve nif the units are different in some readings, due to waters density = 1 kg/l
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
  else if (POPbox->currentText() == "Aldrin")
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

void POPWindow::POPSafetyInfo(const QString &pop)
{ // this is used to fill in the roll-over pop-ups with appropriate information
  if (pop == "PCB")
  {
    safetyLvl->setToolTip("Safety Threshold Information\nCompliant: < 0.1 ug/l\nWarning: between 0.1 and 0.5 ug/l\nNon-Compliant: > 0.5 ug/l");
  }
  else if (pop == "DDT")
  {
    safetyLvl->setToolTip("Safety Threshold Information\nCompliant: < 0.1 ug/l\nWarning: between 0.1 and 0.5 ug/l\nNon-Compliant: > 0.5 ug/l");
  }
  else
  {
    safetyLvl->setToolTip("Safety Threshold Information\nCompliant: < 0.03 ug/l\nWarning: between 0.03 and 0.1 ug/l\nNon-Compliant: > 0.1 ug/l");
  }
}