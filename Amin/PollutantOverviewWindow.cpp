#include "PollutantOverviewWindow.hpp"
#include <QVBoxLayout>

PollutantOverviewWindow::PollutantOverviewWindow(QWidget *parent) : QWidget(parent) {
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search for pollutants");

    chartWidget = new PollutantChartWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(searchBar);
    layout->addWidget(chartWidget);

    connect(searchBar, &QLineEdit::textChanged, chartWidget, &PollutantChartWidget::updateChartBasedOnSearch);
}
