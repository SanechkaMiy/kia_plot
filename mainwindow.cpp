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
    m_kia_graph_manager.reset(new Kia_graph_manager(m_kia_db, m_kia_plot_settings, this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
