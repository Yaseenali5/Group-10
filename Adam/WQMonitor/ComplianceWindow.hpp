#pragma once

#include <QtWidgets>
#include <QSortFilterProxyModel>
#include "WQTableModel.hpp"
#include "filterProxy.hpp"

class ComplianceWindow : public QWidget {
    Q_OBJECT

public:
    explicit ComplianceWindow(QWidget *parent = nullptr);
    void setProxy(filterProxy *proxyModel);
    void updateWindow();
    //~ComplianceWindow();

private:
    void createMainWidget();
    void createToolbar();
    void createSummaryCards(const QString &selectedPollutant, const QString &selectedCompliance);
    void showPopUpDetails(const QModelIndex &index);
    void filterCompliance(const QString &complianceStatus);

    void filterPollutant(const QString &pollutant);
    void populateFilters();

    QAction *openAction;
    QAction *closeAction;
    QMenu *fileMenu;
    QLabel *fileInfo;
    QLabel *dataInfo;
    QString filename;

    QVBoxLayout *mainlayout;

    QTableView *tableView;
    filterProxy *proxy;
    WQTableModel tableModel;
    QWidget *mainWidget;
    QWidget *summaryWidget; 

    QComboBox *pollutantFilterBox;
    QComboBox *complianceFilterBox;


    QGridLayout *summaryCardsLayout = nullptr;
    QVector<QMap<QString, QString>> originalData;
};
