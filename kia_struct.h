#ifndef KIA_STRUCT_H
#define KIA_STRUCT_H
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
struct Kia_plot_settings
{
    Kia_plot_settings()
    {
        m_kia_threads.reset(new Kia_threads());
    }
    std::shared_ptr<Kia_threads> m_kia_threads;
    std::queue<double> m_xValue;
    std::queue<double> m_yValue;
};
#endif // KIA_STRUCT_H
