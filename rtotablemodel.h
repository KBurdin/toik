#ifndef RTOTABLEMODEL_H
#define RTOTABLEMODEL_H

#include <QSqlQueryModel>

#include "sqlmodel.h"

#define RTO_ID           0
#define RTO_WEEK         1
#define RTO_WEEK_TEXT    2
#define RTO_YEAR         3
#define RTO_TASK_CODE    4
#define RTO_DEV_KKS      5
#define RTO_DEV_PART_NUM 6
#define RTO_TASK_ID      7
#define RTO_TASK_NAME    8
#define RTO_DEV_ID       9
#define RTO_AMOUNT       10
#define RTO_PERIOD       11
#define RTO_STATUS       12
#define RTO_STATUS_VAL   13
#define RTO_COMMENT      14
#define RTO_FIND         15

#define RTO_NDONE        0
#define RTO_DONE         1
#define RTO_INTIME       2

class RTOTableModel: public SqlModel
{

public:
    RTOTableModel() = default;

private:
    QVariant data(const QModelIndex &index, int role) const;

};

#endif // RTOTABLEMODEL_H
