#include <QtWidgets>
#include <QFont>
#include <QSqlQuery>
#include <QDesktopServices>
#include <QMessageBox>
#include <QStatusBar>
#include <QSqlTableModel>

#include "mainwindow.h"
#include "dbman.h"
#include "rtodialog.h"
#include "multicolumnsortfilterproxymodel.h"

RTODialog::RTODialog(const QString &query, SqlModel *model, const QString title) :
    MainDialog(query, model, title) {

    formatTable();

    getTableView()->setColumnHidden(RTO_ID, true);
    getTableView()->setColumnHidden(RTO_WEEK, true);
    getTableView()->setColumnHidden(RTO_YEAR, true);    
    getTableView()->setColumnHidden(RTO_TASK_ID, true);
    getTableView()->setColumnHidden(RTO_TASK_NAME, true);
    getTableView()->setColumnHidden(RTO_DEV_ID, true);
    getTableView()->setColumnHidden(RTO_STATUS_VAL, true);
    getTableView()->setColumnHidden(RTO_FIND, true);
    getProxyModel()->setFilterKeyColumn(RTO_FIND);
    /*
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_WEEK_TEXT, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_TASK_CODE, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_DEV_KKS, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_DEV_PART_NUM, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_TASK_ID, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_DEV_ID, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_AMOUNT, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_PERIOD, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_STATUS, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RTO_COMMENT, QHeaderView::Fixed);
    */

    ((MultiColumnSortFilterProxyModel *) getProxyModel())->addSortedColumn(RTO_WEEK_TEXT, Qt::DescendingOrder);
    ((MultiColumnSortFilterProxyModel *) getProxyModel())->addSortedColumn(RTO_ID, Qt::DescendingOrder);

    getTableView()->show();
    model->updateModel();
}

void RTODialog::formatTable()
{

    getSqlQueryModel()->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    getSqlQueryModel()->setHeaderData(1, Qt::Horizontal, QObject::tr("Processing date val"));
    getSqlQueryModel()->setHeaderData(2, Qt::Horizontal, QObject::tr("Processing date"));
    getSqlQueryModel()->setHeaderData(3, Qt::Horizontal, QObject::tr("Year"));
    getSqlQueryModel()->setHeaderData(4, Qt::Horizontal, QObject::tr("Task code"));    
    getSqlQueryModel()->setHeaderData(5, Qt::Horizontal, QObject::tr("KKS"));
    getSqlQueryModel()->setHeaderData(6, Qt::Horizontal, QObject::tr("Part number"));
    getSqlQueryModel()->setHeaderData(7, Qt::Horizontal, QObject::tr("Task id"));
    getSqlQueryModel()->setHeaderData(8, Qt::Horizontal, QObject::tr("Task name"));
    getSqlQueryModel()->setHeaderData(9, Qt::Horizontal, QObject::tr("Dev id"));
    getSqlQueryModel()->setHeaderData(10, Qt::Horizontal, QObject::tr("Manpower", "rtodialog"));
    getSqlQueryModel()->setHeaderData(11, Qt::Horizontal, QObject::tr("Period"));
    getSqlQueryModel()->setHeaderData(12, Qt::Horizontal, QObject::tr("Status"));
    getSqlQueryModel()->setHeaderData(13, Qt::Horizontal, QObject::tr("Status val"));
    getSqlQueryModel()->setHeaderData(14, Qt::Horizontal, QObject::tr("Comment"));
    getSqlQueryModel()->setHeaderData(15, Qt::Horizontal, QObject::tr("Find field"));

    getTableView()->verticalHeader()->setVisible(false);
    //getTableView()->setSortingEnabled(true);
    getTableView()->horizontalHeader()->setStretchLastSection(true);
    getTableView()->horizontalHeader()->resizeSection(0, 140); // id
    getTableView()->horizontalHeader()->resizeSection(1, 140); // Период проведения val
    getTableView()->horizontalHeader()->resizeSection(2, 160); // Период проведения
    getTableView()->horizontalHeader()->resizeSection(3, 60); // Год проведения
    getTableView()->horizontalHeader()->resizeSection(4, 80); // Код ТО
    getTableView()->horizontalHeader()->resizeSection(5, 150); // Код ТС
    getTableView()->horizontalHeader()->resizeSection(6, 120); // Модель ТС
    getTableView()->horizontalHeader()->resizeSection(7, 110); // ТО id
    getTableView()->horizontalHeader()->resizeSection(8, 130); // ТО name
    getTableView()->horizontalHeader()->resizeSection(9, 110); // Модель id
    getTableView()->horizontalHeader()->resizeSection(10, 100); // Трудозатраты
    getTableView()->horizontalHeader()->resizeSection(11, 100); // Период
    getTableView()->horizontalHeader()->resizeSection(12, 120); // Состояние
    getTableView()->horizontalHeader()->resizeSection(13, 100); // Состояние val

    //getProxyModel()->setSortLocaleAware(true);
    //getTableView()->sortByColumn(2, Qt::AscendingOrder); // Период проведения

    getTableView()->setSelectionBehavior(QAbstractItemView::SelectRows);
    getTableView()->verticalHeader()->setDefaultSectionSize(22);

}
