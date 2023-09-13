#ifndef KIA_DB_H
#define KIA_DB_H
#include "kia_struct.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QTime>
#include <QSqlQuery>
class Kia_db
{
public:
    Kia_db(std::shared_ptr<Kia_plot_settings> kia_plot_settings);
    bool create_connection();
    void get_data_from_db(const QString& begin, const QString& end);
private:
    std::shared_ptr<Kia_plot_settings> m_kia_plot_settings;
};

#endif // KIA_DB_H
