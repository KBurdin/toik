#include <QTimer>
#include <QTime>
#include <QDateTime>

#include "clocklabel.h"

ClockLabel::ClockLabel(QWidget *parent) : QLabel(parent),
    timer(new QTimer(this))
{

    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &ClockLabel::updateTime);
    timer->start();

    updateTime();
}

void ClockLabel::updateTime()
{
    setText(QTime::currentTime().toString("hh:mm"));
}
