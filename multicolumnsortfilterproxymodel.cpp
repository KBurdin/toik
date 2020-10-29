#include <QVariant>
#include <QDate>
#include <QDebug>
#include <QFont>

#include "multicolumnsortfilterproxymodel.h"

MultiColumnSortFilterProxyModel::MultiColumnSortFilterProxyModel(QObject *parent):
    QSortFilterProxyModel(parent), m_sortOrder(Qt::AscendingOrder)
{
    maxPrintRows = 0;
    curPrintRow = 0;
    countPrintRow = 0;

}

/*
int MultiColumnSortFilterProxyModel::rowCount(const QModelIndex &parent) const
{
    return maxPrintRows == 0 ? QSortFilterProxyModel::rowCount() : maxPrintRows;
}
*/

QVariant MultiColumnSortFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if(orientation == Qt::Horizontal) {
            for (int i = 0; i < m_sortedColumns.size(); i++) {
                if (m_sortedColumns.at(i).first == section) {
                    QString sortIndicator = m_sortedColumns.at(i).second == Qt::AscendingOrder ? QString(0x2193) : QString(0x2191);
                    if (i > 0)
                        sortIndicator += QString::number(i + 1);
                    return sortIndicator + " " + sourceModel()->headerData(section, orientation, role).toString();
                }
            }
        }
    } else if (role == Qt::FontRole) {
        return QFont("Arial", 8);
    }
    return sourceModel()->headerData(section, orientation, role);
}

QList<QPair<int, Qt::SortOrder> > MultiColumnSortFilterProxyModel::sortedColumns()
{
    return m_sortedColumns;
}

void MultiColumnSortFilterProxyModel::setSortedColumns(QList<QPair<int, Qt::SortOrder> > sortedColumns)
{
    m_sortedColumns = sortedColumns;
    sort();
}

void MultiColumnSortFilterProxyModel::addSortedColumn(int column, Qt::SortOrder order)
{
    m_sortOrder = order;
    QPair<int, Qt::SortOrder> pair;
    pair.first = column;
    pair.second = order;
    m_sortedColumns << pair;
    sort();
}

void MultiColumnSortFilterProxyModel::removeSortedColumn(int column)
{
    for (int i = 0; i < m_sortedColumns.size(); i++) {
        if (m_sortedColumns.at(i).first == column) {
            m_sortedColumns.removeAt(i);
            sort();
            break;
        }
    }
}

void MultiColumnSortFilterProxyModel::changeSortedColumn(int column, Qt::SortOrder order)
{
    for (int i = 0; i < m_sortedColumns.size(); i++) {
        if (m_sortedColumns.at(i).first == column) {
            m_sortOrder = order;
            m_sortedColumns[i].second = order;
            sort();
            break;
        }
    }
}

Qt::SortOrder MultiColumnSortFilterProxyModel::sortOrder() const
{
    return m_sortOrder;
}

Qt::SortOrder MultiColumnSortFilterProxyModel::sortOrder(int column) const
{
    int i;

    for (i = 0; i < m_sortedColumns.size(); i++) {
        if (m_sortedColumns.at(i).first == column) {
            break;
        }
    }
    return m_sortedColumns.at(i).second;
}

bool MultiColumnSortFilterProxyModel::isColumnInSort(int column) const
{
    for (int i = 0; i < m_sortedColumns.size(); i++) {
        if (m_sortedColumns.at(i).first == column) {
            return true;
        }
    }
    return false;
}

void MultiColumnSortFilterProxyModel::setMaxPrintRows(int maxRows)
{
    //maxPrintRows = maxRows;
    maxPrintRows = QSortFilterProxyModel::rowCount();
    if (maxRows < maxPrintRows) {
        maxPrintRows = maxRows;
    }
}

void MultiColumnSortFilterProxyModel::sort()
{
    int column = m_sortedColumns.isEmpty() ? -1 : 0;
    QSortFilterProxyModel::sort(column, Qt::AscendingOrder);
}

bool MultiColumnSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    for (int i = 0; i < m_sortedColumns.size(); i++) {
        QVariant leftData = sourceModel()->index(left.row(), m_sortedColumns.at(i).first).data(Qt::DisplayRole);
        QVariant rightData = sourceModel()->index(right.row(), m_sortedColumns.at(i).first).data(Qt::DisplayRole);
        Qt::SortOrder order = m_sortedColumns.at(i).second;

        if (leftData == rightData)
            continue;

        switch (leftData.type()) {
        case QVariant::Int:
            return (leftData.toInt() < rightData.toInt()) ^ order;
            break;
        case QVariant::UInt:
            return (leftData.toUInt() < rightData.toUInt()) ^ order;
            break;
        case QVariant::LongLong:
            return (leftData.toLongLong() < rightData.toLongLong()) ^ order;
            break;
        case QVariant::ULongLong:
            return (leftData.toULongLong() < rightData.toULongLong()) ^ order;
            break;
        case QVariant::Double:
            return (leftData.toDouble() < rightData.toDouble()) ^ order;
            break;
        case QVariant::Char:
            return (leftData.toChar() < rightData.toChar()) ^ order;
            break;
        case QVariant::Date:
            return (leftData.toDate() < rightData.toDate()) ^ order;
            break;
        case QVariant::Time:
            return (leftData.toTime() < rightData.toTime()) ^ order;
            break;
        case QVariant::DateTime:
            return (leftData.toDateTime() < rightData.toDateTime()) ^ order;
            break;
        case QVariant::String: default:
            return (QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0) ^ order;
            break;
        }
    }
    return false;
}

QVariant MultiColumnSortFilterProxyModel::data(const QModelIndex &index, int role) const
{
    curPrintRow = index.row();
    if ( (maxPrintRows == 0) || ((maxPrintRows != 0) && (curPrintRow < maxPrintRows)) ) {
        if (curPrintRow == index.model()->rowCount()) {
            curPrintRow = 0;
        }
        return QSortFilterProxyModel::data(index, role);
    }
}

int MultiColumnSortFilterProxyModel::rowCount(const QModelIndex &parent) const
{
    return maxPrintRows == 0 ? QSortFilterProxyModel::rowCount(parent) : maxPrintRows;
}

void MultiColumnSortFilterProxyModel::clearSort()
{
    m_sortedColumns.clear();
    invalidate();
    sort();
}

