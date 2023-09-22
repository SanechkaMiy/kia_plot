#ifndef KIA_GRAPH_MANAGER_H
#define KIA_GRAPH_MANAGER_H
#include <QObject>
#include <QDialog>
#include "kia_graph.h"
#include "kia_struct.h"
#include <QTimer>
class Kia_graph_manager : public QObject
{
    Q_OBJECT
public:
    Kia_graph_manager(std::shared_ptr<Kia_plot_settings> kia_plot_settings,
                      QWidget *parent);
    void create_plot_widget(QWidget *parent);
private slots:
    void show_data_point(QCPAbstractPlottable *plottable, int dataIndex);
private:
    void plots_interactions();
    void start_data_timer(uint16_t& num_plot);
    QVector<QVBoxLayout*>m_l_for_plots;
    QVector<QDialog*> m_dialog;
    QVector<Kia_graph*> m_kia_graph;
    QVector<QCustomPlot*> m_plot;
    std::shared_ptr<Kia_plot_settings> m_kia_plot_settings;
    std::vector<std::shared_ptr<Kia_db>> m_kia_db;
    std::shared_ptr<QTimer> m_timer;
    double m_start_time = 0;
};

#endif // KIA_GRAPH_MANAGER_H
