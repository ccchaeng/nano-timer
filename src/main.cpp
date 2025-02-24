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
<<<<<<< HEAD
    w.setFixedSize(1050, 700);

    w.show();
    return a.exec();
}
=======
    w.setFixedSize(1000, 730);

    // Analog Discovery 2 장치 초기화
    DwfInit();
    w.show();

    return a.exec();
}

// 장치 초기화 함수 정의
void DwfInit() {
    char msg[512] = {0};

    if (!FDwfDeviceOpen(-1, &hdwf)) {
        // 장치 연결 실패 시 에러 메시지 출력
        FDwfGetLastErrorMsg(msg);
        QMessageBox::critical(nullptr, "Analog Discovery 2 연결 실패",
                              QString("장치 연결에 실패했습니다:\n%1").arg(msg));
        std::cerr << "Analog Discovery 2 연결 실패: " << msg << std::endl;
        exit(1);
    } else {
        // 장치 연결 성공 시 메시지 출력
        std::cout << "Analog Discovery 2 장치가 성공적으로 연결되었습니다!" << std::endl;
        QMessageBox::information(nullptr, "Analog Discovery 2 연결 성공",
                                 "Analog Discovery 2 장치가 성공적으로 연결되었습니다!");
    }
}
>>>>>>> 389d2f9 (first commit)
