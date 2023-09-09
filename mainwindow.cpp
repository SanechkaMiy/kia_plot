#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_kia_plot_settings(new Kia_plot_settings())
{
    ui->setupUi(this);
    m_plot = new QCustomPlot(this);
    m_plot->setInteraction(QCP::iRangeDrag, true);
    m_plot->setInteraction(QCP::iRangeZoom, true);
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
    //connect(m_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->xAxis2, SLOT(setRange(QCPRange)));
    //connect(m_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->yAxis2, SLOT(setRange(QCPRange)));
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
    double key = timeStart.msecsTo(QTime::currentTime()) / 1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key - lastPointKey > 1) // at most add point every 2 ms
    {
        // add data to lines:
        m_x = 1 + rand() % 3;
        m_plot->graph(0)->addData(key, m_x);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    m_plot->xAxis->setRange(key, 8, Qt::AlignRight);
    m_plot->replot();
}
