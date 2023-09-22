#ifndef KIA_GRAPH_H
#define KIA_GRAPH_H
#include "kia_struct.h"
#include "kia_db.h"
#include <future>
#include <qcustomplot.h>

class Kia_graph : public QCustomPlot
{
    Q_OBJECT
public:
    Kia_graph(std::shared_ptr<Kia_db> kia_db,
              std::shared_ptr<Kia_plot_settings> kia_plot_settings, QWidget *parent = nullptr);
signals:
    void get_data_from_db(QString, QString);
    void send_data_on_plot();
    void change_range();
private slots:
    //void get_data_from_db_slot();
    void get_data_from_db_slot();
    void set_data_on_plot_slot();
    void change_range_slot();
private:
    double last_point_key = 0;
    void start_data_timer();
    void init_plot();
    void set_style();
    std::shared_ptr<Kia_db> m_kia_db;
    std::shared_ptr<Kia_plot_settings> m_kia_plot_settings;
    QVector<double> m_xData, m_yData;
    double m_start_time = 0;
    std::future<void> fut;
};

#endif // KIA_GRAPH_H
