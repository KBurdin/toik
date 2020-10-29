#ifndef RPTABLEMODEL_H
#define RPTABLEMODEL_H

#include <QSqlQueryModel>

#include "sqlmodel.h"
#include "weekcalc.h"

#define RP_ID           0
#define RP_WEEK         1
#define RP_WEEK_TEXT    2
#define RP_YEAR         3
#define RP_KKS          4
#define RP_PART_NUM     5
#define RP_SERIAL_NUM   6
#define RP_AMOUNT       7
#define RP_PERIOD       8
#define RP_STATUS       9
#define RP_REPORT       10
#define RP_STATUS_VAL   11
#define RP_COMMENT      12
#define RP_FIND         13

#define RP_NDONE        0
#define RP_DONE         1
#define RP_INTIME       2

class RPTableModel: public SqlModel
{

public:
    RPTableModel() = default;

private:
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // RPTABLEMODEL_H
