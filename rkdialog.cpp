#include <QtWidgets>
#include <QFont>
#include <QSqlQuery>
#include <QDesktopServices>
#include <QMessageBox>
#include <QStatusBar>
#include <QSqlTableModel>

#include "mainwindow.h"
#include "dbman.h"
#include "rkdialog.h"
#include "multicolumnsortfilterproxymodel.h"


RKDialog::RKDialog(const QString &query, SqlModel *model, const QString &title) :
    MainDialog(query, model, title) {

    formatTable();
    getTableView()->setColumnHidden(RK_ID, true);
    getTableView()->setColumnHidden(RK_WEEK, true);
    getTableView()->setColumnHidden(RK_YEAR, true);
    getTableView()->setColumnHidden(RK_STATUS_VAL, true);
    getTableView()->setColumnHidden(RK_FIND, true);
    getProxyModel()->setFilterKeyColumn(RK_FIND);

    getTableView()->horizontalHeader()->setSectionResizeMode(RK_WEEK_TEXT, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RK_KKS, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RK_PART_NUM, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RK_SERIAL_NUM, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RK_AMOUNT, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RK_PERIOD, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RK_STATUS, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RK_REPORT, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RK_COMMENT, QHeaderView::Fixed);

    ((MultiColumnSortFilterProxyModel *) getProxyModel())->addSortedColumn(RK_WEEK_TEXT, Qt::DescendingOrder);
    ((MultiColumnSortFilterProxyModel *) getProxyModel())->addSortedColumn(RK_ID, Qt::DescendingOrder);

    getTableView()->show();
}

void RKDialog::formatTable()
{
    getSqlQueryModel()->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    getSqlQueryModel()->setHeaderData(1, Qt::Horizontal, QObject::tr("Working period val"));
    getSqlQueryModel()->setHeaderData(2, Qt::Horizontal, QObject::tr("Processing date"));
    getSqlQueryModel()->setHeaderData(3, Qt::Horizontal, QObject::tr("Year"));
    getSqlQueryModel()->setHeaderData(4, Qt::Horizontal, QObject::tr("KKS"));
    getSqlQueryModel()->setHeaderData(5, Qt::Horizontal, QObject::tr("Part number"));
    getSqlQueryModel()->setHeaderData(6, Qt::Horizontal, QObject::tr("Serial number"));
    getSqlQueryModel()->setHeaderData(7, Qt::Horizontal, QObject::tr("Manpower", "rkdialog"));
    getSqlQueryModel()->setHeaderData(8, Qt::Horizontal, QObject::tr("Period"));    
    getSqlQueryModel()->setHeaderData(9, Qt::Horizontal, QObject::tr("Status"));
    getSqlQueryModel()->setHeaderData(10, Qt::Horizontal, QObject::tr("Report"));
    getSqlQueryModel()->setHeaderData(11, Qt::Horizontal, QObject::tr("Status val"));
    getSqlQueryModel()->setHeaderData(12, Qt::Horizontal, QObject::tr("Comment"));
    getSqlQueryModel()->setHeaderData(13, Qt::Horizontal, QObject::tr("Find field"));

    getTableView()->verticalHeader()->setVisible(false);
    //getTableView()->setSortingEnabled(true);
    getTableView()->horizontalHeader()->setStretchLastSection(true);
    getTableView()->horizontalHeader()->resizeSection(0, 130); // id
    getTableView()->horizontalHeader()->resizeSection(1, 140); // Период проведения val
    getTableView()->horizontalHeader()->resizeSection(2, 160); // Период проведения
    getTableView()->horizontalHeader()->resizeSection(3, 60); // Год проведения
    getTableView()->horizontalHeader()->resizeSection(4, 150); // Код ТС
    getTableView()->horizontalHeader()->resizeSection(5, 120); // Модель ТС
    getTableView()->horizontalHeader()->resizeSection(6, 130); // Заводской номер
    getTableView()->horizontalHeader()->resizeSection(7, 100); // Трудозатраты
    getTableView()->horizontalHeader()->resizeSection(8, 100); // Период
    getTableView()->horizontalHeader()->resizeSection(9, 120); // Состояние
    getTableView()->horizontalHeader()->resizeSection(10, 130); // Протокол
    getTableView()->horizontalHeader()->resizeSection(11, 100); // Состояние val

    //getTableView()->sortByColumn(2, Qt::AscendingOrder); // Период проведения

    getTableView()->setSelectionBehavior(QAbstractItemView::SelectRows);
    getTableView()->verticalHeader()->setDefaultSectionSize(22);

}
