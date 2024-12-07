#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include "PollutantChartWidget.hpp"

class PollutantOverviewWindow : public QWidget {
    Q_OBJECT

public:
    explicit PollutantOverviewWindow(QWidget *parent = nullptr);

private:
    QLineEdit *searchBar;
    PollutantChartWidget *chartWidget;
};
