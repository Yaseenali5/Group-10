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
  setWindowTitle("");
  setMinimumSize(1000, 500);
  //createPages();
  createMainWidget();
  createStatusBar();
  addFileMenu();
  addPageMenu();
  addLanguageMenu();
  createButtons();
  createToolbar();
}

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
    QMessageBox::critical(this, "No File Selected", "Please select a valid CSV file");
    return;
  }

  fileInfo->setText(QFileInfo(path).fileName());
  
  if (path.length() > 0) {
    filename = path;
  }
}

void RawWindow::createButtons()
{
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
  monthBox->addItem(tr("Juli"));
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
  dayBox->setPrefix("Day: "); // Optional: Add "Day: " prefix to the value

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
  else if (month == "Juli") {monthNum = 7;}
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
  QToolBar* toolbar = new QToolBar();

  QLabel* loadLabel = new QLabel(tr("Load Data"));
  QLabel* filterLocLabel = new QLabel(tr("Filter by Location:"));
  QLabel* filterDateLabel = new QLabel(tr("Filter by Date:"));

  toolbar->addWidget(loadLabel);
  toolbar->addWidget(loadButton);
  
  toolbar->addSeparator();
  toolbar->addWidget(filterLocLabel);
  toolbar->addWidget(locBox);

  toolbar->addSeparator();
  toolbar->addWidget(filterDateLabel);
  toolbar->addWidget(monthBox);
  toolbar->addWidget(dayBox);

  toolbar->addSeparator();

  // Add cards
  card1Status = new QLabel("0",this);
  card2Status = new QLabel("0",this);
  card3Status = new QLabel("0",this);
  card4Status = new QLabel("0",this);
  toolbar->addWidget(createPollutantCard("Chloroform",card1Status, 10, 5, 15, "ml"));
  toolbar->addWidget(createPollutantCard("POPs",card2Status, 16, 5, 15, "ml"));
  toolbar->addWidget(createPollutantCard("Litter",card3Status, 4, 5, 15, "ml"));
  toolbar->addWidget(createPollutantCard("Fluorinated Compounds",card4Status, 10, 5, 15, "ml"));
  setButtonLabel(card3Status, 7, 5, 15); // Test of setButtonLabel

  addToolBar(Qt::LeftToolBarArea, toolbar);
}

void RawWindow::createMainWidget()
{
  proxy = new filterProxy();
  
  proxy->setSourceModel(&tablemodel);

  proxy->setDateCol(4);       // Date column
  proxy->setLocationCol(3);   // Location column

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

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

  setCentralWidget(table1); 
}

void RawWindow::loadCSV()
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
  
  
  dataInfo->setText(tr("Data Loaded!"));

  locBox->clear();
  locBox->addItem(QString(" ")); // ------------------------------------------------------
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
  // Update page 1 with loaded data
  
}

void RawWindow::filterLoc(const QString& location)
{
  int locCol = 3;

  //proxy->setFilterKeyColumn(locCol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  //proxy->setFilterFixedString(location);
  proxy->setLocationFilter(location);

  dataInfo->setText(QString("Filtered by Location: %1").arg(location));
}

void RawWindow::filterDate()
{

  proxy->setDateFilter(monthNum,dayNum);

    QString info = "Filtered by Date: ";
    if (monthNum != 0) {
        info += QString("Month = %1 ").arg(monthNum);
    }
    if (dayNum != 0) {
        info += QString("Day = %1 ").arg(dayNum);
    }
    if (monthNum == 0 && dayNum == 0) {
        info += "No filtering applied";
    }

    dataInfo->setText(info);
}










void RawWindow::addPageMenu()
{
  QMenu* pageMenu = menuBar()->addMenu("&Page");
   // Add actions to the menu
  QAction *page1Action = pageMenu->addAction("Page1");
  QAction *page2Action = pageMenu->addAction("Page2");
  QAction *page3Action = pageMenu->addAction("Page3");
  QAction *page4Action = pageMenu->addAction("Page4");

  // Connect actions to the setDate slot
  connect(page1Action, &QAction::triggered, this, [this]() { setPage("Page1"); });
  connect(page2Action, &QAction::triggered, this, [this]() { setPage("Page2"); });
  connect(page3Action, &QAction::triggered, this, [this]() { setPage("Page3"); });
  connect(page4Action, &QAction::triggered, this, [this]() { setPage("Page4"); });

}

