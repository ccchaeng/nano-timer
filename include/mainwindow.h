#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "qcustomplot.h"
#include "dwf.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private slots:
  void updateGraph(); //그래프 갱신 함수
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
  QTimer *timer;  // 타이머 (주기적으로 데이터를 가져옴)

  int totalDuration;
  QFile csvFile;
  QTextStream csvStream;

  HDWF hdwf;  // Analog Discovery 2 핸들
  int sampleCount = 1024;  // 샘플 개수

  void initAnalogDiscovery();  // AD2 초기화 함수
  QVector<double> getScopeData(int channel);  // 채널의 데이터 읽기
  double calcImpedance(QVector<double> ch1, QVector<double> ch2);

};
#endif  // MAINWINDOW_H
