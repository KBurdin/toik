#include <QtWidgets>
#include <QFont>
#include <QSqlQuery>
#include <QDesktopServices>
#include <QMessageBox>
#include <QStatusBar>
#include <QSqlTableModel>

#include "mainwindow.h"
#include "dbman.h"
#include "rpdialog.h"
#include "multicolumnsortfilterproxymodel.h"


RPDialog::RPDialog(const QString &query, SqlModel *model, const QString &title) :
    MainDialog(query, model, title) {

    formatTable();
    getTableView()->setColumnHidden(RP_ID, true);
    getTableView()->setColumnHidden(RP_WEEK, true);
    getTableView()->setColumnHidden(RP_YEAR, true);
    getTableView()->setColumnHidden(RP_STATUS_VAL, true);
    getTableView()->setColumnHidden(RP_FIND, true);
    getProxyModel()->setFilterKeyColumn(RP_FIND);

    /*
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_WEEK, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_WEEK_TEXT, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_KKS, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_PART_NUM, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_SERIAL_NUM, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_AMOUNT, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_PERIOD, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_STATUS, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_REPORT, QHeaderView::Fixed);
    getTableView()->horizontalHeader()->setSectionResizeMode(RP_COMMENT, QHeaderView::Fixed);
    */

    ((MultiColumnSortFilterProxyModel *) getProxyModel())->addSortedColumn(RP_WEEK_TEXT, Qt::DescendingOrder);
    ((MultiColumnSortFilterProxyModel *) getProxyModel())->addSortedColumn(RP_ID, Qt::DescendingOrder);

    getTableView()->show();
}

void RPDialog::formatTable()
{    
    getSqlQueryModel()->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    getSqlQueryModel()->setHeaderData(1, Qt::Horizontal, QObject::tr("Working period val"));
    getSqlQueryModel()->setHeaderData(2, Qt::Horizontal, QObject::tr("Processing date"));
    getSqlQueryModel()->setHeaderData(3, Qt::Horizontal, QObject::tr("Year"));
    getSqlQueryModel()->setHeaderData(4, Qt::Horizontal, QObject::tr("KKS"));
    getSqlQueryModel()->setHeaderData(5, Qt::Horizontal, QObject::tr("Part number"));
    getSqlQueryModel()->setHeaderData(6, Qt::Horizontal, QObject::tr("Serial number"));
    getSqlQueryModel()->setHeaderData(7, Qt::Horizontal, QObject::tr("Manpower", "rpdialog"));
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
    getTableView()->horizontalHeader()->resizeSection(7, 100); // Труд.
    getTableView()->horizontalHeader()->resizeSection(8, 100); // Период
    getTableView()->horizontalHeader()->resizeSection(9, 120); // Состояние
    getTableView()->horizontalHeader()->resizeSection(10, 130); // Протокол
    getTableView()->horizontalHeader()->resizeSection(11, 100); // Состояние val


    //getTableView()->sortByColumn(2, Qt::AscendingOrder); // Период проведения

    getTableView()->setSelectionBehavior(QAbstractItemView::SelectRows);
    getTableView()->verticalHeader()->setDefaultSectionSize(22);

}
