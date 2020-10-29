#include <QStringList>
#include <QDebug>

#include "reftablemodel.h"

RefTableModel::RefTableModel(int commentColumn, QObject *parent): SqlModel(parent)
{
    commentColumnNum = commentColumn;
}

QVariant RefTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole) {
        if (index.column() != commentColumnNum) {
            return Qt::AlignCenter;
        } else {
            return QSqlQueryModel::data(index, role);
        }
    } else if (role == Qt::DisplayRole) {
        if (index.column() == commentColumnNum) {
            //return QVariant(QSqlQueryModel::data(index, role).toString().split('\n').first());
            return SqlModel::data(index, role);
        } else {
            return SqlModel::data(index, role);
        }
    } else {
        return SqlModel::data(index, role);
    }
}
