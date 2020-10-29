#ifndef REFTABLEMODEL_H
#define REFTABLEMODEL_H

#include <QSqlQueryModel>

#include "sqlmodel.h"

class RefTableModel: public SqlModel
{

public:
    RefTableModel(int commentColumn, QObject *parent = nullptr);

private:
    QVariant data(const QModelIndex &index, int role) const;

    int commentColumnNum;

};

#endif // REFTABLEMODEL_H
