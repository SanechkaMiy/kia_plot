#ifndef KIA_DB_H
#define KIA_DB_H
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
class Kia_db
{
public:
    Kia_db();
    bool create_connection();
    void get_data_from_db();
};

#endif // KIA_DB_H
