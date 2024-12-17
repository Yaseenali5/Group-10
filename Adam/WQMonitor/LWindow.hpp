#ifndef LWINDOW_HPP
#define LWINDOW_HPP   
// including all necessary libraries and classes
#include <QtWidgets>
#include <QSortFilterProxyModel>
#include "WQDataset.hpp"
#include "WQTableModel.hpp"
#include <QtCore/QSet>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>


class LWindow : public QWidget
{
    Q_OBJECT // initialising the POPWindow class a Q Object

    public : 

    explicit LWindow(QWidget *parent = nullptr);
    void updateFile(WQTableModel& tablemodel);
    void populateWTBox(const QString& loc);

    private:
    void createWidgets();
    void createToolbar();
    void clearWidgets();
    void checkItemCt();


    QVector<QString> *POPs;

    QComboBox *WTBox;
    QComboBox *LTBox;

    QLabel *WTLab;
    QLabel *LTLab;

    QPushButton *loadGButton;

    QVBoxLayout* mainlayout;
    QSortFilterProxyModel *proxy; // this is used to filter throught the csv file
    QString filename;
    QString location;

    private slots:
    void loadGraph();

};
#endif