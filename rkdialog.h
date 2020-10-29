#ifndef RKDIALOG_H
#define RKDIALOG_H

#include <QSqlQueryModel>
#include "maindialog.h"

class RKDialog : public MainDialog
{
    Q_OBJECT

public:
    explicit RKDialog(const QString &query, SqlModel *model, const QString &title);
    void formatTable();

};

#endif // RKDIALOG_H
