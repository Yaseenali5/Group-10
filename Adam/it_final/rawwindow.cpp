#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "rawwindow.hpp"


RawWindow::RawWindow(): QMainWindow()
{
  setWindowTitle("");
  setMinimumSize(1000, 500);
  createMainWidget();
  createStatusBar();
  addFileMenu();
  createButtons();
  createToolbar();
}

void RawWindow::createStatusBar()
{
  fileInfo = new QLabel("Current file: <none>");
  dataInfo = new QLabel("Data not loaded");
  QStatusBar* status = statusBar();
  status->addWidget(fileInfo);
  status->addWidget(dataInfo);
}

void RawWindow::addFileMenu()
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
  loadButton = new QPushButton("Load Data");
  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadCSV()));

  locBox = new QComboBox();
  connect(locBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(filterLoc(const QString&)));  

}

void RawWindow::createToolbar()
{
  QToolBar* toolbar = new QToolBar();

  QLabel* loadLabel = new QLabel("Load Data");
  QLabel* filterLocLabel = new QLabel("Filter by Location:");

  toolbar->addWidget(loadLabel);
  toolbar->addWidget(loadButton);
  
  toolbar->addSeparator();
  toolbar->addWidget(filterLocLabel);
  toolbar->addWidget(locBox);
  

  addToolBar(Qt::LeftToolBarArea, toolbar);
}

void RawWindow::createMainWidget()
{
  proxy = new QSortFilterProxyModel();
  proxy->setSourceModel(&tablemodel);

  table = new QTableView();
  table->setModel(proxy);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);

  setCentralWidget(table); 
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
  
  dataInfo->setText("Data Loaded!");

  locBox->clear();
  locBox->addItem(QString(""));

  int locCol = 3;

  QSet<QString> loc;
  for (int row = 0; row<proxy->rowCount(); ++row){
    QModelIndex index = proxy->index(row, locCol);
    QString lab = proxy->data(index).toString();
    loc.insert(lab);
  }

  locBox->addItems(loc.values());
}

void RawWindow::filterLoc(const QString& location)
{
  int locCol = 3;

  proxy->setFilterKeyColumn(locCol);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy->setFilterFixedString(location);

  dataInfo->setText(QString("Filtered by Location: %1").arg(location));
}