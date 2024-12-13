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
    
    POPWindow();
    void setDL(const QString&);

    private:

    void createWidgets();
    void arrangeWidgets();
    QColor indicatorColor(double result); 
    void removeRightmostWidget();

    QLabel *blank;
    QLabel *resultLabel;
    QLabel *locLabel;
    QLabel *POPLabel;
    QPushButton *loadButton;
    QComboBox *POPBox;
    QComboBox *resultBox;
    QComboBox *locBox;
    QPushButton *loadGButton;
    QPushButton *healthRisks;
    QPushButton *monImp;
    QPushButton *safetyLvl;
    QString filename;
    WQTableModel tablemodel; // this is used to gather data, and for the filtering and graphing to be done
    QSortFilterProxyModel *proxy; // this is used to filter throught the csv file
    QHBoxLayout *mainLayout;
    QVBoxLayout *subLayout;
    QChartView *chartView;

    private slots:
    void POPHealthRisks(const QString &pop);
    void POPSafetyInfo(const QString &pop);
    void loadCSV();
    void filterPOP(const QString &POPname);
    void filterUPOP(const QString &uPOPname);
    void filterLoc(const QString &location);
    void loadGraph();


};
