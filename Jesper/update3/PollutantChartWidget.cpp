#include "PollutantChartWidget.hpp"
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QToolTip>
#include <QCoreApplication>

//using namespace QtCharts;

PollutantChartWidget::PollutantChartWidget(QWidget *parent) : QWidget(parent), chart(new QChart()) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);
}

// Only run after proxy is set
void PollutantChartWidget::createChart() {
    updateChart(""); 
}

void PollutantChartWidget::setModel(filterProxy *proxyModel) {
    this->proxy = proxyModel;
    updateChart("");
}

void PollutantChartWidget::updateChart(const QString &searchTerm) {
    if (!proxy) return;

    // Set all letters to lowercase
    QString normalizedSearchTerm = searchTerm.toLower();

    QMap<int, double> monthlySums;
    QMap<int, int> monthlyCounts;

    // Use min and max values to get a range for y-axis
    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::min();

    // Get average values on the searched pollutant for all months
    for (int row = 0; row < proxy->rowCount(); ++row) {
        QModelIndex defIndex = proxy->index(row, 5); 
        QString pollutantName = proxy->data(defIndex, Qt::DisplayRole).toString().toLower();

        if (pollutantName == normalizedSearchTerm) {
            QModelIndex resultIndex = proxy->index(row, 9);
            double result = proxy->data(resultIndex, Qt::DisplayRole).toDouble();

            QModelIndex dateIndex = proxy->index(row, 4);
            QString dateString = proxy->data(dateIndex, Qt::DisplayRole).toString();
            QDateTime dateTime = QDateTime::fromString(dateString, Qt::ISODate);

            if (dateTime.isValid()) {
                int month = dateTime.date().month();
                monthlySums[month] += result;
                monthlyCounts[month]++;
            }
        }
    }

    QStringList months = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };

    QBarSet *set = new QBarSet(normalizedSearchTerm);

    for (int month = 1; month <= 12; ++month) {
        if (monthlyCounts.contains(month)) {
            double avgValue = monthlySums[month] / monthlyCounts[month];
            set->append(avgValue);

            // Set min/max values
            minValue = std::min(minValue, avgValue);
            maxValue = std::max(maxValue, avgValue);
        } else {
            set->append(0.0); // Set 0 for missing months
        }
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Clear any existing chart elements
    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

    // Add series to chart
    chart->addSeries(series);

    // Configure X-axis
    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
    xAxis->append(months); // Add months
    chart->addAxis(xAxis, Qt::AlignBottom); // Add X-axis to chart
    series->attachAxis(xAxis); // Attach the series to the X-axis

    // Configure the Y-axis
    QValueAxis *yAxis = new QValueAxis();

    // Add padding to the min and max values for better visuals
    double padding = (maxValue - minValue) * 0.1; // 10% of the range
    if (padding == 0) {
        padding = 1; // Ensure non-zero range for uniform values
    }
    yAxis->setRange(minValue - padding, maxValue + padding);

    yAxis->setTitleText("Average Value");
    chart->addAxis(yAxis, Qt::AlignLeft); // Add Y-axis to chart
    series->attachAxis(yAxis); // Attach series to Y-axis

    // Add bar notes
    setupConnections(searchTerm);
}

void PollutantChartWidget::setupConnections(const QString &searchTerm) {
    auto series = new QBarSeries();

    if (!series || searchTerm.isEmpty()) return;

    connect(series, &QBarSeries::hovered, this, [searchTerm, this](bool status, int index, QBarSet *bar) {
        if (!proxy || !status) // Ensure the connection only triggers on hover
            return;

        QString normalizedSearchTerm = searchTerm.toLower();

        QStringList months = {
            "January", "February", "March", "April",
            "May", "June", "July", "August",
            "September", "October", "November", "December"
        };
        QString month = months.at(index);

        
        QString unit = ""; // Default unit

        for (int row = 0; row < proxy->rowCount(); ++row) {
            QModelIndex definitionIndex = proxy->index(row, 5);
            QString pollutantName = proxy->data(definitionIndex, Qt::DisplayRole).toString().toLower();

            if (pollutantName == normalizedSearchTerm) {
                QModelIndex unitIndex = proxy->index(row, 11);
                unit = proxy->data(unitIndex, Qt::DisplayRole).toString();
                break;
            }
        }

        double result = bar->at(0);

        QToolTip::showText(QCursor::pos(),
            QString("Pollutant: %1\nMonth: %2\nConcentration: %3 %4")
            .arg(normalizedSearchTerm)
            .arg(month)
            .arg(QString::number(result))
            .arg(unit)
        );
    });
}


