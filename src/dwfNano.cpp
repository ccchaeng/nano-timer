#include "dwfNano.h"
#include "fitSine.h"  // sineFit2Cycle 함수 구현 필요
#include <cmath>

dwfImpedance::dwfImpedance() {
    _dwf_init();
    rg0Samples.resize(buf_len);
    rg1Samples.resize(buf_len);
}

dwfImpedance::~dwfImpedance() {}

void dwfImpedance::_dwf_init() {
    if (!FDwfDeviceOpen(-1, &hdwf) || hdwf == hdwfNone) {
        char szerr[512];
        FDwfGetLastErrorMsg(szerr);
        std::cerr << "Error: " << szerr << std::endl;
        exit(EXIT_FAILURE);
    }
    _set_power_supply(5);
    _set_function_generator(1, 0);
    _set_analog_input(1024, 2);
    _set_digital_output(0x001F);
}

void dwfImpedance::_set_power_supply(double voltage) {
    FDwfAnalogIOChannelNodeSet(hdwf, 0, 0, 1);
    FDwfAnalogIOChannelNodeSet(hdwf, 0, 1, voltage);
    FDwfAnalogIOChannelNodeSet(hdwf, 1, 0, 1);
    FDwfAnalogIOChannelNodeSet(hdwf, 1, 1, -voltage);
    FDwfAnalogIOEnableSet(hdwf, 1);
}

void dwfImpedance::_set_function_generator(double amplitude, double offset) {
    FDwfAnalogOutNodeEnableSet(hdwf, 0, AnalogOutNodeCarrier, 1);
    FDwfAnalogOutNodeFunctionSet(hdwf, 0, AnalogOutNodeCarrier, funcSine);
    FDwfAnalogOutNodeAmplitudeSet(hdwf, 0, AnalogOutNodeCarrier, amplitude);
    FDwfAnalogOutNodeOffsetSet(hdwf, 0, AnalogOutNodeCarrier, offset);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void dwfImpedance::_set_analog_input(int T, int nT) {
    this->T = T;
    this->nT = nT;
    buf_len = T * nT;
    FDwfAnalogInBufferSizeSet(hdwf, 2 * buf_len);
    FDwfAnalogInChannelEnableSet(hdwf, 0, true);
    FDwfAnalogInChannelRangeSet(hdwf, 0, 0.5);
    FDwfAnalogInChannelEnableSet(hdwf, 1, true);
    FDwfAnalogInChannelRangeSet(hdwf, 1, 0.5);
}

void dwfImpedance::_set_digital_output(int mask) {
    FDwfDigitalIOOutputEnableSet(hdwf, mask);
}

void dwfImpedance::portSel(int channel) {
    FDwfDigitalIOOutputSet(hdwf, channel * 2 + 1);
}

std::pair<std::vector<double>, std::vector<double>> dwfImpedance::getScopeData(double frequency) {
    FDwfAnalogOutNodeFrequencySet(hdwf, 0, AnalogOutNodeCarrier, frequency);
    FDwfAnalogOutConfigure(hdwf, 0, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    FDwfAnalogInConfigure(hdwf, false, true);
    DwfState sts;  // char 대신 DwfState 사용
    do {
        FDwfAnalogInStatus(hdwf, true, &sts);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (sts != DwfStateDone);

    return {rg0Samples, rg1Samples};
}

std::complex<double> dwfImpedance::calcImpedance(const std::vector<double>& data0, const std::vector<double>& data1) {
    auto [R0, T0, M0] = sineFit2Cycle(data0, nT);
    auto [R1, T1, M1] = sineFit2Cycle(data1, nT);

    if (R0 < 0) { R0 = -R0; T0 -= M_PI; }
    if (R1 < 0) { R1 = -R1; T1 -= M_PI; }

    double gain = R1 / R0;
    double phase = T1 - T0;
    if (phase > M_PI) phase -= 2 * M_PI;

    return std::polar(gain, phase);
}
