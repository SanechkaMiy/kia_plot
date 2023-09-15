#include "kia_graph.h"
#include "ui_kia_graph.h"
#include <QDebug>
Kia_graph::Kia_graph(std::shared_ptr<Kia_db> kia_db,
                     std::shared_ptr<Kia_plot_settings> kia_plot_settings,
                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kia_graph),
    m_kia_db(kia_db),
    m_kia_plot_settings(kia_plot_settings)
{
    ui->setupUi(this);
    add_plot_on_widget();
    add_data();
    change_range();
}

Kia_graph::~Kia_graph()
{
    delete ui;
}

void Kia_graph::add_plot_on_widget()
{
    m_layout_for_plot = new QHBoxLayout(this);
    m_plot = new QCustomPlot(this);
    m_plot->setInteraction(QCP::iRangeDrag, true);
    m_plot->setInteraction(QCP::iRangeZoom, true);
    m_plot->connect(m_plot, &QCustomPlot::mouseWheel, [=](QWheelEvent* e)
    {
        auto orient = (e->modifiers() != Qt::ControlModifier) ? Qt::Horizontal : Qt::Vertical;
        m_plot->axisRect()->setRangeZoom(orient);
        //plot->wheelEvent(e);
        e->accept();
    });
    connect(m_plot->xAxis, qOverload<const QCPRange&>(&QCPAxis::rangeChanged),
            this, [this](const QCPRange& range)
    {
        //m_plot->xAxis->setRange(range);

    });
    m_plot->connect(m_plot, &QCustomPlot::mousePress, [=](auto e)
    {
        if (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::ControlModifier)
        {
            m_plot->setInteraction(QCP::iRangeZoom);
            m_plot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
        }
        else if (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::NoModifier)
        {
            m_plot->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
            m_is_change_range = false;
        }
        QSharedPointer<QMetaObject::Connection> x(new QMetaObject::Connection);
        *x = m_plot->connect(m_plot, &QCustomPlot::beforeReplot, [=]()
        {
            m_plot->disconnect(*x);
        });
    });

    m_plot->connect(m_plot, &QCustomPlot::mouseRelease, [=](auto e)
    {

        qDebug() << m_key;
        qDebug() << m_plot->xAxis->range().upper;
        m_key = m_plot->xAxis->range().upper;
        m_is_change_range = true;

    });
    m_plot->addGraph();
    m_plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    m_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    m_layout_for_plot->addWidget(m_plot);
}

void Kia_graph::get_data_from_db_slot()
{
    static QTime timeStart = QTime::currentTime();
    double key = timeStart.msecsTo(QTime::currentTime()) / 1000.0;
    static double last_point_key = 0;

    QTime begin = QTime(0, 0, 0).addMSecs((m_plot->xAxis->range().lower) * 1000);
    QTime end = QTime(0, 0, 0).addMSecs((m_plot->xAxis->range().upper) * 1000);
    if (key - last_point_key >= 1)
    {

        m_kia_db->get_data_from_db(begin.toString("hh:mm:ss.zzz"), end.toString("hh:mm:ss.zzz"));
        for (uint16_t ind = 0; ind < m_kia_plot_settings->m_kias_db->m_x_value.size(); ind++)
        {
            QTime timeStart = m_kia_plot_settings->m_kias_db->m_x_value[ind].toTime();
            double key = QTime(0, 0, 0).secsTo(timeStart);
            m_xData.push_back(key);
            m_yData.push_back(m_kia_plot_settings->m_kias_db->m_y_value[ind].toInt());
        }
        m_plot->graph(0)->addData(m_xData, m_yData);
        m_xData.clear();
        m_yData.clear();
        last_point_key = key;
    }
    double lower = (m_plot->xAxis->range().lower * 1000);
    //m_plot->xAxis->setRange(m_key, 8, Qt::AlignRight);
}

void Kia_graph::change_range_slot()
{
    // static QTime timeStart = QTime::currentTime();
    if (m_is_change_range)
    {
        m_key = m_key + 1 / 1000.0;
        //qDebug() << m_delta_range_time;
        //qDebug() << m_key / 1000;

        m_plot->xAxis->setRange(m_key, 8, Qt::AlignRight);
        m_plot->replot();
    }
}

void Kia_graph::add_data()
{
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    m_plot->xAxis->setTicker(timeTicker);
    m_plot->axisRect()->setupFullAxesBox();
    m_plot->yAxis->setRange(-1.2, 1.2);
    QTime start = QTime(0, 0, 0).fromString(QString("16:47:48.348"), "hh:mm:ss.zzz");
    m_start_time = QTime(0, 0, 0).msecsTo(start) / 1000.0;
    m_plot->xAxis->setRange(m_start_time, 8, Qt::AlignRight);
    m_key = m_start_time;
    connect(m_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(m_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->yAxis2, SLOT(setRange(QCPRange)));
    connect(&data_timer, SIGNAL(timeout()), this, SLOT(get_data_from_db_slot()));
    data_timer.start(0); // Interval 0 means to refresh as fast as possible
}

void Kia_graph::change_range()
{
    connect(&range_timer, SIGNAL(timeout()), this, SLOT(change_range_slot()));
    range_timer.start(0); // Interval 0 means to refresh as fast as possible
}
