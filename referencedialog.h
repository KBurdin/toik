#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QFormLayout>
#include <QLineEdit>
#include <QValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QShortcut>
#include <QLabel>
#include <QSortFilterProxyModel>

class ReferenceDialog: public QWidget
{
    Q_OBJECT

private:
   QGroupBox *mainGroupBox;
   QVBoxLayout *mainVBoxLayout;
   QVBoxLayout *buttonVBoxLayout;
   QHBoxLayout *mainHBoxLayout;
   QHBoxLayout *closeHBoxLayout;
   QVBoxLayout *groupBoxVBoxLayout;
   QHBoxLayout *findHBoxLayout;
   QLabel *findLabel;
   QLineEdit *findLineEdit;
   QPushButton *findPushButton;
   QPushButton *clearPushButton;
   QPushButton *closePushButton;
   QPushButton *addPushButton;
   QPushButton *editPushButton;
   QPushButton *delPushButton;
   QPushButton *viewPushButton;
   QTableView *mainTableView;
   QSqlQueryModel *mainModel;
   QShortcut *deleteShortcut;
   QSortFilterProxyModel *filterModel;

   bool filterFlag;

public:
   explicit ReferenceDialog(QWidget *parent = nullptr, QTableView *table = nullptr);

   void setGroupBoxTitle(const QString& title);
   bool checkRelation(const QString &query_str);
   void hideViewButton();
   void showViewButton();
   void setFilterModel(QSortFilterProxyModel *model);

private slots:
   void setFilter();
   void clearFilter();


signals:

public slots:
   virtual void addAct();
   virtual void editAct();
   virtual void delAct();
   virtual void viewAct();
   void selectionChanged();

};

#endif // REFERENCEDIALOG_H
