#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "POPwindow.hpp"


POPWindow::POPWindow(): QMainWindow()
{
  setWindowTitle("");
  setMinimumSize(1000, 500);
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
  QStatusBar* status = statusBar();
  status->addWidget(fileInfo);
  status->addWidget(dataInfo);
}

void POPWindow::addFileMenu()
{
  openAction = new QAction("&Open...", this);
  openAction->setShortcut(QKeySequence::Open);
  openAction->setStatusTip("Open a CSV file");
  connect(openAction, SIGNAL(triggered()), this, SLOT(setDataLoc()));

  closeAction = new QAction("Quit", this);
  closeAction->setShortcut(QKeySequence::Close);
  closeAction->setStatusTip("Quit the application");
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(openAction);
  fileMenu->addAction(closeAction);
}

void POPWindow::setDataLoc()
{
  QString path = QFileDialog::getOpenFileName(
    this, 
    "Open CSV File", 
    ".", 
    "CSV files (*.csv)");

  if (path.isEmpty()) {
    QMessageBox::critical(this, "No File Selected", "Please select a valid CSV file");
    return;
  }

  fileInfo->setText(QFileInfo(path).fileName());
  
  if (path.length() > 0) {
    filename = path;
  }

}

void POPWindow::createButtons()
{
  loadButton = new QPushButton("Load Data");
  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadCSV()));

  QStringList POPopts = {"", "PCB", "DDT", "Aldrin"};
  POPbox = new QComboBox(this);
  POPbox->addItems(POPopts);
  connect(POPbox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(filterPOP(const QString&)));  
  connect(POPbox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(POPHealthRisks(const QString&)));  
  connect(POPbox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(POPSafetyInfo(const QString&)));  
 
  resultBox = new QComboBox(this);
  connect(resultBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(filterUPOP(const QString&)));  

  locBox = new QComboBox(this);
  connect(locBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(filterLoc(const QString&)));  

  loadGButton = new QPushButton("Load Graph");
  connect(loadGButton, SIGNAL(clicked()), this, SLOT(loadGraph()));
  
  healthRisks = new QPushButton("Health Risks");
  healthRisks->setEnabled(false);
  healthRisks->setToolTip("Persistent Organic Pollutants (POPs) are harmful chemicals that can accumulate in the environment and human bodies. Exposure to POPs can lead to various health issues, including cancer, reproductive problems, weakened immunity, neurological disorders, and endocrine disruption.");
  monImp = new QPushButton("Monitoring Importance");
  monImp->setEnabled(false);
  monImp->setToolTip("Monitoring the levels of Persistent Organic Pollutants (POPs) is essential for protecting both human health and the environment. These pollutants can accumulate in the food chain, leading to serious health problems. Regular monitoring helps identify contamination sources, track pollutant trends over time, and ensure compliance with regulations designed to limit exposure. It also enables early detection of hotspots, providing critical information to guide environmental interventions and safeguard communities.");
  safetyLvl = new QPushButton("Safety Levels");
  safetyLvl->setEnabled(false);
}

void POPWindow::createToolbar()
{
  QToolBar* toolbar = new QToolBar();
  QLabel* POPlabel = new QLabel("Choose POP:");
  QLabel* resultLabel = new QLabel("CSV Results:");
  QLabel* locLabel = new QLabel("Location:");

  toolbar->addWidget(loadButton);

  POPlabel->setAlignment(Qt::AlignVCenter);
  toolbar->addWidget(POPlabel);
  toolbar->addWidget(POPbox);

  resultLabel->setAlignment(Qt::AlignVCenter);
  toolbar->addWidget(resultLabel);
  toolbar->addWidget(resultBox);

  locLabel->setAlignment(Qt::AlignVCenter);
  toolbar->addWidget(locLabel);
  toolbar->addWidget(locBox);

  toolbar->addWidget(loadGButton);

  toolbar->addSeparator();

  toolbar->addWidget(healthRisks);
  toolbar->addWidget(monImp);
  toolbar->addWidget(safetyLvl);

  addToolBar(Qt::LeftToolBarArea, toolbar);
}

void POPWindow::createMainWidget()
{
  proxy = new QSortFilterProxyModel();
  proxy->setSourceModel(&tablemodel);

  QLabel *blank = new QLabel("Load Data And Configure Vairables To View Graph");
  blank->setAlignment(Qt::AlignHCenter);
  setCentralWidget(blank); 
}

void POPWindow::loadCSV()
{ 
  if (filename.length() == 0){
    QMessageBox::critical(this, "Cannot Load CSV File", "Please select a CSV file");
    return;
  }
  
  try {
    tablemodel.updateFromFile(filename);
  }
  catch (const std::exception& error) {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }
  
  dataInfo->setText("Data Loaded!");
}

