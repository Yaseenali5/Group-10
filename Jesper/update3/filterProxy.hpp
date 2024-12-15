#pragma once

#include <QSortFilterProxyModel>
#include <QDate>

class filterProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit filterProxy(QObject *parent = nullptr);

    void setDateFilter(int month, int day);
    void setLocationFilter(const QString &location);
    void setDateCol(int column);
    void setLocationCol(int column);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    int filterMonth = 0; // Month number to filter, 0 is none
    int filterDay = 0;   // Day number to filter, 0 is none
    QString locFilter;   // Location name to filter by

    int dateCol;
    int locationCol;          
};
