#include <QSqlQuery>

#include "sqlmodel.h"


SqlModel::SqlModel(QObject *parent): QSqlQueryModel(parent)
{

}

void SqlModel::updateModel()
{
    setQuery(query().lastQuery());
}

