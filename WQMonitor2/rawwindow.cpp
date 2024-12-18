#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "rawwindow.hpp"
#include "filterProxy.hpp"

RawWindow::RawWindow(): QMainWindow()
{
  dayNum = 0;
  monthNum = 0;
  setWindowTitle("Water Quality Monitor");
  setMinimumSize(1000, 800);
  createMainWidget();
  createStatusBar();
  addFileMenu();
  addPageMenu();
  addLanguageMenu();
  createButtons();
  createToolbar();
}

int toolWidth = 125;

void RawWindow::createStatusBar()
{
  fileInfo = new QLabel(tr("Current file: <none>"));
  dataInfo = new QLabel(tr("Data not loaded"));
  QStatusBar* status = statusBar();
  status->addWidget(fileInfo);
  status->addWidget(dataInfo);
}

void RawWindow::addFileMenu()
{
  openAction = new QAction(tr("&Open..."), this);
  openAction->setShortcut(QKeySequence::Open);
  openAction->setStatusTip(tr("Open a CSV file"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(setDataLoc()));

  closeAction = new QAction(tr("Quit"), this);
  closeAction->setShortcut(QKeySequence::Close);
  closeAction->setStatusTip(tr("Quit the application"));
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAction);
  fileMenu->addAction(closeAction);
}

void RawWindow::setDataLoc()
{
  QString path = QFileDialog::getOpenFileName(
    this, 
    "Open CSV File", 
    ".", 
    "CSV files (*.csv)");

  if (path.isEmpty()) {
    QMessageBox::critical(this, tr("No File Selected"), tr("Please select a valid CSV file"));
    return;
  }

  fileInfo->setText(QFileInfo(path).fileName());
  
  if (path.length() > 0) {
    filename = path;
  }
  
}

void RawWindow::createButtons()
{
  homeButton = new QPushButton(tr("Main Menu"));
  connect(homeButton, &QPushButton::clicked, this, [this]() { setPage("home"); });

  loadButton = new QPushButton(tr("Load Data"));
  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadCSV()));

  // Location menu
  locBox = new QComboBox();
  connect(locBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(filterLoc(const QString&)));
  // Connect to all pages

  // Month
  monthBox = new QComboBox(this);
  monthBox->addItem(tr("None"));
  monthBox->addItem(tr("January"));
  monthBox->addItem(tr("Febuary"));
  monthBox->addItem(tr("March"));
  monthBox->addItem(tr("April"));
  monthBox->addItem(tr("May"));
  monthBox->addItem(tr("June"));
  monthBox->addItem(tr("July"));
  monthBox->addItem(tr("August"));
  monthBox->addItem(tr("September"));
  monthBox->addItem(tr("October"));
  monthBox->addItem(tr("November"));
  monthBox->addItem(tr("December"));

  connect(monthBox, &QComboBox::currentTextChanged, this, &RawWindow::setMonth);

  dayBox = new QSpinBox(this);

  // Configure the spin box
  dayBox->setRange(0, 31); // Allow numbers between 0 and 31
  dayBox->setValue(0);     // Default value
  dayBox->setPrefix(tr("Day: ")); // Optional: Add "Day: " prefix to the value

  connect(dayBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &RawWindow::setDay);

}

void RawWindow::setMonth(const QString& month) {
  
  if (month == "None") {monthNum = 0;}
  else if (month == "January") {monthNum = 1;}
  else if (month == "Febuary") {monthNum = 2;}
  else if (month == "March") {monthNum = 3;}
  else if (month == "April") {monthNum = 4;}
  else if (month == "May") {monthNum = 5;}
  else if (month == "June") {monthNum = 6;}
  else if (month == "July") {monthNum = 7;}
  else if (month == "August") {monthNum = 8;}
  else if (month == "September") {monthNum = 9;}
  else if (month == "October") {monthNum = 10;}
  else if (month == "November") {monthNum = 11;}
  else if (month == "December") {monthNum = 12;}

  this->filterDate();
}

void RawWindow::setDay(int day) {
  dayNum = day;
  this->filterDate();
}

