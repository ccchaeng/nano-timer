#include "./ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QIcon icon(":/image/images/icon.png");
  setWindowIcon(icon);

  // UI 설정 부분
  QString lineEdit_title_style = "QLineEdit {"
                                 "border-radius: 5px;"
                                 "color: blue;"
                                 "font-weight: bold;"
                                 "background: transparent;"
                                 "}";

  QString lineEdit_input_style = "QLineEdit {"
                                 "border-radius: 5px;"
                                 "border: 2px soliod #007BFF"
                                 "}";


  ui->title_Freq->setStyleSheet(lineEdit_title_style);
  ui->title_Ref->setStyleSheet(lineEdit_title_style);
  ui->title_Duration->setStyleSheet(lineEdit_title_style);
  ui->title_Filename->setStyleSheet(lineEdit_title_style);

  ui->input_Freq->setStyleSheet(lineEdit_input_style);
  ui->input_Ref->setStyleSheet(lineEdit_input_style);
  ui->input_Duration->setStyleSheet(lineEdit_input_style);
  ui->input_Filename->setStyleSheet(lineEdit_input_style);
  ui->Elapsed_Time->setStyleSheet(lineEdit_input_style);

  // QCustomPlot 그래프 설정
  customPlot = new QCustomPlot(this);
  QVBoxLayout *layout = new QVBoxLayout(ui->GraphFrame);
  layout->addWidget(customPlot);
  ui->GraphFrame->setLayout(layout);

  customPlot->addGraph();  // 그래프 추가
  customPlot->xAxis->setLabel("Time");
  customPlot->yAxis->setLabel("Voltage");

  // Analog Discovery 2 초기화
  initAnalogDiscovery();

  // QTimer 설정 (100ms마다 데이터 갱신)
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateGraph);
  timer->start(100);
}

MainWindow::~MainWindow()
{
  delete ui;
  if (hdwf != hdwfNone) {
    FDwfDeviceClose(hdwf);  // 장치 닫기
  }
}

// Analog Discovery 2 초기화 함수
void MainWindow::initAnalogDiscovery()
{
    if (FDwfDeviceOpen(-1, &hdwf) == 0) {
        qDebug() << "Analog Discovery 2 연결 실패!";
        return;
    }
    qDebug() << "Analog Discovery 2 연결 성공!";

    // 아날로그 입력 설정 (채널 0, 1 활성화)
    FDwfAnalogInChannelEnableSet(hdwf, 0, true);
    FDwfAnalogInChannelEnableSet(hdwf, 1, true);
    FDwfAnalogInBufferSizeSet(hdwf, sampleCount);
    FDwfAnalogInConfigure(hdwf, false, true);
}

// AD2에서 데이터 가져오는 함수
QVector<double> MainWindow::getScopeData()
{
    QVector<double> data(sampleCount);
    if (FDwfAnalogInStatus(hdwf, true, nullptr) == 0) {
        qDebug() << "AD2 데이터 가져오기 실패!";
        return data;
    }
    FDwfAnalogInStatusData(hdwf, 0, data.data(), sampleCount);
    return data;
}

// 그래프 갱신 함수 (주기적으로 호출)
void MainWindow::updateGraph()
{
    QVector<double> data = getScopeData();
    if (data.isEmpty()) return;

    // X 축 데이터 생성 (시간)
    QVector<double> time(sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        time[i] = i * 0.001;  // 1kHz 샘플링 가정
    }

    // 그래프 업데이트
    customPlot->graph(0)->setData(time, data);
    customPlot->xAxis->setRange(0, time.last());
    customPlot->yAxis->setRange(-1, 1);  // ±1V 범위
    customPlot->replot();
}
