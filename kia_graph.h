#ifndef KIA_GRAPH_H
#define KIA_GRAPH_H

#include <QDialog>
#include "kia_struct.h"
#include "kia_db.h"
#include <qcustomplot.h>
#include <QLayout>
#include <QTime>
#include <QTimer>
namespace Ui {
class Kia_graph;
}

class Kia_graph : public QDialog
{
    Q_OBJECT

public:
    explicit Kia_graph(std::shared_ptr<Kia_db> kia_db,
                       std::shared_ptr<Kia_plot_settings> kia_plot_settings,
                       QWidget *parent = nullptr);
    ~Kia_graph();
    void add_plot_on_widget();
    QCustomPlot* get_plot();
private slots:
    void get_data_from_db_slot();
    void change_range_slot();
private:
    void add_data();
    void change_range();
    Ui::Kia_graph *ui;
    std::shared_ptr<Kia_db> m_kia_db;
    std::shared_ptr<Kia_plot_settings> m_kia_plot_settings;
    QCustomPlot* m_plot;
    QHBoxLayout* m_layout_for_plot;
    QTimer data_timer;
    QTimer range_timer;
    QVector<double> m_xData, m_yData;

    QCPRange m_range;
    double m_start_time = 0;
};

#endif // KIA_GRAPH_H
