#ifndef RKTABLEMODEL_H
#define RKTABLEMODEL_H

#include <QSqlQueryModel>

#include "sqlmodel.h"
#include "weekcalc.h"

#define RK_ID           0
#define RK_WEEK         1
#define RK_WEEK_TEXT    2
#define RK_YEAR         3
#define RK_KKS          4
#define RK_PART_NUM     5
#define RK_SERIAL_NUM   6
#define RK_AMOUNT       7
#define RK_PERIOD       8
#define RK_STATUS       9
#define RK_REPORT       10
#define RK_STATUS_VAL   11
#define RK_COMMENT      12
#define RK_FIND         13

#define RK_NDONE        0
#define RK_DONE         1
#define RK_INTIME       2

class RKTableModel: public SqlModel
{

public:
    RKTableModel() = default;

private:
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // RKTABLEMODEL_H
