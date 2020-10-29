#ifndef CLOCKLABEL_H
#define CLOCKLABEL_H

#include <QLabel>

class ClockLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClockLabel(QWidget *parent = nullptr);

private slots:
    void updateTime();

private:
    QTimer *timer;

};

#endif // CLOCKLABEL_H
