#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
<<<<<<< HEAD
#include <QTimer>
#include "qcustomplot.h"
#include "dwf.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
=======

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
>>>>>>> 389d2f9 (first commit)
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

<<<<<<< HEAD
private slots:
  void updateGraph(); //그래프 갱신 함수

private:
  Ui::MainWindow* ui;
  QCustomPlot *customPlot; //그래프 위젯
  QTimer *timer;  // 타이머 (주기적으로 데이터를 가져옴)

  HDWF hdwf;  // Analog Discovery 2 핸들
  QVector<double> dataX, dataY;  // 그래프 데이터 저장용
  int sampleCount = 1024;  // 샘플 개수

  void initAnalogDiscovery();  // AD2 초기화 함수
  QVector<double> getScopeData();  // AD2에서 데이터 가져오기
=======
private:
  Ui::MainWindow* ui;
>>>>>>> 389d2f9 (first commit)
};
#endif  // MAINWINDOW_H
