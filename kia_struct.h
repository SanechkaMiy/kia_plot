#ifndef KIA_STRUCT_H
#define KIA_STRUCT_H
#include <QTime>
#include <QVariant>
#include <iostream>
#include <future>
#include <memory>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>

struct Kias_graph
{
    double m_key = 0;
    double m_size = 8;
    std::atomic_bool m_is_change_range{true};
};

struct Kias_db
{
    QVector<QVariant> m_x_value;
    QVector<QVariant> m_y_value;
};

struct Kia_plot_settings
{
    Kia_plot_settings()
    {
        m_kias_db.reset(new Kias_db());
        m_kias_graph.reset(new Kias_graph());
    }
    std::shared_ptr<Kias_db> m_kias_db;
    std::shared_ptr<Kias_graph> m_kias_graph;

};
#endif // KIA_STRUCT_H
