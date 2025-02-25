#include "fitSine.h"
#include <Eigen/Dense>
#include <cmath>

// **사인파 피팅 함수**
std::tuple<double, double, double> sineFit2Cycle(const std::vector<double>& data, int nT) {
    int N = static_cast<int>(data.size());
    Eigen::VectorXd t(N), y(N);

    for (int i = 0; i < N; ++i) {
        t(i) = (i * nT * 2 * M_PI) / N;
        y(i) = data[i];
    }

    // 선형 모델 구성: y = A*sin(t) + B*cos(t) + C
    Eigen::MatrixXd A(N, 3);
    A.col(0) = t.array().sin();
    A.col(1) = t.array().cos();
    A.col(2) = Eigen::VectorXd::Ones(N);

    // 최소자승법(Least Squares)으로 해 구하기
    Eigen::VectorXd params = A.colPivHouseholderQr().solve(y);

    double A_sin = params(0);
    double B_cos = params(1);
    double C_mean = params(2);

    double amplitude = std::sqrt(A_sin * A_sin + B_cos * B_cos);
    double phase = std::atan2(B_cos, A_sin);
    double mean = C_mean;

    return std::make_tuple(amplitude, phase, mean);
}