void RawWindow::addLanguageMenu() {
  
  QMenu* languageMenu = menuBar()->addMenu("&Language");
  QAction *engAction = languageMenu->addAction("English");
  QAction *sweAction = languageMenu->addAction("Svenska");

  // Connect actions to the setDate slot
  connect(engAction, &QAction::triggered, this, [this]() { setLanguage("English"); });
  connect(sweAction, &QAction::triggered, this, [this]() { setLanguage("Svenska"); });

}

void RawWindow::setLanguage(const QString& lan) {
  
  // Add code to change language
  if (lan == "English") {
  
  }
  else if (lan == "Svenska") {

  }
}

// Use this function to set the status of the cards, input the cardXStatus QLabel to change
void RawWindow::setButtonLabel(QLabel *label, double value, double safeValue, double cautionValue) {
    label->setText(QString::number(value));
    if (value <= safeValue) {
        label->setStyleSheet("color: green;");
    } 
    else if (value < cautionValue) {
        label->setStyleSheet("color: orange;");
    } 
    else {
        label->setStyleSheet("color: red;");
    } 
}

QPushButton* RawWindow::createPollutantCard(const QString &pollutantName, QLabel *label, double value, double safeValue,
 double cautionValue, const QString &unit) {

    // Make button
    QPushButton* cardbutton = new QPushButton();
    cardbutton->setCheckable(false);

    cardbutton->setStyleSheet(
        "QPushButton {"
    "   background-color: #949ba6;"
    "   border: 1px solid #949ba6;"
    "   border-radius: 0px;" // No rounded corners
    "   padding: 10px;"
    "   text-align: left;"
    "}"
    "QPushButton:hover {"
    "   background-color: #62666e;"
    "}"
    "QPushButton:pressed {"
    "   background-color: #4d5057;"
    "}"
    );

    setButtonLabel(label, value, safeValue, cautionValue);
    
    QVBoxLayout* cardLayout = new QVBoxLayout(cardbutton);
    cardbutton->setMinimumHeight(80);
    cardbutton->setMinimumWidth(200);

    QLabel *pollutantLabel = new QLabel(pollutantName);
    pollutantLabel->setStyleSheet("font-weight: bold;"); // Set style of text

    if (pollutantName == "TestName1") {
        connect(cardbutton, &QPushButton::clicked, this, [this]() { setPage("Page1"); });
    } else if (pollutantName == "TestName2") {
        connect(cardbutton, &QPushButton::clicked, this, [this]() { setPage("Page2"); });
    } else if (pollutantName == "TestName3") {
        connect(cardbutton, &QPushButton::clicked, this, [this]() { setPage("Page3"); });
    } else if (pollutantName == "TestName4") {
        connect(cardbutton, &QPushButton::clicked, this, [this]() { setPage("Page4"); });
    }
    
    cardLayout->addWidget(pollutantLabel);
    cardLayout->addWidget(label);
    cardLayout->addStretch();
    cardLayout->addWidget(cardbutton);

    cardbutton->setLayout(cardLayout);

    return cardbutton;
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
  if (page == "Page1") {setCentralWidget(table1);}
  else if (page == "Page2") {setCentralWidget(table2);}
  else if (page == "Page3") {setCentralWidget(table3);}
  else {setCentralWidget(table4);}
}

// @id, sample.samplingPoint , sample.samplingPoint.notation , sample.samplingPoint.label , 
// sample.sampleDateTime , determinand.label , determinand.definition , determinand.notation
// , resultQualifier.notation , result,codedResultInterpretation.interpretation,determinand.unit.label,sample.sampledMaterialType.label,sample.isComplianceSample,sample.purpose.label,sample.samplingPoint.easting,sample.samplingPoint.northing

/*
Complete translator
Documentation
Add actual data to cards( Average of latest messurement of all places together?), and reset when new file is loaded
Page1?
Help button
Card links not working

*/