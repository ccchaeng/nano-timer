#include "dwfThread.h"
#include <iostream>

dwfThread::dwfThread(MainWindow* main_ui, double period, int loop, double freq)
    : main_ui(main_ui), period(period), loop(loop), freq(freq), cur_loop(0), stop_flag(false) {
    dwfim = main_ui->getDwfImpedance();
}

dwfThread::~dwfThread() {
    stop();
    if (worker.joinable()) worker.join();
}

void dwfThread::start() {
    worker = std::thread(&dwfThread::run, this);
}

void dwfThread::stop() {
    stop_flag = true;
}

void dwfThread::run() {
    auto start_time = std::chrono::steady_clock::now();

    while (cur_loop < loop) {
        if (stop_flag) {
            std::cout << "Thread stopped" << std::endl;
            break;
        }

        auto cur_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = cur_time - start_time;

        main_ui->updateElapsedTime(elapsed.count());

        if (elapsed.count() >= period) {
            start_time = cur_time;
            measureImpedance(elapsed.count());
            cur_loop++;
        }
    }
}

void dwfThread::measureImpedance(double elapsed_time) {
    auto [data0, data1] = dwfim->getScopeData(freq);
    std::complex<double> zo = dwfim->calcImpedance(data0, data1);
    std::complex<double> z = -main_ui->getRef() * zo;

    main_ui->updateMeasurements(data0, data1, z, cur_loop, elapsed_time);
}
