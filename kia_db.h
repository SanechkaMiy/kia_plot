#ifndef KIA_DB_H
#define KIA_DB_H
#include "kia_struct.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QTime>
#include <QSqlQuery>
#include <QSqlField>
#include <utility>
class Kia_db : public QObject
{
    Q_OBJECT
public:
    Kia_db(const QString& name_connection, std::shared_ptr<Kia_plot_settings> kia_plot_settings);
    bool create_connection(const QString &name_connection);
    QStringList get_all_type_dev();
private slots:
    void get_data_from_db_slot(QString begin, QString end);
private:
    QString m_name_connection;
    std::shared_ptr<Kia_plot_settings> m_kia_plot_settings;
    QSqlQuery* m_query;
};

#endif // KIA_DB_H
