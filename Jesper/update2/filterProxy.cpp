#include "filterProxy.hpp"
#include <QDate>
#include <iostream>

filterProxy::filterProxy(QObject *parent)
    : QSortFilterProxyModel(parent) {}

void filterProxy::setDateFilter(int month, int day)
{
    filterMonth = month; // Set month to filter
    filterDay = day;     // Set day to filter
    invalidateFilter();  // Reapply filter
}

void filterProxy::setLocationFilter(const QString &location)
{
    locFilter = location;
    invalidateFilter(); // Reapply filter
}

void filterProxy::setDateCol(int column)
{
    dateCol = column;
}

void filterProxy::setLocationCol(int column)
{
    locationCol = column;
}

bool filterProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{

    // If no filters applyed
    if (filterMonth == 0 && filterDay == 0 && locFilter.isEmpty()) {
        return true; 
    }

    // Get location column
    QModelIndex locationIndex = sourceModel()->index(sourceRow, locationCol, sourceParent);
    QString locationString = sourceModel()->data(locationIndex).toString();
    
    // Apply location filter
    if (!locFilter.isEmpty() && !locationString.contains(locFilter, Qt::CaseInsensitive)) {
        return false;
    }
   
    // Get date column
    QModelIndex dateIndex = sourceModel()->index(sourceRow, dateCol, sourceParent);
    QString dateString = sourceModel()->data(dateIndex).toString();
    QDateTime dateTime = QDateTime::fromString(dateString, Qt::ISODate);

   
    if (!dateTime.isValid()) {
         
        return false; 
    }
    
    QDate date = dateTime.date();

    // Filter months
    if (filterMonth != 0 && date.month() != filterMonth) {
        return false;
    }
    // Filter days
    if (filterDay != 0 && date.day() != filterDay) {
        
        return false;
    }

    return true; 
}




