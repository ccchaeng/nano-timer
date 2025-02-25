#include "./ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon(":/image/images/icon.png");
    setWindowIcon(icon);

    // UI 스타일 설정
    QString lineEdit_title_style = "QLineEdit {"
                                   "border-radius: 5px;"
                                   "color: blue;"
                                   "font-weight: bold;"
                                   "background: transparent;"
                                   "}";

    QString lineEdit_input_style = "QLineEdit {"
                                   "border-radius: 5px;"
                                   "border: 2px solid #007BFF;"
                                   "background: white;"
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

    // **Signal Graph (Ch1 & Ch2)**
    Signal_Plot = new QCustomPlot(this);
    QVBoxLayout *layout1 = new QVBoxLayout(ui->GraphFrame_1);
    layout1->addWidget(Signal_Plot);
    ui->GraphFrame_1->setLayout(layout1);

    Signal_Plot->addGraph(); // Ch1
    Signal_Plot->addGraph(); // Ch2

    Signal_Plot->xAxis->setLabel("Sample");
    Signal_Plot->yAxis->setLabel("Amplitude");
    Signal_Plot->graph(0)->setPen(QPen(Qt::blue));    // Ch1 - 파란색
    Signal_Plot->graph(1)->setPen(QPen(QColorConstants::Svg::darkorange)); // Ch2 - 주황색
    Signal_Plot->legend->setVisible(true);
    Signal_Plot->graph(0)->setName("Ch1");
    Signal_Plot->graph(1)->setName("Ch2");

    // **Impedance Graph**
    Impedance_Plot = new QCustomPlot(this);
    QVBoxLayout *layout2 = new QVBoxLayout(ui->GraphFrame_2);
    layout2->addWidget(Impedance_Plot);
    ui->GraphFrame_2->setLayout(layout2);

    Impedance_Plot->addGraph();
    Impedance_Plot->xAxis->setLabel("Time (sec)");
    Impedance_Plot->yAxis->setLabel("Impedance");
    Impedance_Plot->graph(0)->setPen(QPen(Qt::blue));
    Impedance_Plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    Impedance_Plot->graph(0)->setLineStyle(QCPGraph::lsLine);

    // AD2 초기화
    initAnalogDiscovery();

    // **타이머 설정 (100ms마다 업데이트)**
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGraph);

    // **Start 버튼 클릭 시 측정 시작**
    connect(ui->StartButton, &QPushButton::clicked, this, &MainWindow::toggleMeasurement);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (hdwf != hdwfNone) {
        FDwfDeviceClose(hdwf);  // 장치 닫기
    }
}

// **Analog Discovery 2 초기화**
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

// **AD2에서 데이터 가져오기**
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

// **임피던스 계산 함수**
double MainWindow::calcImpedance(QVector<double> ch1, QVector<double> ch2)
{
    double sumCh1 = 0, sumCh2 = 0;
    for (int i = 0; i < ch1.size(); i++) {
        sumCh1 += ch1[i] * ch1[i];
        sumCh2 += ch2[i] * ch2[i];
    }

    double rmsCh1 = sqrt(sumCh1 / ch1.size());
    double rmsCh2 = sqrt(sumCh2 / ch2.size());

    return (rmsCh2 / rmsCh1) * 1000.0; // 예제 공식 (조절 가능)
}

// **그래프 갱신 함수**
void MainWindow::updateGraph()
{
    QVector<double> ch1Data = getScopeData(0);
    QVector<double> ch2Data = getScopeData(1);

    if (ch1Data.isEmpty() || ch2Data.isEmpty()) return;

    QVector<double> time(sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        time[i] = i;
    }

    // **Signal plot 업데이트**
    Signal_Plot->graph(0)->setData(time, ch1Data);
    Signal_Plot->graph(1)->setData(time, ch2Data);
    Signal_Plot->xAxis->setRange(0, sampleCount);
    Signal_Plot->yAxis->setRange(-0.5, 0.5);
    Signal_Plot->replot();

    // **Impedance plot 업데이트**
    double impedance = calcImpedance(ch1Data, ch2Data);
    impedanceValues.append(impedance);
    timeValues.append(elapsedTime);

    Impedance_Plot->graph(0)->setData(timeValues, impedanceValues);
    Impedance_Plot->xAxis->setRange(0, elapsedTime);
    Impedance_Plot->yAxis->setRange(0, 150);
    Impedance_Plot->replot();

    // **Elapsed Time UI 업데이트**
    elapsedTime += 0.1;
    ui->Elapsed_Time->setText(QString::number(elapsedTime, 'f', 1));
}

// **Start/Stop 버튼 동작**
void MainWindow::toggleMeasurement()
{
    if (timer->isActive()) {
        stopMeasurement();
    } else {
        startMeasurement();
    }
}

// **측정 시작**
void MainWindow::startMeasurement()
{
    elapsedTime = 0.0;
    totalDuration = ui->input_Duration->text().toInt();

    QString filename = ui->input_Filename->text() + ".csv";
    csvFile.setFileName(filename);

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "CSV 파일 열기 실패!";
        return;
    }
    csvStream.setDevice(&csvFile);
    csvStream << "Time,Signal,Impedance\n";

    timer->start(100);
    QTimer::singleShot(totalDuration * 1000, this, &MainWindow::stopMeasurement);

    // **버튼 색상 및 텍스트 변경**
    ui->StartButton->setText("Stop");
    ui->StartButton->setStyleSheet("background-color: red; color: white;");
}

// **측정 종료**
void MainWindow::stopMeasurement()
{
    timer->stop();
    csvFile.close();
    qDebug() << "측정 종료!";

    // **버튼 색상 및 텍스트 복원**
    ui->StartButton->setText("Start");
    ui->StartButton->setStyleSheet("");
}
