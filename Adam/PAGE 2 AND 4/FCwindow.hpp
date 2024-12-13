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

class FCWindow : public QWidget
{
    Q_OBJECT // initialising the POPWindow class a Q Object

    public : 
    
    FCWindow();
    void setDL(const QString&);

    private:

    void createWidgets();
    void arrangeWidgets();
    void populatePFAS();
    void removeRightmostWidget();
    QColor indicatorColor(double result);
    
    QString filename;
    QLabel *blank;
    QLabel *PFASLabel;
    QLabel *locLabel;
    QPushButton *loadButton;
    QPushButton *loadGButton;
    QPushButton *healthInfo;
    QPushButton *envInfo;
    QPushButton *safetyLvl;
    QComboBox *PFASBox;
    QComboBox *locBox;

    QHBoxLayout *mainLayout;
    QVBoxLayout* subLayout;

    WQTableModel tablemodel; // this is used to gather data, and for the filtering and graphing to be done
    QSortFilterProxyModel *proxy; // this is used to filter throught the csv file

    QChartView *chartView;

    private slots:

    void loadCSV();
    void filterPFAS(const QString &pfas);
    void filterLoc(const QString &location);
    void loadGraph();


};
