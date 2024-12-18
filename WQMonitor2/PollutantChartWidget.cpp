#include "PollutantChartWidget.hpp"
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QToolTip>
#include <QCoreApplication>
#include <QDebug>

// Constructor
PollutantChartWidget::PollutantChartWidget(QWidget *parent)
    : QWidget(parent), chart(new QChart()), xAxis(nullptr), yAxis(nullptr) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);
    chartType = "bar";
}

// Create chart based on the default bar type
void PollutantChartWidget::createChart() {
    updateChart(""); 
}

void PollutantChartWidget::setModel(filterProxy *proxyModel) {
    this->proxy = proxyModel;
    updateChart("");
}

void PollutantChartWidget::changeChartType() {
  chartType = "line";
}

// Main updateChart function with chart type parameter
void PollutantChartWidget::updateChart(const QString &searchTerm) {
    if (!proxy) return;

    QString normalizedSearchTerm = searchTerm.toLower();
    QMap<int, double> monthlySums;
    QMap<int, int> monthlyCounts;

    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::min();

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

    QList<QPointF> points;

    for (int month = 1; month <= 12; ++month) {
        if (monthlyCounts.contains(month)) {
            double avgValue = monthlySums[month] / monthlyCounts[month];
            points.append(QPointF(month - 1, avgValue));
            minValue = std::min(minValue, avgValue);
            maxValue = std::max(maxValue, avgValue);
        } else {
            points.append(QPointF(month - 1, 0.0));
        }
    }

    // Clear previous chart content
    chart->removeAllSeries();

    // Remove existing axes safely
    QList<QAbstractAxis *> existingAxes = chart->axes(Qt::Horizontal);
    for (QAbstractAxis *axis : existingAxes) {
        chart->removeAxis(axis);
    }
    existingAxes = chart->axes(Qt::Vertical);
    for (QAbstractAxis *axis : existingAxes) {
        chart->removeAxis(axis);
    }

    if (chartType == "bar") {
        // Create bar chart
        QBarSet *set = new QBarSet(normalizedSearchTerm);
        for (const QPointF &point : points) {
            set->append(point.y());
        }
        QBarSeries *barSeries = new QBarSeries();
        barSeries->append(set);
        chart->addSeries(barSeries);

        QBarCategoryAxis *xAxis = new QBarCategoryAxis();
        xAxis->append(months);
        chart->addAxis(xAxis, Qt::AlignBottom);
        barSeries->attachAxis(xAxis);

    } else if (chartType == "line") {
        // Create line series
        QLineSeries *lineSeries = new QLineSeries();
        for (const QPointF &point : points) {
            lineSeries->append(point);
        }
        chart->addSeries(lineSeries);

    } else if (chartType == "scatter") {
        // Create scatter series
        QScatterSeries *scatterSeries = new QScatterSeries();
        scatterSeries->setMarkerSize(8.0); // Adjust marker size as needed
        for (const QPointF &point : points) {
            scatterSeries->append(point);
        }
        chart->addSeries(scatterSeries);
    }

    // Configure Y-axis
    QValueAxis *yAxis = new QValueAxis();
    double padding = (maxValue - minValue) * 0.1;
    if (padding == 0) padding = 1;
    yAxis->setRange(minValue - padding, maxValue + padding);
    yAxis->setTitleText("Average Value");
    chart->addAxis(yAxis, Qt::AlignLeft);

    // Configure X-axis
    QValueAxis *xAxis = new QValueAxis();
    xAxis->setRange(0, months.size() - 1);
    xAxis->setTitleText("Month");
    chart->addAxis(xAxis, Qt::AlignBottom);

    if (chartType != "bar") {
        // Attach series to axes for non-bar charts
        for (QAbstractSeries *series : chart->series()) {
            series->attachAxis(xAxis);
            series->attachAxis(yAxis);
        }
    }
}


