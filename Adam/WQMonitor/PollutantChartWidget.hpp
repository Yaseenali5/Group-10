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

    // Set the proxy model for filtering
    void setModel(filterProxy *proxyModel);

    // Create and update the chart with a specified chart type
    void createChart(); 
    void updateChart(const QString &searchTerm);
    void changeChartType();

private:
    QChart *chart;                      // Main chart object
    QChartView *chartView;              // Chart view for rendering
    QValueAxis *xAxis;                  // X-axis (used for non-bar charts)
    QValueAxis *yAxis;                  // Y-axis for all chart types
    filterProxy *proxy;                 // Proxy model for filtered data
    QString chartType;

    // Helper to set up hover connections for bar charts
    void setupConnections(QBarSeries *series, const QString &searchTerm);
};
