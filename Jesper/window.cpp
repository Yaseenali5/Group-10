// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "window.hpp"
#include "stats.hpp"

static const int MIN_WIDTH = 620;


QuakeWindow::QuakeWindow(): QMainWindow(), statsDialog(nullptr)
{
  loadData();
  createStatusBar();
  addFileMenu();
  addPageMenu();
  addHelpMenu();
  createPages();
  

  setMinimumWidth(MIN_WIDTH);
  setWindowTitle("Quake Tool");
}

void loadData() {

}

// Jesper Johansson
void QuakeWindow::createPages() {

  // Instead of Qwidget, make our own page
  QLabel* page1 = new QLabel("Test1");
  QLabel* page2 = new QLabel("Test2");
  QLabel* page3 = new QLabel("Test3");
  QLabel* page4 = new QLabel("Test4");

  pages.append(page1);
  pages.append(page2);
  pages.append(page3);
  pages.append(page4);

  setPage1();

}

// Jesper Johansson
void QuakeWindow::setPage1() {
  setCentralWidget(pages[0]);
}

// Jesper Johansson
void QuakeWindow::setPage2() {
  setCentralWidget(pages[1]);
}

// Jesper Johansson
void QuakeWindow::setPage3() {
  setCentralWidget(pages[2]);
}

// Jesper Johansson
void QuakeWindow::setPage4() {
  setCentralWidget(pages[3]);
}

void QuakeWindow::createStatusBar()
{
  fileInfo = new QLabel("Current file: <none>");
  QStatusBar* status = statusBar();
  status->addWidget(fileInfo);
}


void QuakeWindow::addFileMenu()
{
  QAction* locAction = new QAction("Set Data &Location", this);
  locAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

  QAction* closeAction = new QAction("Quit", this);
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  QMenu* fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(locAction);
  fileMenu->addAction(closeAction);
}

// Jesper Johansson
void QuakeWindow::addPageMenu()
{

  QAction* locAction1 = new QAction("Page1", this);
  locAction1->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction1, SIGNAL(triggered()), this, SLOT(setPage1()));

  QAction* locAction2 = new QAction("Page2", this);
  locAction2->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction2, SIGNAL(triggered()), this, SLOT(setPage2()));

  QAction* locAction3 = new QAction("Page3", this);
  locAction3->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction3, SIGNAL(triggered()), this, SLOT(setPage3()));

  QAction* locAction4 = new QAction("Page4", this);
  locAction4->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction4, SIGNAL(triggered()), this, SLOT(setPage4()));

  QMenu* fileMenu = menuBar()->addMenu("&Page");
  fileMenu->addAction(locAction1);
  fileMenu->addAction(locAction2);
  fileMenu->addAction(locAction3);
  fileMenu->addAction(locAction4);
}


void QuakeWindow::addHelpMenu()
{
  QAction* aboutAction = new QAction("&About", this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  QAction* aboutQtAction = new QAction("About &Qt", this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QMenu* helpMenu = menuBar()->addMenu("&Help");
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}


void QuakeWindow::setDataLocation()
{
  QString directory = QFileDialog::getExistingDirectory(
    this, "Data Location", ".",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (directory.length() > 0) {
    dataLocation = directory;
  }
}


void QuakeWindow::openCSV()
{
  if (dataLocation == "") {
    QMessageBox::critical(this, "Data Location Error",
      "Data location has not been set!\n\n"
      "You can specify this via the File menu."
    );
    return;
  }

  auto filename = QString("%1_%2.csv")
    .arg(significance->currentText()).arg(period->currentText());

  auto path = dataLocation + "/" + filename;

  try {
    model.updateFromFile(path);
  }
  catch (const std::exception& error) {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(filename));
  table->resizeColumnsToContents();

  if (statsDialog != nullptr && statsDialog->isVisible()) {
    statsDialog->update(model.meanDepth(), model.meanMagnitude());
  }
}


void QuakeWindow::displayStats()
{
  if (model.hasData()) {
    if (statsDialog == nullptr) {
      statsDialog = new StatsDialog(this);
    }

    statsDialog->update(model.meanDepth(), model.meanMagnitude());

    statsDialog->show();
    statsDialog->raise();
    statsDialog->activateWindow();
  }
}


void QuakeWindow::about()
{
  QMessageBox::about(this, "About Quake Tool",
    "Quake Tool displays and analyzes earthquake data loaded from"
    "a CSV file produced by the USGS Earthquake Hazards Program.\n\n"
    "(c) 2024 Nick Efford");
}
