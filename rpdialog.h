#ifndef RPDIALOG_H
#define RPDIALOG_H

#include <QSqlQueryModel>
#include "maindialog.h"

class RPDialog : public MainDialog
{
    Q_OBJECT

public:
    explicit RPDialog(const QString &query, SqlModel *model, const QString &title);
    void formatTable();

};

#endif // RPDIALOG_H
