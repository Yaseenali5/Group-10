#ifndef POPWINDOW_HPP
#define POPWINDOW_HPP   
// including all necessary libraries and classes
#include <QtWidgets>
#include <QSortFilterProxyModel>
#include "WQDataset.hpp"
#include "WQTableModel.hpp"
#include <QtCore/QSet>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>


class POPWindow : public QWidget
{
    Q_OBJECT // initialising the POPWindow class a Q Object

    public : 

    explicit POPWindow(QWidget *parent = nullptr);
    void updateFile(WQTableModel& tablemodel);
    void populateResultsBox(const QString& loc);

    private:
    void createWidgets();
    void createToolbar();
    QColor indicatorColor(double result);
    void clearWidgets();
    void checkItemCt();


    QVector<QString> *POPs;

    QLabel *resultLabel;
    QPushButton *loadButton;
    QComboBox *resultBox;
    QPushButton *loadGButton;
    QPushButton *healthRisks;
    QPushButton *monImp;
    QPushButton *safetyLvl;
    QVBoxLayout* mainlayout;
    QSortFilterProxyModel *proxy; // this is used to filter throught the csv file
    QString filename;
    QString location;

    private slots:
    void loadGraph();
    void POPHealthRisks(const QString &pop);
};
#endif