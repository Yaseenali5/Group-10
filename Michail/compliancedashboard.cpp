#include "ComplianceDashboard.h"
#include <QMessageBox>

ComplianceDashboard::ComplianceDashboard(QWidget *parent) : QWidget(parent), model(new QStandardItemModel(this)) {
    setupUI();
    loadData();
    populateFilters();
    updateChart();
}

void ComplianceDashboard::setupUI() {
    //layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    //setting up filters for locations, pollutants and compliance
    QHBoxLayout *filterLayout = new QHBoxLayout();
    locationFilter = new QComboBox();
    pollutantFilter = new QComboBox();
    complianceFilter = new QComboBox();

    //updates the filter as the user selects an option
    connect(locationFilter, &QComboBox::currentIndexChanged, this, &ComplianceDashboard::updateFilter);
    connect(pollutantFilter, &QComboBox::currentIndexChanged, this, &ComplianceDashboard::updateFilter);
    connect(complianceFilter, &QComboBox::currentIndexChanged, this, &ComplianceDashboard::updateFilter);

    filterLayout->addWidget(new QLabel("Location:"));
    filterLayout->addWidget(locationFilter);
    filterLayout->addWidget(new QLabel("Pollutant:"));
    filterLayout->addWidget(pollutantFilter);
    filterLayout->addWidget(new QLabel("Compliance:"));
    filterLayout->addWidget(complianceFilter);

    mainLayout->addLayout(filterLayout);

    //views the data table 
    dataTable = new QTableView(this);
    dataTable->setModel(model);
    mainLayout->addWidget(dataTable);

    //compliance pie chart 
    complianceChart = new QChartView(new QChart());
    complianceChart->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(complianceChart);

    setLayout(mainLayout);
    setWindowTitle("Compliance Dashboard");
    resize(800, 600);
}

void ComplianceDashboard::loadData() {
    QFile file("");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open CSV file.");
        return;
    }

    QTextStream in(&file);
    bool firstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (firstLine) { 
            firstLine = false;
            continue;
        }

        QStringList fields = line.split(",");
        if (fields.size() == 4) {
            data.append(fields);
        }
    }

    file.close();

    //this adds all data to the table 
    model->setHorizontalHeaderLabels({"Location", "Pollutant", "Level", "Compliance"});
    for (const QList<QString> &row : data) {
        QList<QStandardItem *> items;
        for (const QString &cell : row) {
            items.append(new QStandardItem(cell));
        }
        model->appendRow(items);
    }
}

    //populating the filters
void ComplianceDashboard::populateFilters() {
    locationFilter->addItem("All Locations");
    pollutantFilter->addItem("All Pollutants");
    complianceFilter->addItem("All Compliance Levels");

    QSet<QString> locations, pollutants, compliance;
    for (const QList<QString> &row : data) {
        locations.insert(row[0]);
        pollutants.insert(row[1]);
        compliance.insert(row[3]);
    }

    for (const QString &location : locations) {
        locationFilter->addItem(location);
    }
    for (const QString &pollutant : pollutants) {
        pollutantFilter->addItem(pollutant);
    }
    for (const QString &complianceLevel : compliance) {
        complianceFilter->addItem(complianceLevel);
    }
}
    // this updates the filters when user selects an option
void ComplianceDashboard::updateFilter() {
    QString selectedLocation = locationFilter->currentText();
    QString selectedPollutant = pollutantFilter->currentText();
    QString selectedCompliance = complianceFilter->currentText();

    model->removeRows(0, model->rowCount()); 

    for (const QList<QString> &row : data) {
        bool matches = true;

        if (selectedLocation != "All Locations" && row[0] != selectedLocation) {
            matches = false;
        }
        if (selectedPollutant != "All Pollutants" && row[1] != selectedPollutant) {
            matches = false;
        }
        if (selectedCompliance != "All Compliance Levels" && row[3] != selectedCompliance) {
            matches = false;
        }

        if (matches) {
            QList<QStandardItem *> items;
            for (const QString &cell : row) {
                items.append(new QStandardItem(cell));
            }
            model->appendRow(items);
        }
    }

    updateChart();
}
    //updates the chart depending on what is selected
void ComplianceDashboard::updateChart() {
    int compliantCount = 0;
    int nonCompliantCount = 0;

    for (int row = 0; row < model->rowCount(); ++row) {
        QString compliance = model->item(row, 3)->text();
        if (compliance == "Compliant") {
            compliantCount++;
        } else if (compliance == "Non-Compliant") {
            nonCompliantCount++;
        }
    }

    QPieSeries *series = new QPieSeries();
    series->append("Compliant", compliantCount);
    series->append("Non-Compliant", nonCompliantCount);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Compliance Overview");
    complianceChart->setChart(chart);
}
