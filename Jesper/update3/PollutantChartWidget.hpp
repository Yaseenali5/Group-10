#pragma once
#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include "WQDataset.hpp"
#include "WQSample.hpp"
#include "filterProxy.hpp"
#include <QMap>
#include <QPair>
#include <QDate>
#include <iostream>

QT_USE_NAMESPACE

class PollutantChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit PollutantChartWidget(QWidget *parent = nullptr);
    void updateChart(const QString &searchTerm);
    void setModel(filterProxy *proxyModel);

    filterProxy *proxy;

private:
    QChart *chart;
    QChartView *chartView;
    WQDataset dataset;

    static const QMap<QString, QPair<double, double>> pollutantThresholds;
    

    void loadModelData(const QString &filename);
    void createChart();
    void setupConnections(const QString &searchTerm);
    
};
