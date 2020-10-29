#include <QDate>

#include "weekcalc.h"

QString WeekCalc::getWeekRange(int weekNum, int year, int curyear)
{

    int week = weekNum;

    QDate fdate;

    if (getFirstDayOfWeek(fdate, year, week) != -1) {
        QDate ldate = fdate.addDays(6);

        return QString(fdate.toString("dd.MM" + QString(fdate.year() != curyear ? ".yy-" : "-"))
                           + ldate.toString("dd.MM" + QString(ldate.year() != curyear ? ".yy" : "")));
    }

    return "";

}

QString WeekCalc::getWeekRange(const weekDate_s &weekDate)
{
    return getWeekRange(weekDate.week, weekDate.year, weekDate.curyear);
}

int WeekCalc::getFirstDayOfWeek(QDate& date, int year, int week)
{

    /*
    Check week validity
    */

    QDate firstDate;

    firstDate.setDate(year, 1, 1);
    int dayOfWeek = firstDate.dayOfWeek();

    date = firstDate.addDays( ((week - 1) * 7) + ((dayOfWeek > 4) ? (7 - dayOfWeek + 1)  : (-(dayOfWeek - 1)) ) );

    return 0;
}
