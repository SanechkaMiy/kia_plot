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
    m_kia_graph.push_back(std::make_shared<Kia_graph>(m_kia_db, m_kia_plot_settings, this));
    //m_kia_graph.push_back(std::make_shared<Kia_graph>(m_kia_db, m_kia_plot_settings, this));
    m_kia_graph[0]->show();
    //m_kia_graph[1]->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


