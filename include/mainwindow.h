#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "qcustomplot.h"
#include "dwf.h"
#include <complex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    // ✅ 중복된 calcImpedance 선언 제거
    std::complex<double> calcImpedance(QVector<double> ch1, QVector<double> ch2, double refResistance);

private slots:
    void updateGraph();
    void startMeasurement();
    void stopMeasurement();
    void toggleMeasurement();

private:
    Ui::MainWindow* ui;
    QCustomPlot *Signal_Plot;
    QCustomPlot *Impedance_Plot;

    QVector<double> impedanceValues;
    QVector<double> timeValues;
    double elapsedTime = 0.0;
    QTimer *timer;

    int totalDuration;
    QFile csvFile;
    QTextStream csvStream;

    HDWF hdwf;
    int sampleCount = 1024;

    void initAnalogDiscovery();
    QVector<double> getScopeData(int channel);
};

#endif  // MAINWINDOW_H
