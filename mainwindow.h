#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "kia_struct.h"
#include <iostream>
#include <QMainWindow>
#include <qcustomplot.h>
#include "kia_graph.h"
#include "kia_db.h"
#include <QLayout>
#include <QTime>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    std::shared_ptr<Kia_plot_settings> m_kia_plot_settings;
    QVector<std::shared_ptr<Kia_graph>> m_kia_graph;
    std::shared_ptr<Kia_db> m_kia_db;
};

#endif // MAINWINDOW_H
