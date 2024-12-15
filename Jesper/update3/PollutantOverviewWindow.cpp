#include "PollutantOverviewWindow.hpp"
#include <QVBoxLayout>
#include <iostream>

PollutantOverviewWindow::PollutantOverviewWindow(QWidget *parent) : QWidget(parent) {
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search for pollutants");

    
    chartWidget = new PollutantChartWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(searchBar);
    layout->addWidget(chartWidget);

    connect(searchBar, &QLineEdit::textChanged, chartWidget, &PollutantChartWidget::updateChart);
}

void PollutantOverviewWindow::setModel(filterProxy *proxyModel) {
    chartWidget->setModel(proxyModel);
}

void PollutantOverviewWindow::updateWindow() {
    chartWidget->updateChart("");
}
