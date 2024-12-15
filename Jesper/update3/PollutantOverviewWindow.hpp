#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include "PollutantChartWidget.hpp"

class PollutantOverviewWindow : public QWidget {
    Q_OBJECT

public:
    explicit PollutantOverviewWindow(QWidget *parent = nullptr);
    void setModel(filterProxy *proxyModel);
    void updateWindow();

private:
    QLineEdit *searchBar;
    PollutantChartWidget *chartWidget;
};
