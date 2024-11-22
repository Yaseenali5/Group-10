#ifndef COMPLIANCEDASHBOARD_H
#define COMPLIANCEDASHBOARD_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

struct ComplianceData {
    QString pollutant;
    QString location;
    double concentration;
    QString compliance;
};

class ComplianceDashboard : public QWidget {
    Q_OBJECT

public:
    explicit ComplianceDashboard(QWidget *parent = nullptr);
    ~ComplianceDashboard();

    void loadData(const QVector<ComplianceData> &data);

private slots:
    void applyFilters();
    void showPieChart();

private:
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;
    QComboBox *locationFilter;
    QComboBox *pollutantFilter;
    QComboBox *complianceFilter;
    QPushButton *filterButton;
    QPushButton *pieChartButton;
    QChartView *chartView;

    QVector<ComplianceData> complianceData;

    void populateTable(const QVector<ComplianceData> &data);
    void setupFilters();
};

#endif // COMPLIANCEDASHBOARD_H
