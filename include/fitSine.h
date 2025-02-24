#ifndef FITSINE_H
#define FITSINE_H

#include <vector>
#include <tuple>

// sineFit2Cycle 함수 선언
std::tuple<double, double, double> sineFit2Cycle(const std::vector<double>& data, int nT);

#endif // FITSINE_H
