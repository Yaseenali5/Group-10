#include <QtWidgets>
#include <QSortFilterProxyModel>
#include "WQDataset.hpp"
#include "WQTableModel.hpp"
#include "filterProxy.hpp"

class RawWindow : public QMainWindow
{    
    Q_OBJECT

public:
    RawWindow();

private:
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

    QAction *openAction;
    QAction *closeAction;
    QMenu *fileMenu;
    QLabel *fileInfo;
    QLabel *dataInfo;
    QString filename;
    QPushButton *loadButton;
    QTranslator translator;

    QTableView *table1;
    QTableView *table2;
    QTableView *table3;
    QTableView *table4;

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
