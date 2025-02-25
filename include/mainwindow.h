#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QTimer>
#include "qcustomplot.h"
#include "dwf.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void startGraphUpdate(); // start 버튼 클릭 시 그래프 업데이트
    void updateGraph(); //그래프 갱신 함수

private:
    Ui::MainWindow* ui;
    QCustomPlot *customPlotSignal;      // Signal 그래프용 QCustomPlot
    QCustomPlot *customPlotImpedance;   // Impedance 그래프용 QCustomPlot
    QTimer *timer;  // 타이머 (주기적으로 데이터를 가져옴)

    HDWF hdwf;  // Analog Discovery 2 핸들
    QVector<double> dataX, dataY;  // 그래프 데이터 저장용
    QVector<double> calculateImpedance(const QVector<double>& vRef, const QVector<double>& vUnknown, double refResistance);
    int sampleCount = 1024;  // 샘플 개수

    void initAnalogDiscovery();  // AD2 초기화 함수
    QVector<double> getScopeData(int channel);  // AD2에서 데이터 가져오기
};
#endif  // MAINWINDOW_H
