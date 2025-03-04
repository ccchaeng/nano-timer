#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include <complex>
#include "fitSine.h"
#include <QDir>
#include <QCoreApplication>

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

// **임피던스 계산 함수 (수정 버전)**
std::complex<double> MainWindow::calcImpedance(QVector<double> ch1, QVector<double> ch2, double refResistance) {
    if (ch1.isEmpty() || ch2.isEmpty()) {
        qDebug() << "채널 데이터가 비어 있습니다!";
        return std::complex<double>(0.0, 0.0);
    }

    // sineFit2Cycle() 적용
    std::vector<double> ch1Vec(ch1.begin(), ch1.end());
    std::vector<double> ch2Vec(ch2.begin(), ch2.end());

    auto [R0, T0, M0] = sineFit2Cycle(ch1Vec, 2);
    auto [R1, T1, M1] = sineFit2Cycle(ch2Vec, 2);

    // 진폭 값이 너무 작으면 보정
    if (R0 < 1e-3) R0 = 1e-3;
    if (R1 < 1e-3) R1 = 1e-3;

    double g = fabs(R1 / R0); // 음수 방지
    double p = T1 - T0;

    // 위상 차이 보정 (-π ~ π 범위로 맞춤)
    if (p > M_PI) {
        p -= 2 * M_PI;
    } else if (p < -M_PI) {
        p += 2 * M_PI;
    }

    // 임피던스 계산
    std::complex<double> impedance = std::polar(g * refResistance, p);

    qDebug() << "계산된 임피던스 값: " << impedance.real() << " + j " << impedance.imag();

    return impedance;
}




void MainWindow::updateGraph() {
    QVector<double> ch1Data = getScopeData(0);
    QVector<double> ch2Data = getScopeData(1);

    if (ch1Data.isEmpty() || ch2Data.isEmpty()) {
        qDebug() << "데이터 없음! 임피던스 계산 스킵";
        return;
    }

    QVector<double> time(sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        time[i] = elapsedTime + (i * 0.001);
    }

    double refResistance = ui->input_Ref->text().toDouble();
    if (refResistance > 10) {
        refResistance /= 1000.0;
    }

    std::complex<double> impedance = calcImpedance(ch1Data, ch2Data, refResistance);
    qDebug() << "계산된 임피던스 값: " << impedance.real() << " + j" << impedance.imag();

    // ✅ Signal 그래프 업데이트
    Signal_Plot->graph(0)->setData(time, ch1Data);
    Signal_Plot->graph(1)->setData(time, ch2Data);
    Signal_Plot->xAxis->setRange(time.first(), time.last());
    Signal_Plot->yAxis->setRange(-0.5, 0.5);
    Signal_Plot->replot();

    // ✅ Impedance 그래프 업데이트
    impedanceValues.append(impedance.real());
    timeValues.append(time.last());

    Impedance_Plot->graph(0)->setData(timeValues, impedanceValues);
    Impedance_Plot->xAxis->setRange(timeValues.first(), timeValues.last());
    Impedance_Plot->graph(0)->rescaleAxes();

    Impedance_Plot->replot();

    elapsedTime += 0.1;
    ui->Elapsed_Time->setText(QString::number(elapsedTime, 'f', 1));

    // CSV 파일에 데이터 저장 (data 폴더 내부)
    // 100ms 단위로 데이터 저장
    if (static_cast<int>(elapsedTime * 10) % 1 == 0) {  // 100ms마다 저장
        csvStream << elapsedTime << "," << impedance.real() << "," << impedance.imag() << "," << std::abs(impedance) << "\n";
    }

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

    QString saveDir = QCoreApplication::applicationDirPath() + "/../../data/";
    QDir().mkpath(saveDir);  // ✅ 디렉터리 없으면 생성

    QString filename = saveDir + "/" + ui->input_Filename->text() + ".csv";
    csvFile.setFileName(filename);

    qDebug() << "CSV 파일 저장 경로: " << csvFile.fileName();

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "CSV 파일 저장 실패! 파일 경로:" << filename;
        return;
    }

    csvStream.setDevice(&csvFile);
    csvStream << "Time,real,imag,abs\n";

     qDebug() << "✅ CSV 파일 저장 성공! 파일 경로:" << filename;

    // ✅ **그래프 데이터 초기화**
    impedanceValues.clear();
    timeValues.clear();
    Signal_Plot->graph(0)->data()->clear();
    Signal_Plot->graph(1)->data()->clear();
    Impedance_Plot->graph(0)->data()->clear();

    // ✅ **그래프 강제 초기화**
    Signal_Plot->replot();
    Impedance_Plot->replot();

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

    // csv 파일 닫기
    if (csvFile.isOpen()) {
        csvFile.close();
        qDebug() << "CSV 저장 완료! 파일 위치: " << csvFile.fileName();
    }


    // **버튼 색상 및 텍스트 복원**
    ui->StartButton->setText("Start");
    ui->StartButton->setStyleSheet("");
}
