#ifndef PRINTDIALOG_H
#define PRINTDIALOG_H

#include <QTableView>
#include <QPrinter>
#include <QPrintPreviewDialog>

class PrintDialog
{

public:
    PrintDialog() = default;

    QPrinter *printer;
    QPrintPreviewDialog *preview;

    void tablePrint(QTableView *tableView, QPrinter *printer);
    void printPreview();

};

#endif // PRINTDIALOG_H
