#pragma once
#include <QWidget>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include "WQDataset.hpp"
#include "WQSample.hpp"

QT_CHARTS_USE_NAMESPACE

class PollutantChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit PollutantChartWidget(QWidget *parent = nullptr);
    void updateChartBasedOnSearch(const QString &searchTerm);

private:
    QChart *chart;
    QChartView *chartView;
    WQDataset dataset;

    void loadModelData(const QString &filename);
    void createChart();
    void setupConnections();
};
