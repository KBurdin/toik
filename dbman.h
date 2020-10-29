#ifndef DBMAN_H
#define DBMAN_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class DbManager
{
public:
    DbManager(const QString& db_path);
    QSqlDatabase getDB();
    int columnProc(const QString& proc_query);
    int sqlProc(const QString& proc_query);

private:
    QSqlDatabase m_db;
};

#endif // DBMAN_H
