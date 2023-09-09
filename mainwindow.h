#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "kia_struct.h"
#include <QMainWindow>
#include <qcustomplot.h>
#include <QLayout>
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
    double m_x = 0;
    double m_y = 0;
    void start_get_data();
    void stop_get_data();
    std::shared_ptr<Kia_plot_settings> m_kia_plot_settings;
    Ui::MainWindow *ui;
    QCustomPlot* m_plot;
    QVector<double> m_xv, m_yv;
    double key = 0;
    QTimer dataTimer;
};

#endif // MAINWINDOW_H
