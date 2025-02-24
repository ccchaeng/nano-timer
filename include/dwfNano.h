#ifndef DWFNANO_H
#define DWFNANO_H

#include <vector>
#include <complex>
#include <thread>
#include <chrono>
#include <iostream>
#include "dwfconstants.h"
#include "dwf.h"

class dwfImpedance {
public:
    dwfImpedance();
    ~dwfImpedance();

    void portSel(int channel);
    std::pair<std::vector<double>, std::vector<double>> getScopeData(double frequency);
    std::complex<double> calcImpedance(const std::vector<double>& data0, const std::vector<double>& data1);

private:
    void _dwf_init();
    void _set_power_supply(double voltage);
    void _set_function_generator(double amplitude, double offset);
    void _set_analog_input(int T, int nT);
    void _set_digital_output(int mask);

    int hdwf;
    int buf_len;
    int T, nT;
    std::vector<double> rg0Samples;
    std::vector<double> rg1Samples;
};

#endif // DWFNANO_H
