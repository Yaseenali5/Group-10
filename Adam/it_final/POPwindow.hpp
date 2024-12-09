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

class POPWindow : public QMainWindow
{
    Q_OBJECT // initialising the POPWindow class a Q Object

public : POPWindow();

private:
    void createMainWidget(); // this function creates the central widget
    void createButtons();                 // this creates all the buttons on the page
    void createToolbar();                 // this creates the side toobar and organises the labels and buttons
    void createStatusBar();               // this creates the status bar on the bottom
    void addFileMenu();                   // this adds the file menu onto the top tool bar
    QColor indicatorColor(double result); // this is for compliance coloring of the graph

    QAction *openAction;
    QAction *closeAction; // these are for the opening of files and the closing of the progam
    QMenu *fileMenu;      // this is for the file menu
    QLabel *fileInfo;     // this holds filename for the file that is currently set as the data location
    QLabel *dataInfo;     // this holds whether or not the data has been loaded
    QString filename;
    QComboBox *POPbox;        // this is the intial filter for POPs
    QComboBox *resultBox;     // this holds the specific type of POPs in the csv file that are present (ie DDT Derived, or PCB con 028)
    QComboBox *locBox;        // this is the final filter which can holds each of the sampling points which are tested for the specific POP
    QPushButton *loadGButton; // this button is to load the graph after the filters are applied
    QPushButton *healthRisks;
    QPushButton *monImp;
    QPushButton *safetyLvl;       // these are used as roll-over popups to inform the user of extra information
    QPushButton *loadButton;      // this is used to load the file from the set data location
    QSortFilterProxyModel *proxy; // this is used to filter throught the csv file

    const QString *loc_;
    const QString *uPOP; // this is used for the filtering, as they sotre the users preference

    WQDataset *dataset;
    WQTableModel tablemodel; // this is used to gather data, and for the filtering and graphing to be done

private slots:
    void setDataLoc();                        // this sets the data location
    void loadCSV();                           // this is for loading the csv file
    void filterPOP(const QString &POPname);   // this is for the initial filtering of POPs
    void filterUPOP(const QString &uPOPname); // this is for filtering the specific POPs listed in the csv (UPOP = Unique Persistent Orangic Pollutant)
    void filterLoc(const QString &location);  // this is for filtering the specific sampling point with the specific POP preference
    void loadGraph();                         // this is used to load the graph form the data supplied
    void POPHealthRisks(const QString &pop);
    void POPSafetyInfo(const QString &pop); // these are for showing specific info depending on user interest (ie if they filter for Aldrin , they can see additional information on Aldrin)
};