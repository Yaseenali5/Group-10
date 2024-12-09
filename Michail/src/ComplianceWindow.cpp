#include "ComplianceWindow.hpp"

//constructor
ComplianceWindow::ComplianceWindow() {
    setWindowTitle("Compliance Dashboard");
    setMinimumSize(1000, 600);
    createMainWidget();
    createStatusBar();
    addFileMenu();
    createButtons();
    createToolbar();
}

//destructor
ComplianceWindow::~ComplianceWindow() {

}

// status bar at the bottom of the screen that shows whether a file is currently loaded in
void ComplianceWindow::createStatusBar() {
    fileInfo = new QLabel("Current file: <none>", this);
    dataInfo = new QLabel("Data not loaded", this);

    statusBar()->addWidget(fileInfo);
    statusBar()->addWidget(dataInfo);
}

// file menu
void ComplianceWindow::addFileMenu() {
    fileMenu = menuBar()->addMenu("&File");

    openAction = new QAction("&Open CSV", this);
    connect(openAction, &QAction::triggered, this, &ComplianceWindow::openAndLoadCSV);

    closeAction = new QAction("&Quit", this);
    connect(closeAction, &QAction::triggered, this, &QWidget::close);

    fileMenu->addAction(openAction);
    fileMenu->addAction(closeAction);
}

// button for loading in the csv file
void ComplianceWindow::createButtons() {
    loadButton = new QPushButton("Load Data", this);
    connect(loadButton, &QPushButton::clicked, this, &ComplianceWindow::openAndLoadCSV);
}

// toolbar for the filters 
void ComplianceWindow::createToolbar() {
    QToolBar *toolbar = addToolBar("Main Toolbar");

    locationFilterBox = new QComboBox(this);
    pollutantFilterBox = new QComboBox(this);
    complianceFilterBox = new QComboBox(this);

    complianceFilterBox->addItems({"All", "Compliant", "Non-Compliant"});

    connect(locationFilterBox, &QComboBox::currentTextChanged, this, &ComplianceWindow::filterLocation);
    connect(pollutantFilterBox, &QComboBox::currentTextChanged, this, &ComplianceWindow::filterPollutant);
    connect(complianceFilterBox, &QComboBox::currentTextChanged, this, &ComplianceWindow::filterCompliance);

    toolbar->addWidget(new QLabel("Location: "));
    toolbar->addWidget(locationFilterBox);
    toolbar->addWidget(new QLabel("Pollutant: "));
    toolbar->addWidget(pollutantFilterBox);
    toolbar->addWidget(new QLabel("Compliance: "));
    toolbar->addWidget(complianceFilterBox);
    toolbar->addWidget(loadButton);
}

// main widget for the summary cards,
void ComplianceWindow::createMainWidget() {
    mainWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mainWidget);

    // qscrollarea allows user to scroll through the cards as the data inputed is large
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    summaryWidget = new QWidget(this);
    summaryWidget->setLayout(new QVBoxLayout());
    scrollArea->setWidget(summaryWidget);

    layout->addWidget(scrollArea);

    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);
}

// creating the summary cards, that display the pollutant and compliance and non compliance counts
void ComplianceWindow::createSummaryCards(const QString &selectedLocation, const QString &selectedPollutant, const QString &selectedCompliance) {


    // clears previous sumary cards to update filters
    if (summaryCardsLayout) {
        QLayoutItem *child;
        while ((child = summaryCardsLayout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater();
            }
            delete child;
        }
        delete summaryCardsLayout;
        summaryCardsLayout = nullptr;
    }

    summaryCardsLayout = new QGridLayout();

    if (originalData.isEmpty()) {
        QMessageBox::warning(this, "No Data", "Please load data before generating summary cards.");
        return;
    }

    QMap<QString, QMap<QString, int>> pollutantSummary;

    for (const auto &row : originalData) {
        QString location = row["location"];
        QString pollutant = row["pollutant"];
        QString compliance = row["compliance"].trimmed().toLower(); // Ensure trimming and lowercase

        if (pollutant.isEmpty() || compliance.isEmpty()) {
            continue;
        }


        // implementing the filters for the summary cards
        if (!selectedLocation.isEmpty() && selectedLocation != "All" && location.compare(selectedLocation, Qt::CaseInsensitive) != 0)
            continue;

        if (!selectedPollutant.isEmpty() && selectedPollutant != "All" && pollutant.compare(selectedPollutant, Qt::CaseInsensitive) != 0)
            continue;

        if (selectedCompliance == "Compliant" && compliance != "true")
            continue;

        if (selectedCompliance == "Non-Compliant" && compliance != "false")
            continue;

        if (compliance == "true") {
            pollutantSummary[pollutant]["Compliant"]++;
        } else if (compliance == "false") {
            pollutantSummary[pollutant]["Non-Compliant"]++;
        }
    }

    // this populates the summary cards with the data from the csv file
    int row = 0;
    for (auto it = pollutantSummary.begin(); it != pollutantSummary.end(); ++it) {
        QString pollutant = it.key();
        int compliantCount = it.value().value("Compliant", 0);
        int nonCompliantCount = it.value().value("Non-Compliant", 0);

        QLabel *pollutantLabel = new QLabel(QString("<b>%1</b>").arg(pollutant));
        QLabel *compliantLabel = new QLabel(QString("✅ Compliant: %1").arg(compliantCount));
        QLabel *nonCompliantLabel = new QLabel(QString("❌ Non-Compliant: %1").arg(nonCompliantCount));

        //this highlights and filters the compliance section to either non compliant or compliant
        if (selectedCompliance == "Compliant") {
            compliantLabel->setStyleSheet("color: green; font-weight: bold;");
            nonCompliantLabel->setStyleSheet("color: gray;");
        } else if (selectedCompliance == "Non-Compliant") {
            nonCompliantLabel->setStyleSheet("color: red; font-weight: bold;");
            compliantLabel->setStyleSheet("color: gray;");
        } else {
            compliantLabel->setStyleSheet("color: green;");
            nonCompliantLabel->setStyleSheet("color: red;");
        }

        summaryCardsLayout->addWidget(pollutantLabel, row, 0);
        summaryCardsLayout->addWidget(compliantLabel, row, 1);
        summaryCardsLayout->addWidget(nonCompliantLabel, row, 2);

        row++;
    }

    
    delete summaryWidget->layout();
    summaryWidget->setLayout(summaryCardsLayout);

    summaryWidget->update();
    summaryWidget->show();
}




