#include <QtWidgets>
#include <QSortFilterProxyModel>
#include "WQDataset.hpp"
#include "WQTableModel.hpp"
#include "filterProxy.hpp"
#include "PollutantOverviewWindow.hpp"
#include "homePage.hpp"
#include "POPwindow.hpp"
#include "ComplianceWindow.hpp"
#include "FCwindow.hpp"
#include "LWindow.hpp"

class RawWindow : public QMainWindow
{    
    Q_OBJECT

public:
    RawWindow();

private:
    void showMonthAndDaySelectors();
    void removeMonthAndDaySelectors();
    void createMainWidget();
    void createFileSelectors();
    void createButtons();
    void createToolbar();
    void createStatusBar();
    void addFileMenu();
    void filterDate();
    void addPageMenu();
    void addLanguageMenu();
    void clearCentralWidget();
    void setButtonLabel( QLabel *label, double value, double safeValue, double cautionValue);
    QPushButton* createPollutantCard(const QString &pollutantName, QLabel *label, double value, double safeValue,
    double cautionValue, const QString &unit);
    void createToolbarAlt();

    QAction *openAction;
    QAction *closeAction;
    QMenu *fileMenu;
    QLabel *fileInfo;
    QLabel *dataInfo;
    QString filename;
    QPushButton *loadButton;
    QTranslator translator;
    homePage *homePageWidget;
    QPushButton *homeButton;
    QLabel *filterLocLabel;

    QToolBar *toolbar;
    QToolBar *toolbarAlt;

    // Remove these when all pages are implemented
    QTableView *table1;
    QTableView *table2;
    QTableView *table3;
    QTableView *table4;

    PollutantOverviewWindow *page1;
    POPWindow *page2;
    ComplianceWindow *page5;
    FCWindow *page4;
    LWindow *page3;

    // Pollutant card status
    QLabel *card1Status, *card2Status, *card3Status, *card4Status;

    QComboBox *locBox;
    QComboBox *monthBox;
    QSpinBox *dayBox = new QSpinBox(this);


    int monthNum;
    int dayNum;
    //QSortFilterProxyModel* proxy;
    filterProxy* proxy;
    //POPwindow *PopPage
    //QVector<QLabel*> pages;   // vector of pages, Cange QLable
    
    WQTableModel tablemodel;  
  
  private slots:
    void setDataLoc();
    void loadCSV();
    void filterLoc(const QString& location);
    
    void setPage(const QString& page);
    void setMonth(const QString& month);
    void setDay(int day);
    void setLanguage(const QString& lan);
};
