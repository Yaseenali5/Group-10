#include "PollutantChartWidget.hpp"
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QToolTip>
#include <QCoreApplication>

using namespace QtCharts;

PollutantChartWidget::PollutantChartWidget(QWidget *parent) : QWidget(parent), chart(new QChart()) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

    loadModelData("data/Y-2024.csv");
    createChart();
}

void PollutantChartWidget::loadModelData(const QString &filename) {
    QString basePath = QCoreApplication::applicationDirPath();
    QString fullPath = basePath + "/" + filename;
    dataset.loadData(fullPath.toStdString());
    createChart();
}

void PollutantChartWidget::createChart() {
    updateChartBasedOnSearch(""); // Initialize with all data
}

void PollutantChartWidget::updateChartBasedOnSearch(const QString &searchTerm) {
    QBarSeries *newSeries = new QBarSeries();

    // Hypothetical thresholds for simplification
    double lowThreshold = 0.0001;  // Threshold for green
    double highThreshold = 0.0010; // Threshold for red, amber in between

    for (auto it = dataset.begin(); it != dataset.end(); ++it) {
        if (QString::fromStdString(it->getDeterminandDefinition()).contains(searchTerm, Qt::CaseInsensitive)) {
            QBarSet *set = new QBarSet(QString::fromStdString(it->getDeterminandDefinition()));
            double result = it->getResult();

            // Apply color based on the threshold
            if (result < lowThreshold) {
                set->setColor(Qt::green); // Green for low concentrations
            } else if (result >= lowThreshold && result <= highThreshold) {
                set->setColor(QColor(255, 191, 0)); // Amber for caution
            } else {
                set->setColor(Qt::red); // Red for high concentrations
            }

            *set << result;
            newSeries->append(set);
        }
    }

    // Remove the old series and add the new filtered series
    chart->removeAllSeries();
    chart->addSeries(newSeries);
    chart->createDefaultAxes();
    setupConnections();
}

void PollutantChartWidget::setupConnections() {
    auto series = dynamic_cast<QBarSeries *>(chart->series().at(0));
    connect(series, &QBarSeries::hovered, this, [&](bool status, int index, QBarSet *bar) {
        QString label = QString::fromStdString(dataset.at(index).getDeterminandDefinition());
        double result = bar->at(0);
        QToolTip::showText(QCursor::pos(), QString("Pollutant: %1\nConcentration: %2 ug/l").arg(label, QString::number(result)));
    });
}
