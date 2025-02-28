#ifndef FITSINE_H
#define FITSINE_H

#include <vector>
#include <tuple>


std::tuple<double, double, double> sineFit2Cycle(const std::vector<double>& data, int nT);

#endif // FITSINE_H
