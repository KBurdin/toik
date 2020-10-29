#include <QApplication>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "dbman.h"

DbManager::DbManager(const QString& db_path):
    m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
   //m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(db_path);

   if (!m_db.open()) {
      qDebug() << "Error: connection with database fail. " <<  m_db.lastError().text();

   } else {
      qDebug() << "Database: connection ok (" + db_path + ")";
   }
}

int DbManager::columnProc(const QString &proc_query)
{
    QSqlQuery query;
    if (!query.exec(proc_query)) {
        qDebug() << query.lastError().text();

    } else {
        while (query.next()) {
            return query.value(0).toInt();
        }
    }
    return 0;
}

int DbManager::sqlProc(const QString &proc_query)
{
    QSqlQuery query;
    int res = query.exec(proc_query);
    if (!res) {
        qDebug() << query.lastError().text();
    }
    return res;
}
