#ifndef SSTSDIALOG_H
#define SSTSDIALOG_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class SSTSDialog : public QWidget
{
    Q_OBJECT

public:
    SSTSDialog();

protected:
    void nop();

private:
    QPushButton *testButton2;
    QHBoxLayout *testLayout2;

};

#endif // SSTSDIALOG_H
