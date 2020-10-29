#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QList>
#include <QVector>
#include <QStringList>
#include <QLine>
#include <QBrush>
#include <QModelIndex>
#include <QDebug>

#include "printdialog.h"

void PrintDialog::tablePrint(QTableView *tableView, QPrinter *printer)
{    
    QString headerName;
    int k = 0;

    if (tableView) {
        QAbstractItemModel* model = tableView->model();
        QHeaderView* hHeader = tableView->horizontalHeader();
        QHeaderView* vHeader = tableView->verticalHeader();

        if (model) {
            //QPrintDialog *printDialog = new QPrintDialog(printer);

            //if (printDialog->exec() == QDialog::Accepted) {
            {
            QPainter painter(printer);

          if (!painter.isActive())
            return;

          int pageLeft = 0;
          int pageTop = 0;
          int pageHeight = painter.device()->height();
          int pageWidth = painter.device()->width();
          int x = pageLeft;
          int y = pageTop;
          QList<QVector<QLine> > linePages;
          QVector<QLine> verticalLines;
          QList<QList<int> > sectionPages;
          QList<int> sections;
          QList<QList<int> > widthPages;
          QList<int> widths;
          QList<int> pageRights;
          QList<QStringList> pageLabels;
          QStringList labels;
          int logicalIndex;

          // Перечисляем столбцы
          for (int i = 0; i < hHeader->count(); ++i) {
            logicalIndex = hHeader->logicalIndex(i);
            headerName = model->headerData(i, Qt::Horizontal).toString();
            //qDebug() << headerName + " : " + QString::number(k, 10);
            if (k == 0) {
              if (headerName == "Status val") {
                  k++;
              }
            } else {
                k++;
            }
            // Если столбец не скрыт
            if (!hHeader->isSectionHidden(logicalIndex)) {

              // Не используем расширение на новую страницу, обрезаем поле
              // Если уже не помещаемся на страницу, добавляем новую
              /*
              if (pageWidth <= x + hHeader->sectionSize(logicalIndex)) {

                verticalLines.append(QLine(x, pageTop, x, pageHeight));
                pageRights.append(x);
                x = pageLeft;
                sectionPages.append(sections);
                sections.clear();
                linePages.append(verticalLines);
                verticalLines.clear();
                widthPages.append(widths);
                widths.clear();
                pageLabels.append(labels);
                labels.clear();
              }
              */

              // Добавляем, так как колонка видимая
              sections.append(logicalIndex);

              if (k == 2) {
                  widths.append(pageWidth - x);
              } else {
                  widths.append(hHeader->sectionSize(logicalIndex));
              }

              labels.append(model->headerData(logicalIndex, Qt::Horizontal).toString());
              verticalLines.append(QLine(x, pageTop, x, pageHeight));
              // Смещаемся правее на ширину колонки
              if (k == 2) {
                x += pageWidth - x;
              } else {
                x += hHeader->sectionSize(logicalIndex);
              }
            }
          }

          if (!sections.isEmpty()) {
            verticalLines.append(QLine(x, pageTop, x, pageHeight));
            pageRights.append(x);
            x = pageLeft;
            sectionPages.append(sections);
            sections.clear();
            linePages.append(verticalLines);
            verticalLines.clear();
            widthPages.append(widths);
            widths.clear();
            pageLabels.append(labels);
            labels.clear();
          }

          int columnPageCount = sectionPages.count();
          int columnCount = sections.count();
          int rowCount = model->rowCount(tableView->rootIndex());
          int rowHeight;
          int columnWidth;
          QModelIndex currentIndex;
          int row = 0;
          int lastPageRow = 0;
          int columnPageIndex = 0;
          int rightColumn;

          QFont font("Arial", 8);
          painter.setFont(font);

          while (true) {
            // Получаем колонки текущей страницы
            sections = sectionPages.at(columnPageIndex);
            widths = widthPages.at(columnPageIndex);
            verticalLines = linePages.at(columnPageIndex);
            // Количество колонок текущей страницы
            columnCount = sections.count();
            rightColumn = pageRights.at(columnPageIndex);
            labels = pageLabels.at(columnPageIndex);
            // Перемещаемся в левый верхний угол страницы
            x = pageLeft;
            y = pageTop;
            rowHeight = hHeader->height();

            // Рисуем колонки
            for (int i = 0; i < columnCount; ++i) {
              // Текущее значение
              columnWidth = widths.at(i);
              painter.drawText(x, y, columnWidth, rowHeight
                               , currentIndex.data(Qt::TextAlignmentRole).toInt()
                               | Qt::TextWordWrap
                               | Qt::AlignVCenter
                               | Qt::AlignHCenter
                               , labels.at(i)
                              );
              painter.drawLine(pageLeft, y, rightColumn, y);
              // Перемащаемся правее
              x += widths.at(i);
            }

            y += rowHeight;
            x = pageLeft;

            // Пока можем рисовать на текущей странице
            while (true) {
              // Высота текущей строки
              rowHeight = vHeader->sectionSize(row);

              if (y + rowHeight > pageHeight) {
                //--row;
                break;
              }

              // Рисуем колонки
              for (int i = 0; i < columnCount; ++i) {
                // Текущее значение
                currentIndex = model->index(row, sections.at(i), tableView->rootIndex());
                columnWidth = widths.at(i);

                if (!currentIndex.data().isNull())
                  painter.drawText(x, y, columnWidth, rowHeight
                                   , currentIndex.data(Qt::TextAlignmentRole).toInt()
                                   | Qt::TextWordWrap
                                   | Qt::AlignVCenter
                                   | Qt::AlignHCenter
                                   , currentIndex.data().toString()
                                  );

                painter.drawLine(pageLeft, y, rightColumn, y);
                // Перемещаемся правее
                x += widths.at(i);
              }

              x = pageLeft;
              // Перемещаемся ниже
              y += rowHeight;
              // Берем следующую строку
              ++row;

              // Если строки кончились
              if (row >= rowCount)
                break;
            }

            // Рисуем завершающую горизонтальную линию
            painter.drawLine(pageLeft, y, rightColumn, y);
            // Рисуем вертикальные линии
            painter.drawLines(verticalLines);
            // Вытираем лишние концы
            // TODO: hard-coded background color for page
            painter.fillRect(pageLeft, y + 1, pageWidth, pageHeight - (y + 1), QBrush(Qt::white));

            // Если предыдущие колонки не поместились
            if (columnPageIndex < columnPageCount - 1) {
              ++columnPageIndex;
              row = lastPageRow;
            // Предыдущие колонки поместились
            } else {
              columnPageIndex = 0;
              lastPageRow = row;

              // Строки закончились
              if (row >= rowCount)
                break;
            }

            // Создаем новую страницу
            printer->newPage();
          }
         } // if (printDialog->exec())
        } // if (model)
      } // if (tableView)
}

void PrintDialog::printPreview()
{
    preview->exec();
}
