#ifndef DWFCONSTANTS_H
#define DWFCONSTANTS_H

#include <cstdint>

namespace DWF {

// HDWF
constexpr int hdwfNone = 0;

// ENUMFILTER
enum EnumFilter : uint8_t {
    enumfilterAll = 0,
    enumfilterEExplorer = 1,
    enumfilterDiscovery = 2
};

// DEVID
enum DevId : uint8_t {
    devidEExplorer = 1,
    devidDiscovery = 2
};

// DEVVER
enum DevVer : uint8_t {
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
    trigsrcDetectorDigitalIn = 3,
    trigsrcAnalogIn = 4,
    trigsrcDigitalIn = 5,
    trigsrcDigitalOut = 6,
    trigsrcAnalogOut1 = 7,
    trigsrcAnalogOut2 = 8,
    trigsrcAnalogOut3 = 9,
    trigsrcAnalogOut4 = 10,
    trigsrcExternal1 = 11,
    trigsrcExternal2 = 12,
    trigsrcExternal3 = 13,
    trigsrcExternal4 = 14
};

// INSTRUMENT STATES
enum DwfState : uint8_t {
    DwfStateReady = 0,
    DwfStateConfig = 4,
    DwfStatePrefill = 5,
    DwfStateArmed = 1,
    DwfStateWait = 7,
    DwfStateTriggered = 3,
    DwfStateRunning = 3,
    DwfStateDone = 2
};

// STS
enum Sts : uint8_t {
    stsRdy = 0,
    stsArm = 1,
    stsDone = 2,
    stsTrig = 3,
    stsCfg = 4,
    stsPrefill = 5,
    stsNotDone = 6,
    stsTrigDly = 7,
    stsError = 8,
    stsBusy = 9,
    stsStop = 10
};

// ACQMODE
enum AcqMode : uint8_t {
    acqmodeSingle = 0,
    acqmodeScanShift = 1,
    acqmodeScanScreen = 2,
    acqmodeRecord = 3
};

// FILTER
enum Filter : uint8_t {
    filterDecimate = 0,
    filterAverage = 1,
    filterMinMax = 2
};

// TRIGTYPE
enum TrigType : uint8_t {
    trigtypeEdge = 0,
    trigtypePulse = 1,
    trigtypeTransition = 2
};

// TRIGCOND
enum TrigCond : uint8_t {
    trigcondRisingPositive = 0,
    trigcondFallingNegative = 1
};

// TRIGLEN
enum TrigLen : uint8_t {
    triglenLess = 0,
    triglenTimeout = 1,
    triglenMore = 2
};

// DWFERC
enum Dwferc : uint8_t {
    dwfercNoErc = 0,
    dwfercUnknownError = 1,
    dwfercApiLockTimeout = 2,
    dwfercAlreadyOpened = 3,
    dwfercNotSupported = 4,
    dwfercInvalidParameter0 = 16,
    dwfercInvalidParameter1 = 17,
    dwfercInvalidParameter2 = 18,
    dwfercInvalidParameter3 = 19
};

// FUNC
enum Func : uint8_t {
    funcDC = 0,
    funcSine = 1,
    funcSquare = 2,
    funcTriangle = 3,
    funcRampUp = 4,
    funcRampDown = 5,
    funcNoise = 6,
    funcCustom = 30,
    funcPlay = 31
};

// ANALOGIO
enum AnalogIO : uint8_t {
    analogioEnable = 1,
    analogioVoltage = 2,
    analogioCurrent = 3,
    analogioPower = 4,
    analogioTemperature = 5
};

// AnalogOutNode
enum AnalogOutNode : uint8_t {
    AnalogOutNodeCarrier = 0,
    AnalogOutNodeFM = 1,
    AnalogOutNodeAM = 2
};

// DIGITALIN
enum DigitalInClockSource : uint8_t {
    DwfDigitalInClockSourceInternal = 0,
    DwfDigitalInClockSourceExternal = 1
};

enum DigitalInSampleMode : uint8_t {
    DwfDigitalInSampleModeSimple = 0,
    DwfDigitalInSampleModeNoise = 1
};

// DIGITALOUT
enum DigitalOutOutput : uint8_t {
    DwfDigitalOutOutputPushPull = 0,
    DwfDigitalOutOutputOpenDrain = 1,
    DwfDigitalOutOutputOpenSource = 2,
    DwfDigitalOutOutputThreeState = 3
};

enum DigitalOutType : uint8_t {
    DwfDigitalOutTypePulse = 0,
    DwfDigitalOutTypeCustom = 1,
    DwfDigitalOutTypeRandom = 2
};

enum DigitalOutIdle : uint8_t {
    DwfDigitalOutIdleInit = 0,
    DwfDigitalOutIdleLow = 1,
    DwfDigitalOutIdleHigh = 2,
    DwfDigitalOutIdleZet = 3
};

} // namespace DWF

#endif // DWFCONSTANTS_H
