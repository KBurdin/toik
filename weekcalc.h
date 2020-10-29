#ifndef WEEKCALC_H
#define WEEKCALC_H

#include <QString>
#include <QVariant>
#include <QDate>

struct weekDate_s {
    int week;
    int year;
    int curyear;
};

Q_DECLARE_METATYPE(weekDate_s)

class WeekCalc
{

public:
    WeekCalc() = default;

    static QString getWeekRange(int weekNum, int year, int curyear);
    static QString getWeekRange(const weekDate_s &weekDate);
    static int getFirstDayOfWeek(QDate& date, int year, int week);

};

#endif // WEEKCALC_H
