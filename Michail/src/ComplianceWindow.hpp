#pragma once

#include <QtWidgets>
#include <QSortFilterProxyModel>
#include "WQTableModel.hpp"

class ComplianceWindow : public QMainWindow {
    Q_OBJECT

public:
    ComplianceWindow();
    ~ComplianceWindow();

private:
    void createMainWidget();
    void createButtons();
    void createToolbar();
    void createStatusBar();
    void addFileMenu();
    void createSummaryCards(const QString &selectedLocation, const QString &selectedPollutant, const QString &selectedCompliance);
    void openAndLoadCSV();
    void showPopUpDetails(const QModelIndex &index);
    void filterCompliance(const QString &complianceStatus);


    void filterLocation(const QString &location);
    void filterPollutant(const QString &pollutant);
    void populateFilters();

    QAction *openAction;
    QAction *closeAction;
    QMenu *fileMenu;
    QLabel *fileInfo;
    QLabel *dataInfo;
    QString filename;

    QPushButton *loadButton;
    QTableView *tableView;
    QSortFilterProxyModel *proxy;
    WQTableModel tableModel;
    QWidget *mainWidget;
    QWidget *summaryWidget; 


    QComboBox *locationFilterBox;
    QComboBox *pollutantFilterBox;
    QComboBox *complianceFilterBox;


    QGridLayout *summaryCardsLayout = nullptr;
    QVector<QMap<QString, QString>> originalData;
};
