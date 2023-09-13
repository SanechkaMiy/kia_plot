#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_kia_plot_settings(new Kia_plot_settings()),
    m_kia_db(new Kia_db(m_kia_plot_settings))
{
    ui->setupUi(this);
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
    m_plot->addGraph();
    m_plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    m_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->gl_plots->addWidget(m_plot);
    start_get_data();
}

MainWindow::~MainWindow()
{
    stop_get_data();
    delete ui;
}

void MainWindow::start_get_data()
{
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    m_plot->xAxis->setTicker(timeTicker);
    m_plot->axisRect()->setupFullAxesBox();
    m_plot->yAxis->setRange(-1.2, 1.2);
    QTime start = QTime(0, 0, 0).fromString(QString("16:47:48.348"), "hh:mm:ss.zzz");
    double start_double = QTime(0, 0, 0).msecsTo(start) / 1000.0;
    m_plot->xAxis->setRange(start_double, 8, Qt::AlignRight);
    connect(m_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(m_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->yAxis2, SLOT(setRange(QCPRange)));
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::stop_get_data()
{
    if (m_kia_plot_settings->m_kia_threads->m_get_data_stop)
    {
        m_kia_plot_settings->m_kia_threads->m_get_data_stop =  false;
        m_kia_plot_settings->m_kia_threads->m_th_get_data.get();
    }
}

void MainWindow::realtimeDataSlot()
{
    static QTime timeStart = QTime::currentTime();

    // calculate two new data points:
    double key_1 = timeStart.msecsTo(QTime::currentTime()) / 1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    QTime begin = QTime(0, 0, 0).addMSecs((m_plot->xAxis->range().lower) * 1000);
    QTime end = QTime(0, 0, 0).addMSecs((m_plot->xAxis->range().upper) * 1000);
    if (key_1 - lastPointKey > 1) // at most add point every 2 ms
    {
        // add data to lines:
        m_x = 1 + rand() % 3;
        m_kia_db->get_data_from_db(begin.toString("hh:mm:ss.zzz"), end.toString("hh:mm:ss.zzz"));
        for (uint16_t ind = 0; ind < m_kia_plot_settings->m_kias_db->m_x_value.size(); ind++)
        {
            QTime timeStart = m_kia_plot_settings->m_kias_db->m_x_value[ind].toTime();
            double key = QTime(0, 0, 0).secsTo(timeStart);
            m_xv.push_back(key);
            m_yv.push_back(m_kia_plot_settings->m_kias_db->m_y_value[ind].toInt());
        }
        m_plot->graph(0)->addData(m_xv, m_yv);
        qDebug() << m_plot->graph(0)->dataCount() << m_xv.size() << m_yv.size();
        m_xv.clear();
        m_yv.clear();
        lastPointKey = key_1;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    //qDebug() << QTime(0, 0, 0).addMSecs((m_plot->xAxis->range().upper) * 1000.0);
    m_plot->replot();
    m_plot->update();
}
//void MainWindow::realtimeDataSlot()
//{
//    for (uint16_t ind = 0; ind < m_kia_plot_settings->m_kias_db->m_x_value.size(); ind++)
//    {
//        QTime timeStart = m_kia_plot_settings->m_kias_db->m_x_value[ind].toTime();
//        // calculate two new data points:
//        qDebug() << QTime(0, 0, 0).secsTo(timeStart);
//        double key =  QTime(0, 0, 0).secsTo(timeStart);// time elapsed since start of demo, in seconds
//        // add data to lines:
//        m_x = 1 + rand() % 3;
//        m_plot->graph(0)->addData(key, m_kia_plot_settings->m_kias_db->m_y_value[ind].toInt());
//        // make key axis range scroll with the data (at a constant range size of 8):
//        m_plot->xAxis->setRange(key, 8, Qt::AlignRight);
//        m_plot->replot();
//    }
//}
