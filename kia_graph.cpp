#include "kia_graph.h"

Kia_graph::Kia_graph(std::shared_ptr<Kia_db> kia_db,
                     std::shared_ptr<Kia_plot_settings> kia_plot_settings,
                     QWidget *parent) :
    QCustomPlot(parent),
    m_kia_db(kia_db),
    m_kia_plot_settings(kia_plot_settings)
{
    init_plot();
    connect(this, SIGNAL(get_data_from_db(QString, QString)), m_kia_db.get(), SLOT(get_data_from_db_slot(QString, QString)));
    connect(this, SIGNAL(send_data_on_plot()), this, SLOT(set_data_on_plot_slot()));
    connect(this, SIGNAL(change_range()), this, SLOT(change_range_slot()));
}

void Kia_graph::init_plot()
{

    set_style();
    setAttribute(Qt::WA_OpaquePaintEvent);
    setNoAntialiasingOnDrag(true);
    setInteraction(QCP::iRangeDrag, true);
    setInteraction(QCP::iRangeZoom, true);
    addGraph();
    graph()->setScatterStyle(QCPScatterStyle::ssDisc);
    graph()->setLineStyle(QCPGraph::lsNone);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void Kia_graph::set_style()
{
    QList<QCPAxis*> axises  = {xAxis, yAxis};
    for (auto a : axises)
    {
        a->setBasePen(QPen(Qt::black, 1));
        a->setTickPen(QPen(Qt::black, 1));
        a->setSubTickPen(QPen(Qt::black, 1));
        a->setTickLabelColor(Qt::black);
        a->grid()->setPen(QPen(Qt::gray, 1, Qt::DotLine));
        //a->grid()->setSubGridPen(QPen(Qt::gray, 0, Qt::DotLine));
        a->grid()->setSubGridVisible(false);
        a->grid()->setZeroLinePen(Qt::NoPen);
        a->setLabelColor(Qt::black);
        QFont font("JetBrains Mono ExtraLight", 10);
        a->setLabelFont(font);
        a->setTickLabelFont(font);
    }
    setBackground(QColor{255,255,255});
    replot();
}

void Kia_graph::get_data_from_db_slot()
{
    fut = std::async([this]()
    {
        QTime begin = QTime(0, 0, 0).addMSecs((xAxis->range().lower) * 1000);
        QTime end = QTime(0, 0, 0).addMSecs((xAxis->range().upper) * 1000);
        if (m_kia_plot_settings->m_kias_graph->m_key - last_point_key >= 1)
        {
            emit get_data_from_db(begin.toString("hh:mm:ss.zzz"), end.toString("hh:mm:ss.zzz"));
            emit send_data_on_plot();
            last_point_key = m_kia_plot_settings->m_kias_graph->m_key;
        }
        emit change_range();
    });

}

void Kia_graph::set_data_on_plot_slot()
{
    //qDebug() << QTime::currentTime();


    for (uint16_t ind = 0; ind < m_kia_plot_settings->m_kias_db->m_x_value.size(); ind++)
    {
        QTime timeStart = m_kia_plot_settings->m_kias_db->m_x_value[ind].toTime();
        double key = QTime(0, 0, 0).secsTo(timeStart);
        m_xData.push_back(key);
        m_yData.push_back(m_kia_plot_settings->m_kias_db->m_y_value[ind].toInt());
    }
    graph()->setData(m_xData, m_yData);
    m_xData.clear();
    m_yData.clear();

}

void Kia_graph::change_range_slot()
{
    if (m_kia_plot_settings->m_kias_graph->m_is_change_range)
    {
        m_kia_plot_settings->m_kias_graph->m_key = m_kia_plot_settings->m_kias_graph->m_key + 1 / 1000.0;
        xAxis->setRange(m_kia_plot_settings->m_kias_graph->m_key, m_kia_plot_settings->m_kias_graph->m_size, Qt::AlignRight);
        replot(QCustomPlot::rpQueuedReplot);
        update();

    }
}
