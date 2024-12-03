#ifndef COMPLIANCEDASHBOARD_H
#define COMPLIANCEDASHBOARD_H

#include <QWidget>
#include <QTableView>
#include <QComboBox>
#include <QChartView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPieSeries>
#include <QLabel>
#include <QFile>
#include <QStandardItemModel>
#include <QTextStream>

QT_CHARTS_USE_NAMESPACE

class ComplianceDashboard : public QWidget {
    Q_OBJECT

public:
    ComplianceDashboard(QWidget *parent = nullptr);

private slots:
    void updateFilter();

private:
    QTableView *dataTable;
    QComboBox *locationFilter;
    QComboBox *pollutantFilter;
    QComboBox *complianceFilter;
    QChartView *complianceChart;

    QStandardItemModel *model; // Data model for the table
    QList<QList<QString>> data; // To store the loaded CSV data

    void setupUI();
    void loadData();
    void populateFilters();
    void updateChart();
    void filterData();
};

#endif // COMPLIANCEDASHBOARD_H
