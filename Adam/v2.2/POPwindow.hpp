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

class POPWindow : public QMainWindow
{
    Q_OBJECT

public:
    POPWindow();

private:
    void createMainWidget();
    void createFileSelectors();
    void createButtons();
    void createToolbar();
    void createStatusBar();
    void addFileMenu();
    QColor indicatorColor(double result);
    
    QAction *openAction;
    QAction *closeAction;
    QMenu *fileMenu;
    QLabel *fileInfo;
    QLabel *dataInfo;
    QString filename;
    QComboBox *POPbox;
    QComboBox *resultBox;
    QComboBox *locBox;
    QPushButton *loadGButton;
    QPushButton *healthRisks;
    QPushButton *monImp;
    QPushButton *safetyLvl;
    QPushButton *loadButton;
    QSortFilterProxyModel* proxy;
    const QString* loc_;
    const QString* uPOP;

    WQDataset *dataset;  
    WQTableModel tablemodel;  
  
  private slots:
    void setDataLoc();
    void loadCSV();
    void filterPOP(const QString& POPname);
    void filterUPOP(const QString& uPOPname);
    void filterLoc(const QString& location);
    void loadGraph();
    void POPHealthRisks(const QString& pop);
    void POPSafetyInfo(const QString& pop);

};