void RawWindow::createToolbar()
{
  if (toolbarAlt)
  {
    removeToolBar(toolbarAlt);
  }
  if (toolbar)
  {
    removeToolBar(toolbar);
  }

  toolbar = new QToolBar();

  filterLocLabel = new QLabel(tr("Filter by Location:"));
  QLabel* filterDateLabel = new QLabel(tr("Filter by Date:"));

  filterLocLabel->setStyleSheet("QLabel { color : white; }");
  filterDateLabel->setStyleSheet("QLabel { color : white; }");
  
  homeButton->setFixedWidth(toolWidth);
  toolbar->addWidget(homeButton);

  loadButton->setFixedWidth(toolWidth);
  toolbar->addWidget(loadButton);
  
  toolbar->addSeparator();
  
  filterLocLabel->setFixedWidth(toolWidth);
  toolbar->addWidget(filterLocLabel);
  locBox->setFixedWidth(toolWidth);
  toolbar->addWidget(locBox);

  toolbar->addSeparator();
  filterDateLabel->setFixedWidth(toolWidth);
  toolbar->addWidget(filterDateLabel);
  monthBox->setFixedWidth(toolWidth);
  toolbar->addWidget(monthBox);
  dayBox->setFixedWidth(toolWidth);
  toolbar->addWidget(dayBox);

  toolbar->addSeparator();

  addToolBar(Qt::LeftToolBarArea, toolbar);
}

void RawWindow::createToolbarAlt()
{
  if (toolbar)
  {
    removeToolBar(toolbar);
  }
  if (toolbarAlt)
  {
    removeToolBar(toolbarAlt);
  }

  toolbarAlt = new QToolBar();

  filterLocLabel = new QLabel(tr("Filter by Location:"));

  filterLocLabel->setStyleSheet("QLabel { color : white; }");

  homeButton->setFixedWidth(toolWidth);
  toolbarAlt->addWidget(homeButton);

  loadButton->setFixedWidth(toolWidth);
  toolbarAlt->addWidget(loadButton);
  
  toolbarAlt->addSeparator();
  filterLocLabel->setFixedWidth(toolWidth);
  toolbarAlt->addWidget(filterLocLabel);
  locBox->setFixedWidth(toolWidth);
  toolbarAlt->addWidget(locBox);


  addToolBar(Qt::LeftToolBarArea, toolbarAlt);
}

void RawWindow::createMainWidget()
{
  proxy = new filterProxy();

  proxy->setSourceModel(&tablemodel);
  proxy->setDateCol(4);       // Date column
  proxy->setLocationCol(3);   // Location column

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

  homePageWidget = new homePage(this);
  connect(homePageWidget, &homePage::buttonClicked, this, &RawWindow::setPage);

  page1 = new PollutantOverviewWindow();
  page1->setModel(proxy);

  page5 = new ComplianceWindow();
  page5->setProxy(proxy);

  page2 = new POPWindow();

  page3 = new LWindow();

  page4 = new FCWindow();
  
  // Raplace tables with custom widgets
  table1 = new QTableView();
  table1->setModel(proxy);
  table1->setFont(tableFont);
 
  table2 = new QTableView();
  table2->setModel(proxy);
  table2->setFont(tableFont);

  table3 = new QTableView();
  table3->setModel(proxy);
  table3->setFont(tableFont);

  table4 = new QTableView();
  table4->setModel(proxy);
  table4->setFont(tableFont);

  setCentralWidget(homePageWidget); 
}

void RawWindow::loadCSV()
{ 
  
  if (filename.length() == 0){
    QMessageBox::critical(this, tr("Cannot Load CSV File"), tr("Please select a CSV file"));
    return;
  }
  
  try {
    tablemodel.updateFromFile(filename);
  }
  catch (const std::exception& error) {
    QMessageBox::critical(this, tr("CSV File Error"), error.what());
    return;
  }
  
  page2->updateFile(tablemodel);
  page4->updateFile(tablemodel);
  page3->updateFile(tablemodel);
  
  dataInfo->setText(tr("Data Loaded!"));

  locBox->clear();
  locBox->addItem(QString(" "));
  int locCol = 3;
  
  QStringList loc;
  for (int row = 0; row<proxy->rowCount(); ++row){
    QModelIndex index = proxy->index(row, locCol);
    QString lab = proxy->data(index).toString();
    if(!loc.contains(lab)) loc.append(lab);
    
  }
  std::sort(loc.begin(), loc.end());

  int dateCol = 4;
  locBox->addItems(loc);
  page1->updateWindow();
  page5->updateWindow();
  
}

