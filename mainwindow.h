#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "kia_struct.h"
#include <iostream>
#include <QMainWindow>
#include <qcustomplot.h>

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
private slots:
    void realtimeDataSlot();
private:
    Ui::MainWindow *ui;
    std::shared_ptr<Kia_plot_settings> m_kia_plot_settings;
    std::shared_ptr<Kia_db> m_kia_db;
    double m_x = 0;
    double m_y = 0;
    void start_get_data();
    void stop_get_data();
    QCustomPlot* m_plot;
    QVector<double> m_xv, m_yv;
    double key = 0;
    QTimer dataTimer;
};

#endif // MAINWINDOW_H
