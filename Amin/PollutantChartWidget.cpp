#include "PollutantChartWidget.hpp"
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QToolTip>
#include <QCoreApplication>
#include <algorithm>

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
    updateChartBasedOnSearch("");
}

void PollutantChartWidget::updateChartBasedOnSearch(const QString &searchTerm) {
    QBarSeries *newSeries = new QBarSeries();
    std::vector<double> results;

    for (auto it = dataset.begin(); it != dataset.end(); ++it) {
        if (QString::fromStdString(it->getDeterminandDefinition()).contains(searchTerm, Qt::CaseInsensitive)) {
            results.push_back(it->getResult());
        }
    }

    std::sort(results.begin(), results.end());
    double q1 = results[results.size() * 25 / 100];  // 25th percentile
    double q3 = results[results.size() * 75 / 100];  // 75th percentile

    for (auto it = dataset.begin(); it != dataset.end(); ++it) {
        if (QString::fromStdString(it->getDeterminandDefinition()).contains(searchTerm, Qt::CaseInsensitive)) {
            QBarSet *set = new QBarSet(QString::fromStdString(it->getDeterminandDefinition()));
            double result = it->getResult();
            if (result < q1) {
                set->setColor(Qt::green);
            } else if (result < q3) {
                set->setColor(QColor(255, 191, 0));  // Amber
            } else {
                set->setColor(Qt::red);
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
        QToolTip::showText(QCursor::pos(), QString("Substance: %1\nConcentration: %2 ug/l").arg(label, QString::number(result)));
    });
}
