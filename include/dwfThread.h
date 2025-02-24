#ifndef DWFTHREAD_H
#define DWFTHREAD_H

#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <complex>
#include "dwfNano.h"
#include "mainwindow.h"

class dwfThread {
public:
    dwfThread(MainWindow* main_ui, double period, int loop, double freq);
    ~dwfThread();

    void start();
    void stop();

private:
    void run();
    void measureImpedance(double elapsed_time);

    MainWindow* main_ui;
    dwfImpedance* dwfim;
    double period;
    int loop;
    double freq;
    int cur_loop;
    std::atomic<bool> stop_flag;
    std::thread worker;
};

#endif // DWFTHREAD_H