void POPWindow::filterPOP(const QString& POPname)
{
  resultBox->clear();
  locBox->clear();
  int DLcol = 5;

  proxy->setFilterKeyColumn(DLcol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(POPname);

  resultBox->clear();

  QSet<QString> uniquePOP;
  for (int row = 0; row<proxy->rowCount(); ++row){
    QModelIndex index = proxy->index(row, DLcol);
    QString lab = proxy->data(index).toString();
    uniquePOP.insert(lab);
  }

  resultBox->addItems(uniquePOP.values());


}

void POPWindow::filterUPOP(const QString& uPOPname)
{
  locBox->clear();
  uPOP = new QString(uPOPname);
  int DLcol = 5;
  int locCol = 3;

  proxy->setFilterKeyColumn(DLcol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(uPOPname);

  QSet<QString> loc;
  for (int row = 0; row<proxy->rowCount(); ++row){
    QModelIndex index = proxy->index(row, locCol);
    QString lab = proxy->data(index).toString();
    loc.insert(lab);
  }

  locBox->addItems(loc.values());

}

void POPWindow::filterLoc(const QString& location)
{
  int locCol = 3;
  loc_ = new QString(location);

  proxy->setFilterKeyColumn(locCol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(location);

}

void POPWindow::loadGraph()
{
    setCentralWidget(new QWidget); 

    QChart *chart = new QChart();
    QString unit1;
    QScatterSeries *Sseries = new QScatterSeries();
    QLineSeries *Lseries = new QLineSeries();

    Sseries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    Sseries->setMarkerSize(10.0);

    int locCol = 3;
    int UPOPCol = 5;
    int resCol = 9;
    int timeCol = 4;
    int unitCol = 11;

    QString selectedLocation = locBox->currentText();
    QString selectedUPOP = resultBox->currentText();

    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::lowest();

    qint64 timestamp;
    double lastRes;

    for (int row = 0; row < proxy->rowCount(); ++row) {
        QModelIndex locationIndex = proxy->index(row, locCol);
        QModelIndex resultIndex = proxy->index(row, resCol);
        QModelIndex timeIndex = proxy->index(row, timeCol);
        QModelIndex UPOPIndex = proxy->index(row, UPOPCol);
        QModelIndex unitIndex = proxy->index(row, unitCol);

        QString location = proxy->data(locationIndex).toString();
        QString result = proxy->data(resultIndex).toString();
        QString time = proxy->data(timeIndex).toString();
        QString UPOP_ = proxy->data(UPOPIndex).toString();
        QString unit = proxy->data(unitIndex).toString();

        if (location == selectedLocation && UPOP_ == selectedUPOP) {
            bool ok;
            double resultValue = result.toDouble(&ok);

            if (ok) {
                QDateTime datetime = QDateTime::fromString(time, "yyyy-MM-ddTHH:mm:ss");
                datetime.setTimeSpec(Qt::LocalTime);
                if (datetime.isValid()){
                  timestamp = datetime.toMSecsSinceEpoch();
                  Sseries->append(timestamp, resultValue);
                  Lseries->append(timestamp, resultValue);
                  unit1 = unit;

                  if (resultValue < minValue) minValue = resultValue;
                  if (resultValue > maxValue) maxValue = resultValue;

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
    axisY->setTitleText(QString("POP Level / %1").arg(unit1));

    QColor indicator = indicatorColor(lastRes);
    Lseries->setColor(indicator);
    Sseries->setColor(indicator);
    
    double valuePadding = (maxValue - minValue) * 0.15;

    if (valuePadding == 0)
    {
      QPointF pt = Sseries->at(0);
      qreal y = pt.y();
      valuePadding = y * 0.15;
    }

    int numPt = Sseries->count();

    if (numPt == 1)
    {
      QDateTime min = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(timestamp - (timestamp*0.15)));
      QDateTime max = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(timestamp + (timestamp*0.15)));

      axisX->setRange(min, max);
    }

    axisY->setRange(minValue - valuePadding, maxValue + valuePadding);
    
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

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);
}

void POPWindow::POPHealthRisks(const QString& pop)
{
  if (pop == "PCB") {
    healthRisks->setToolTip("Polychlorinated biphenyls (PCBs) are a type of POP. Exposure to PCBs can lead to a variety of health problems, including cancer, reproductive problems, immune system problems, and neurological problems.");
  } else if (pop == "DDT") {
    healthRisks->setToolTip("DDT, a persistent organic pollutant, has been linked to a range of health issues. Long-term exposure can lead to neurological disorders, reproductive problems, and increased cancer risk. Additionally, DDT has detrimental effects on the environment, especially for birds of prey, causing thinning of eggshells and reproductive failure.");
  } else if (pop == "Aldrin") {
    healthRisks->setToolTip("Aldrin, a potent insecticide and a POP, poses significant health risks. Exposure can lead to neurological disorders, including tremors and seizures. It can also affect the liver and kidneys. Additionally, aldrin is highly toxic to aquatic life and can disrupt ecosystems.");
  }

}

QColor POPWindow::indicatorColor(double result)
{
  if (POPbox->currentText() == "PCB" || POPbox->currentText() == "DDT") {
    if (result < 0.1){
      return Qt::green;
    }
    else if (0.1 < result < 0.5){
      return QColor(255,165,0);
    } 
    else {
      return Qt::red;
    }    
  } else if (POPbox->currentText() == "Aldrin") {
    if (result < 0.03){
      return Qt::green;
    }
    else if (0.03 < result < 0.1){
      return Qt::yellow;
    } 
    else {
      return Qt::red;
    }  
  }

  return Qt::black;
}

void POPWindow::POPSafetyInfo(const QString& pop)
{
  if (pop == "PCB") {
    safetyLvl->setToolTip("Safety Threshold Information\nCompliant: < 0.1 ug/l\nWarning: between 0.1 and 0.5 ug/l\nNon-Compliant: > 0.5 ug/l");
  } else if (pop == "DDT") {
    safetyLvl->setToolTip("Safety Threshold Information\nCompliant: < 0.1 ug/l\nWarning: between 0.1 and 0.5 ug/l\nNon-Compliant: > 0.5 ug/l");
  } else {
    safetyLvl->setToolTip("Safety Threshold Information\nCompliant: < 0.03 ug/l\nWarning: between 0.03 and 0.1 ug/l\nNon-Compliant: > 0.1 ug/l");
  }
}