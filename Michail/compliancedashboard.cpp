#include "compliancedashboard.h"
#include <QtCharts/QPieSlice>
#include <QSet>

ComplianceDashboard::ComplianceDashboard(QWidget *parent)
    : QWidget(parent),
      tableWidget(new QTableWidget(this)),
      locationFilter(new QComboBox(this)),
      pollutantFilter(new QComboBox(this)),
      complianceFilter(new QComboBox(this)),
      filterButton(new QPushButton("Apply Filters", this)),
      pieChartButton(new QPushButton("Show Pie Chart", this)),
      chartView(new QChartView(this)) {

    // Set up main layout
    mainLayout = new QVBoxLayout(this);

    // Filters Layout
    QHBoxLayout *filtersLayout = new QHBoxLayout();
    filtersLayout->addWidget(new QLabel("Location:"));
    filtersLayout->addWidget(locationFilter);
    filtersLayout->addWidget(new QLabel("Pollutant:"));
    filtersLayout->addWidget(pollutantFilter);
    filtersLayout->addWidget(new QLabel("Compliance:"));
    filtersLayout->addWidget(complianceFilter);
    filtersLayout->addWidget(filterButton);
    filtersLayout->addWidget(pieChartButton);

    // Add filters and table to main layout
    mainLayout->addLayout(filtersLayout);
    mainLayout->addWidget(tableWidget);
    mainLayout->addWidget(chartView);

    // Table setup
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({"Pollutant", "Location", "Concentration", "Compliance"});

    // Connect signals to slots
    connect(filterButton, &QPushButton::clicked, this, &ComplianceDashboard::applyFilters);
    connect(pieChartButton, &QPushButton::clicked, this, &ComplianceDashboard::showPieChart);
}

ComplianceDashboard::~ComplianceDashboard() {
    // Destructor
}

void ComplianceDashboard::loadData(const QVector<ComplianceData> &data) {
    complianceData = data;
    setupFilters();
    populateTable(data);
}

void ComplianceDashboard::setupFilters() {
    QSet<QString> locations, pollutants, compliances;
    for (const auto &entry : complianceData) {
        locations.insert(entry.location);
        pollutants.insert(entry.pollutant);
        compliances.insert(entry.compliance);
    }

    locationFilter->addItem("All");
    locationFilter->addItems(locations.values());

    pollutantFilter->addItem("All");
    pollutantFilter->addItems(pollutants.values());

    complianceFilter->addItem("All");
    complianceFilter->addItems(compliances.values());
}

void ComplianceDashboard::populateTable(const QVector<ComplianceData> &data) {
    tableWidget->clearContents();
    tableWidget->setRowCount(data.size());

    for (int i = 0; i < data.size(); ++i) {
        const auto &entry = data[i];

        tableWidget->setItem(i, 0, new QTableWidgetItem(entry.pollutant));
        tableWidget->setItem(i, 1, new QTableWidgetItem(entry.location));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(entry.concentration)));
        tableWidget->setItem(i, 3, new QTableWidgetItem(entry.compliance));

        // Add compliance-based coloring
        if (entry.compliance == "Compliant") {
            tableWidget->item(i, 3)->setBackground(Qt::green);
        } else if (entry.compliance == "Near Threshold") {
            tableWidget->item(i, 3)->setBackground(Qt::yellow);
        } else if (entry.compliance == "Non-Compliant") {
            tableWidget->item(i, 3)->setBackground(Qt::red);
        }
    }
}

void ComplianceDashboard::applyFilters() {
    QString selectedLocation = locationFilter->currentText();
    QString selectedPollutant = pollutantFilter->currentText();
    QString selectedCompliance = complianceFilter->currentText();

    QVector<ComplianceData> filteredData;
    for (const auto &entry : complianceData) {
        if ((selectedLocation == "All" || entry.location == selectedLocation) &&
            (selectedPollutant == "All" || entry.pollutant == selectedPollutant) &&
            (selectedCompliance == "All" || entry.compliance == selectedCompliance)) {
            filteredData.append(entry);
        }
    }

    populateTable(filteredData);
}

void ComplianceDashboard::showPieChart() {
    QPieSeries *series = new QPieSeries();

    int compliant = 0, nearThreshold = 0, nonCompliant = 0;
    for (const auto &entry : complianceData) {
        if (entry.compliance == "Compliant") compliant++;
        else if (entry.compliance == "Near Threshold") nearThreshold++;
        else if (entry.compliance == "Non-Compliant") nonCompliant++;
    }

    series->append("Compliant", compliant);
    series->append("Near Threshold", nearThreshold);
    series->append("Non-Compliant", nonCompliant);

    QPieSlice *sliceCompliant = series->slices().at(0);
    sliceCompliant->setBrush(Qt::green);

    QPieSlice *sliceNearThreshold = series->slices().at(1);
    sliceNearThreshold->setBrush(Qt::yellow);

    QPieSlice *sliceNonCompliant = series->slices().at(2);
    sliceNonCompliant->setBrush(Qt::red);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Compliance Distribution");

    chartView->setChart(chart);
}