// loading the csv file into the application
void ComplianceWindow::openAndLoadCSV() {
    filename = QFileDialog::getOpenFileName(this, "Open CSV File", ".", "CSV files (*.csv)");

    if (filename.isEmpty()) {
        QMessageBox::warning(this, "No File Selected", "Please select a valid CSV file.");
        return;
    }

    fileInfo->setText(QString("Current file: %1").arg(QFileInfo(filename).fileName()));

    try {
        tableModel.updateFromFile(filename);
        dataInfo->setText("Data Loaded Successfully!");

        // originalData is used so that when selecting "All" on the filters, all original data from the file shows up again
        originalData.clear();
        int totalRows = tableModel.rowCount(QModelIndex());
        for (int row = 0; row < totalRows; ++row) {
            QMap<QString, QString> rowData;
            rowData["location"] = tableModel.data(tableModel.index(row, 3), Qt::DisplayRole).toString().trimmed();
            rowData["pollutant"] = tableModel.data(tableModel.index(row, 5), Qt::DisplayRole).toString().trimmed();
            rowData["compliance"] = tableModel.data(tableModel.index(row, 13), Qt::DisplayRole).toString().trimmed();
            originalData.append(rowData);
        }

        //populating filters after loading the file
        populateFilters();

        // generating summary cards after loading in the file
        createSummaryCards("All", "All", "All");

    } catch (const std::exception &error) {
        QMessageBox::critical(this, "CSV Load Error", error.what());
        dataInfo->setText("Data Load Failed!");
    }
}


// populating the filters
void ComplianceWindow::populateFilters() {
    locationFilterBox->clear();
    pollutantFilterBox->clear();

    QSet<QString> locations;
    QSet<QString> pollutants;

    int totalRows = tableModel.rowCount(QModelIndex());

    for (int row = 0; row < totalRows; ++row) {
    QString location = tableModel.data(tableModel.index(row, 3), Qt::DisplayRole).toString().trimmed();
    QString pollutant = tableModel.data(tableModel.index(row, 5), Qt::DisplayRole).toString().trimmed();

        if (!location.isEmpty()) locations.insert(location);
        if (!pollutant.isEmpty()) pollutants.insert(pollutant);
    }

    locationFilterBox->addItem("All");
    locationFilterBox->addItems(locations.values());

    pollutantFilterBox->addItem("All");
    pollutantFilterBox->addItems(pollutants.values());
}

// filter functions for compliance, location and pollutant
void ComplianceWindow::filterCompliance(const QString &complianceStatus) {
    QString location = locationFilterBox->currentText();
    QString pollutant = pollutantFilterBox->currentText();
    createSummaryCards(location, pollutant, complianceStatus);
}

void ComplianceWindow::filterLocation(const QString &location) {
    QString pollutant = pollutantFilterBox->currentText();
    QString compliance = complianceFilterBox->currentText();
    createSummaryCards(location, pollutant, compliance);
}

void ComplianceWindow::filterPollutant(const QString &pollutant) {
    QString location = locationFilterBox->currentText();
    QString compliance = complianceFilterBox->currentText();
    createSummaryCards(location, pollutant, compliance);
}
