#ifndef SQLMODEL_H
#define SQLMODEL_H

#include <QSqlQueryModel>

//#include "weekcalc.h"

class SqlModel : public QSqlQueryModel
{
Q_OBJECT

public:
    SqlModel(QObject *parent = nullptr);

public slots:
    void updateModel();

};

#endif // SQLMODEL_H
