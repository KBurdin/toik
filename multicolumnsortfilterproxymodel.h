#ifndef MULTICOLUMNSORTFILTERPROXYMODEL_H
#define MULTICOLUMNSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QList>
#include <QPair>
#include <QVariant>

class MultiColumnSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MultiColumnSortFilterProxyModel(QObject *parent = Q_NULLPTR);

    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QList<QPair<int, Qt::SortOrder> > sortedColumns();
    void setSortedColumns(QList<QPair<int, Qt::SortOrder> > sortedColumns);
    void addSortedColumn(int column, Qt::SortOrder order);
    void removeSortedColumn(int column);
    void changeSortedColumn(int column, Qt::SortOrder order);
    Qt::SortOrder sortOrder() const;
    Qt::SortOrder sortOrder(int column) const;

    bool isColumnInSort(int column) const;

    void setMaxPrintRows(int maxRows);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;

protected:
    //int rowCount(const QModelIndex &parent) const;
    void sort();
    bool lessThan ( const QModelIndex & left, const QModelIndex & right ) const;


public slots:
    void clearSort();

private:
    QList<QPair<int, Qt::SortOrder> > m_sortedColumns;
    Qt::SortOrder m_sortOrder;

    mutable int maxPrintRows;
    mutable int curPrintRow;
    mutable int countPrintRow;

};

#endif // MULTICOLUMNSORTFILTERPROXYMODEL_H
