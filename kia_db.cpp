#include "kia_db.h"
#include <QDebug>
Kia_db::Kia_db(const QString &name_connection, std::shared_ptr<Kia_plot_settings> kia_plot_settings):
    m_name_connection(name_connection),
    m_kia_plot_settings(kia_plot_settings)
{
    create_connection(name_connection);

}

bool Kia_db::create_connection(const QString& name_connection)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", name_connection);
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
    m_query = new QSqlQuery(db);
    return true;
}

void Kia_db::get_data_from_db_slot(QString begin, QString end)
{
        m_kia_plot_settings->m_kias_db->m_x_value.clear();
        m_kia_plot_settings->m_kias_db->m_y_value.clear();
        //qDebug() << begin << end;
        qDebug() << m_name_connection;
        if (!m_query->exec("SELECT datetime, bshv FROM bokzm60.mshior "
                           "WHERE host_id='04:92:26:d0:ef:d6'::macaddr "
                           "AND experiment_id='2023-09-13 16:47:48.348' AND serial_num=0 "
                           "AND datetime >= '2023-09-13 " + begin + "' AND datetime <='2023-09-13 " + end + "';"))
        {
            qDebug() <<"Unable to execute query";
        }
        QSqlRecord rec = m_query->record();
        QDateTime date_time;
        qint32 bshv;
        while(m_query->next())
        {
            date_time = m_query->value(rec.indexOf("datetime")).toDateTime();
            bshv = m_query->value(rec.indexOf("bshv")).toInt();
            m_kia_plot_settings->m_kias_db->m_x_value.push_back(date_time.time());
            m_kia_plot_settings->m_kias_db->m_y_value.push_back(bshv);
            //qDebug() <<  date_time.time() << bshv;
        }


}

