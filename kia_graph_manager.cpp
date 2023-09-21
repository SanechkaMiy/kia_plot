#include "kia_graph_manager.h"

Kia_graph_manager::Kia_graph_manager(std::shared_ptr<Kia_plot_settings> kia_plot_settings,
                                     QWidget *parent) :
    m_kia_plot_settings(kia_plot_settings)
{
    create_plot_widget(parent);
    plots_interactions();
}

void Kia_graph_manager::create_plot_widget(QWidget *parent)
{
    for (uint16_t num_graph = 0; num_graph < 20; ++num_graph)
    {
        m_timer = std::make_shared<QTimer>();
        m_timer->start(0);
        m_kia_db.push_back(std::make_shared<Kia_db>("con_" + QString::number(num_graph), m_kia_plot_settings));
        m_kia_graph.push_back(new Kia_graph(m_kia_db[num_graph], m_kia_plot_settings));
        //m_l_for_plots->addWidget(m_kia_graph[num_graph]);
        m_kia_graph[num_graph]->show();
    }
}

void Kia_graph_manager::show_data_point(QCPAbstractPlottable *plottable, int dataIndex)
{
    double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    qDebug() << dataValue << m_kia_plot_settings->m_kias_db->m_x_value[dataIndex].toTime();
}

void Kia_graph_manager::plots_interactions()
{
    for (auto plot : m_kia_graph)
    {
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
                    if (m_kia_plot_settings->m_kias_graph->m_size > 1)
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
        start_data_timer(num_plot);
        connect(m_plot[num_plot]->xAxis, qOverload<const QCPRange&>(&QCPAxis::rangeChanged),
                this, [this](const QCPRange& range)
        {
            for (uint16_t num_plot = 0; num_plot < m_plot.size(); ++num_plot)
            {
                m_plot[num_plot]->xAxis->setRange(range);
                m_plot[num_plot]->replot(QCustomPlot::rpQueuedReplot);
                m_plot[num_plot]->update();
            }
        });
    }
}

void Kia_graph_manager::start_data_timer(uint16_t& num_plot)
{
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    m_plot[num_plot]->xAxis->setTicker(timeTicker);
    m_plot[num_plot]->axisRect()->setupFullAxesBox();
    m_plot[num_plot]->yAxis->setRange(-1.2, 1.2);
    QTime start = QTime(0, 0, 0).fromString(QString("16:47:48.348"), "hh:mm:ss.zzz");
    m_start_time = QTime(0, 0, 0).msecsTo(start) / 1000.0;
    m_plot[num_plot]->xAxis->setRange(m_start_time, 8, Qt::AlignRight);
    m_kia_plot_settings->m_kias_graph->m_key = m_start_time;
    connect(m_plot[num_plot]->xAxis, SIGNAL(rangeChanged(QCPRange)), m_plot[num_plot]->xAxis2, SLOT(setRange(QCPRange)));
    connect(m_plot[num_plot]->yAxis, SIGNAL(rangeChanged(QCPRange)), m_plot[num_plot]->yAxis2, SLOT(setRange(QCPRange)));
    // Interval 0 means to refresh as fast as possible
    connect(m_timer.get(), SIGNAL(timeout()), m_kia_graph[num_plot], SLOT(get_data_from_db_slot()));
}

