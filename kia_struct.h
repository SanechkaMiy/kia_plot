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
struct Kia_threads
{
    std::future<void> m_th_get_data;
    std::atomic_bool m_get_data_stop{false};
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
        m_kia_threads.reset(new Kia_threads());
        m_kias_db.reset(new Kias_db());
    }
    std::shared_ptr<Kia_threads> m_kia_threads;
    std::shared_ptr<Kias_db> m_kias_db;
    std::queue<double> m_xValue;
    std::queue<double> m_yValue;
};
#endif // KIA_STRUCT_H
