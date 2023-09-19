#include "kia_graph_manager.h"

Kia_graph_manager::Kia_graph_manager(std::shared_ptr<Kia_db> kia_db,
                                     std::shared_ptr<Kia_plot_settings> kia_plot_settings,
                                     QWidget *parent) :
    m_kia_plot_settings(kia_plot_settings),
    m_kia_db(kia_db)
{
    create_plot_widget(parent);
    plots_interactions();
}

void Kia_graph_manager::create_plot_widget(QWidget *parent)
{
    m_kia_graph.push_back(std::make_shared<Kia_graph>(m_kia_db, m_kia_plot_settings, parent));
    m_kia_graph.push_back(std::make_shared<Kia_graph>(m_kia_db, m_kia_plot_settings, parent));
    m_kia_graph[0]->show();
    m_kia_graph[1]->show();
}

void Kia_graph_manager::show_data_point(QCPAbstractPlottable *plottable, int dataIndex)
{
    double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    qDebug() << dataValue << m_kia_plot_settings->m_kias_db->m_x_value[dataIndex].toTime();
}

void Kia_graph_manager::plots_interactions()
{
    for (auto el : m_kia_graph)
    {
        auto plot = el->get_plot();
        m_plot.push_back(plot);
        plot->connect(plot, &QCustomPlot::mouseWheel, [=](QWheelEvent* e)
        {
            m_kia_plot_settings->m_kias_graph->m_is_change_range = false;
            auto orient = (e->modifiers() != Qt::ControlModifier) ? Qt::Horizontal : Qt::Vertical;
            plot->axisRect()->setRangeZoom(orient);
            if (!(e->modifiers() == Qt::ControlModifier))
            {
                if((e->angleDelta().y()) > 0)
                {
                    if (m_kia_plot_settings->m_kias_graph->m_size > 8)
                        m_kia_plot_settings->m_kias_graph->m_size = m_kia_plot_settings->m_kias_graph->m_size - 0.5;

                }
                else
                {
                    m_kia_plot_settings->m_kias_graph->m_size = m_kia_plot_settings->m_kias_graph->m_size + 0.5;
                }
            }

            //plot->wheelEvent(e);
            e->accept();
            m_kia_plot_settings->m_kias_graph->m_is_change_range = true;
        });
        plot->connect(plot, &QCustomPlot::mousePress, [=](auto e)
        {
            if (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::ControlModifier)
            {
                plot->setInteraction(QCP::iRangeZoom);
                plot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
            }
            else if (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::NoModifier)
            {
                plot->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
                m_kia_plot_settings->m_kias_graph->m_is_change_range = false;
            }
            QSharedPointer<QMetaObject::Connection> x(new QMetaObject::Connection);
            *x = plot->connect(plot, &QCustomPlot::beforeReplot, [=]()
            {
                plot->disconnect(*x);
            });
        });

        plot->connect(plot, &QCustomPlot::mouseRelease, [=](auto e)
        {
            m_kia_plot_settings->m_kias_graph->m_key = plot->xAxis->range().upper;
            m_kia_plot_settings->m_kias_graph->m_is_change_range = true;

        });
        connect(plot, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(show_data_point(QCPAbstractPlottable*,int)));
    }
    for (uint16_t num_plot = 0; num_plot < m_plot.size(); ++num_plot)
    {
        connect(m_plot[num_plot]->xAxis, qOverload<const QCPRange&>(&QCPAxis::rangeChanged),
                this, [this](const QCPRange& range)
        {
            for (uint16_t num_plot = 0; num_plot < m_plot.size(); ++num_plot)
            {
                m_plot[num_plot]->xAxis->setRange(range);
                m_plot[num_plot]->replot();
            }
        });
    }
}

