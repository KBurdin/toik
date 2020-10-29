#ifndef USERSTYPE_H
#define USERSTYPE_H

#include <QString>

struct userStruct
{
        int id;
        QString login;
        QString pass;
        QString f_name;
        QString m_name;
        QString l_name;
        QString position;
        int level;
        QString field_1;
        QString field_2;
        QString field_3;
 };

typedef userStruct userType;

#endif // USERSTYPE_H
