#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <iostream>

// WaveForms SDK 헤더
#include "dwf.h"

// 전역 변수
HDWF hdwf;  // 장치 핸들

// 함수 프로토타입 선언
void DwfInit();

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(1050, 700);

    w.show();
    return a.exec();
}
