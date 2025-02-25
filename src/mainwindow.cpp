/*
 * Updated mainwindow.cpp to trigger graph updates when the Start button is clicked.
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

    customPlot = new QCustomPlot(this);
    QVBoxLayout *layout = new QVBoxLayout(ui->GraphFrame);
    layout->addWidget(customPlot);
    ui->GraphFrame->setLayout(layout);

    customPlot->addGraph();
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Voltage");

    initAnalogDiscovery();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGraph);

    // Start 버튼 클릭 시 그래프 업데이트 시작
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

void MainWindow::updateGraph()
{
    QVector<double> data = getScopeData();
    if (data.isEmpty()) return;

    QVector<double> time(sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        time[i] = i * 0.001;
    }

    customPlot->graph(0)->setData(time, data);
    customPlot->xAxis->setRange(0, time.last());
    customPlot->yAxis->setRange(-1, 1);
    customPlot->replot();
}

void MainWindow::startGraphUpdate()
{
    qDebug() << "Start 버튼 클릭됨 - 그래프 업데이트 시작";
    if (!timer->isActive()) {
        timer->start(100);  // 100ms마다 그래프 업데이트
    }
}
