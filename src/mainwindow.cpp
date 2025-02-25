// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon(":/image/images/icon.png");
    setWindowIcon(icon);

    QString lineEdit_title_style = "QLineEdit {"
                                   "border-radius: 5px;"
                                   "color: blue;"
                                   "font-weight: bold;"
                                   "background: transparent;"
                                   "}";

    QString lineEdit_input_style = "QLineEdit {"
                                   "border-radius: 5px;"
                                   "border: 2px solid #007BFF;"
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

    // QFrame 안에 두 개의 QCustomPlot 생성
    QVBoxLayout *layout = new QVBoxLayout(ui->GraphFrame);

    customPlotSignal = new QCustomPlot(this);
    customPlotSignal->addGraph();
    customPlotSignal->xAxis->setLabel("Time (s)");
    customPlotSignal->yAxis->setLabel("Voltage (V)");
    customPlotSignal->yAxis->setRange(-1, 1);
    layout->addWidget(customPlotSignal);

    customPlotImpedance = new QCustomPlot(this);
    customPlotImpedance->addGraph();
    customPlotImpedance->xAxis->setLabel("Time (s)");
    customPlotImpedance->yAxis->setLabel("Impedance (Ohm)");
    customPlotImpedance->yAxis->setRange(0, 0.05);  // 임피던스 기본 범위 설정
    layout->addWidget(customPlotImpedance);

    ui->GraphFrame->setLayout(layout);

    initAnalogDiscovery();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGraph);
    connect(ui->StartButton, &QPushButton::clicked, this, &MainWindow::startGraphUpdate);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (hdwf != hdwfNone) {
        FDwfDeviceClose(hdwf);
    }
}

void MainWindow::initAnalogDiscovery()
{
    if (FDwfDeviceOpen(-1, &hdwf) == 0) {
        qDebug() << "Analog Discovery 2 연결 실패!";
        return;
    }
    qDebug() << "Analog Discovery 2 연결 성공!";

    FDwfAnalogInChannelEnableSet(hdwf, 0, true);
    FDwfAnalogInChannelEnableSet(hdwf, 1, true);
    FDwfAnalogInBufferSizeSet(hdwf, sampleCount);
    FDwfAnalogInConfigure(hdwf, false, true);
}

QVector<double> MainWindow::getScopeData(int channel)
{
    QVector<double> data(sampleCount);
    if (FDwfAnalogInStatus(hdwf, true, nullptr) == 0) {
        qDebug() << "AD2 데이터 가져오기 실패!";
        return data;
    }
    FDwfAnalogInStatusData(hdwf, channel, data.data(), sampleCount);
    return data;
}


QVector<double> MainWindow::calculateImpedance(const QVector<double>& vRef, const QVector<double>& vUnknown, double refResistance)
{
    QVector<double> impedance(vRef.size());
    for (int i = 0; i < vRef.size(); ++i) {
        double current = (fabs(vRef[i]) < 1e-6) ? 1e-6 : vRef[i] / refResistance;  // 전류 계산 (0 분모 방지)
        impedance[i] = (fabs(current) < 1e-9) ? 0 : vUnknown[i] / current;        // 임피던스 계산
    }
    return impedance;
}


void MainWindow::updateGraph()
{
    QVector<double> vRefData = getScopeData(0);       // 채널 0 (Reference)
    QVector<double> vUnknownData = getScopeData(1);   // 채널 1 (Unknown)

    if (vRefData.isEmpty() || vUnknownData.isEmpty()) return;

    double refResistance = ui->input_Ref->text().toDouble() * 1000;  // kOhm → Ohm 변환
    QVector<double> impedanceData = calculateImpedance(vRefData, vUnknownData, refResistance);

    QVector<double> time(sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        time[i] = i * 0.001;
    }

    customPlotSignal->graph(0)->setData(time, vUnknownData);
    customPlotSignal->xAxis->setRange(0, time.last());
    customPlotSignal->replot();

    customPlotImpedance->graph(0)->setData(time, impedanceData);
    customPlotImpedance->xAxis->setRange(0, time.last());
    customPlotImpedance->yAxis->rescale();
    customPlotImpedance->replot();
}


void MainWindow::startGraphUpdate()
{
    qDebug() << "Start 버튼 클릭됨 - 그래프 업데이트 시작";
    if (!timer->isActive()) {
        timer->start(100);  // 100ms마다 그래프 업데이트
    }
}
