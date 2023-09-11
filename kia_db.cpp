#include "kia_db.h"
#include <QDebug>
Kia_db::Kia_db()
{
    create_connection();
}

bool Kia_db::create_connection()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    db.setDatabaseName("kia_bokzm60");
    db.setUserName("postgres");
    db.setPassword("88005553535");
    if (!db.open())
    {
        qDebug() << "cannot open data_base" << db.lastError();
        return false;
    }
    return true;
}

void Kia_db::get_data_from_db()
{

}
