#ifndef RTODIALOG_H
#define RTODIALOG_H

#include <QSqlQueryModel>
#include <QStyledItemDelegate>

#include "maindialog.h"
#include "rtotablemodel.h"

class RTODialog : public MainDialog
{
    Q_OBJECT

public:
    explicit RTODialog(const QString &query, SqlModel *model, const QString title);
    void formatTable();

};

#endif // RTODIALOG_H
