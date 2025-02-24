#ifndef DWFCONSTANTS_H
#define DWFCONSTANTS_H

#include <cstdint>

// HDWF
#define hdwfNone;

    // ENUMFILTER
    enum EnumFilter { enumfilterAll = 0, enumfilterEExplorer = 1, enumfilterDiscovery = 2 };

// DEVID
enum DevId { devidEExplorer = 1, devidDiscovery = 2 };

// DEVVER
enum DevVer {
    devverEExplorerC = 2,
    devverEExplorerE = 4,
    devverEExplorerF = 5,
    devverDiscoveryA = 1,
    devverDiscoveryB = 2,
    devverDiscoveryC = 3
};

// TRIGSRC
enum TrigSrc : uint8_t {
    trigsrcNone = 0,
    trigsrcPC = 1,
    trigsrcDetectorAnalogIn = 2,
    trigsrcDetectorDigitalIn = 3
};

// FUNC
enum FuncType : uint8_t {
    funcDC = 0,
    funcSine = 1,
    funcSquare = 2
};

#endif // DWFCONSTANTS_H