void RawWindow::filterLoc(const QString& location)
{
  int locCol = 3;

  //proxy->setFilterKeyColumn(locCol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  //proxy->setFilterFixedString(location);
  proxy->setLocationFilter(location);

  dataInfo->setText(QString("Filtered by Location: %1").arg(location));
  page1->updateWindow();
  page5->updateWindow();
  page2->populateResultsBox(location);
  page3->populateWTBox(location);
  page4->populateResultsBox(location);
}

void RawWindow::filterDate()
{

  proxy->setDateFilter(monthNum,dayNum);

    QString info = tr("Filtered by Date: ");
    if (monthNum != 0) {
        info += QString("Month = %1 ").arg(monthNum);
    }
    if (dayNum != 0) {
        info += QString("Day = %1 ").arg(dayNum);
    }
    if (monthNum == 0 && dayNum == 0) {
        info += tr("No filtering applied");
    }

    dataInfo->setText(info);
    page1->updateWindow();
    page5->updateWindow();
}

void RawWindow::addPageMenu()
{
  QMenu* pageMenu = menuBar()->addMenu("&Page");
   // Add actions to the menu
  QAction *page1Action = pageMenu->addAction(tr("Pollutant Overview"));
  QAction *page2Action = pageMenu->addAction(tr("Persistent Organic Pollutants (POPs)"));
  QAction *page3Action = pageMenu->addAction(tr("Environmental Litter Indicators"));
  QAction *page4Action = pageMenu->addAction(tr("Fluorinated Compounds"));
  QAction *page5Action = pageMenu->addAction(tr("Compliance Dashboard"));
  QAction *page6Action = pageMenu->addAction(tr("Raw Data"));

  // Connect actions to the setDate slot
  connect(page1Action, &QAction::triggered, this, [this]() { setPage("Page1"); });
  connect(page2Action, &QAction::triggered, this, [this]() { setPage("Page2"); });
  connect(page3Action, &QAction::triggered, this, [this]() { setPage("Page3"); });
  connect(page4Action, &QAction::triggered, this, [this]() { setPage("Page4"); });
  connect(page5Action, &QAction::triggered, this, [this]() { setPage("Page5"); });
  connect(page6Action, &QAction::triggered, this, [this]() { setPage("Page6"); });

}

void RawWindow::addLanguageMenu() {
  QMenu* languageMenu = menuBar()->addMenu("&Language");
  QAction *engAction = languageMenu->addAction("English");
  QAction *frAction = languageMenu->addAction("French");

  // Connect actions to the setLanguage slot
  connect(engAction, &QAction::triggered, this, [this]() { setLanguage("English"); });
  connect(frAction, &QAction::triggered, this, [this]() { setLanguage("French"); });
}

void RawWindow::setLanguage(const QString& lan) {
    static QTranslator translator; // Static to persist across calls

    if (lan == "English") {
        QApplication::instance()->removeTranslator(&translator); // Uninstall translator for default language
    } else if (lan == "French") {
        if (translator.load(":/translations/french.qm")) { // Ensure the file is present in resources
            QApplication::instance()->installTranslator(&translator);
        } else {
            qWarning() << "Failed to load French translation.";
        }
    }
}


// Central widget must be cleared before setting a new one
void RawWindow::clearCentralWidget() {
   QWidget *oldWidget = centralWidget();
    if (oldWidget) {
        oldWidget->setParent(nullptr); // Detach the old widget
    }
}

void RawWindow::setPage(const QString& page){
  clearCentralWidget();
  
  if (page == "Page2" || page == "Page4" || page == "Page3") {
    createToolbarAlt();
  } else {
    createToolbar();
  }
  
  if (page == "Page1") {
    setCentralWidget(page1);
  }
  else if (page == "Page2") {
    setCentralWidget(page2);
  }
  else if (page == "Page3") {
    setCentralWidget(page3);
  }
  else if (page == "Page4") {
    setCentralWidget(page4);
  }
  else if (page == "Page5") {
    setCentralWidget(page5);
  }
  else if (page == "Page6") {
    setCentralWidget(table1);
  }
  else {
    setCentralWidget(homePageWidget);
  }
}







// @id, sample.samplingPoint , sample.samplingPoint.notation , sample.samplingPoint.label , 
// sample.sampleDateTime , determinand.label , determinand.definition , determinand.notation
// , resultQualifier.notation , result,codedResultInterpretation.interpretation,determinand.unit.label,sample.sampledMaterialType.label,sample.isComplianceSample,sample.purpose.label,sample.samplingPoint.easting,sample.samplingPoint.northing

