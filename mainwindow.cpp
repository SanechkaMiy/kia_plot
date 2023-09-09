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
    m_plot->addGraph();
    m_plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    m_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->gl_plots->addWidget(m_plot);
    start_get_data();
}

MainWindow::~MainWindow()
{
    stop_get_data();
    m_kia_plot_settings->m_kia_threads->m_get_data_stop =  false;
    m_kia_plot_settings->m_kia_threads->m_th_get_data.get();
    delete ui;
}

void MainWindow::start_get_data()
{
    m_kia_plot_settings->m_kia_threads->m_th_get_data = std::async([this]()
    {
        m_kia_plot_settings->m_kia_threads->m_get_data_stop = true;
        while(m_kia_plot_settings->m_kia_threads->m_get_data_stop)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            m_x = m_x + 2;
            m_y = m_y + 1;
            m_xv.append(m_x);
            m_yv.append(m_y);
            m_plot->graph(0)->setData(m_xv, m_yv);
            m_plot->replot();
            qDebug() << m_plot->xAxis->range().upper;
            m_plot->update();
        }
    });
}

void MainWindow::stop_get_data()
{

}
