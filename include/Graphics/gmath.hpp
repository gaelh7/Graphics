#include "xtensor/xfixed.hpp"
#include "vector"

/**
 * Sign of a number.
 *
 * @param val Number to find the sign of.
 * @return 1 if val > 0, -1 if val < 0, and 0 otherwise.
 */
static constexpr int sign(const double val) {
    return (0 < val) - (val < 0);
};

/**
 * Norm of a length three vector.
 *
 * @param vec Vector to find the magnitude of.
 * @return Magnitude of the vector
 */
static double norm(const xt::xtensor_fixed<double, xt::xshape<3>> &vec){
    double mag = 0;
    for(double i: vec) mag += i*i;
    return sqrt(mag);
};

/**
 * Dot product of two length three vectors.
 *
 * @param v1 First vector.
 * @param v1 Second vector.
 * @return Dot product of v1 and v2.
 */
static double dot(const xt::xtensor_fixed<double, xt::xshape<3>> &v1, const xt::xtensor_fixed<double, xt::xshape<3>> &v2){
    double val = 0;
    for(unsigned int i = 0; i < 3; i++)
        val += v1[i]*v2[i];
    return val;
};

static xt::xtensor_fixed<double, xt::xshape<3>> cross(const xt::xtensor_fixed<double, xt::xshape<3>> &arr1, const xt::xtensor_fixed<double, xt::xshape<3>> &arr2){
    double i = arr1[1]*arr2[2] - arr1[2]*arr2[1];
    double j = arr1[2]*arr2[0] - arr1[0]*arr2[2];
    double k = arr1[0]*arr2[1] - arr1[1]*arr2[0];
    return {i, j, k};
};

/**
 * Determinant of a 3x3 matrix.
 *
 * This function does not take in a matrix or two dimensional array.
 * Instead, it takes 3 xtensor parameters which instead define the
 * columns of the matrix.
 *
 * @param v1 First column of matrix.
 * @param v2 Second column of matrix.
 * @param v3 Third column of matrix.
 * @return Determinant of the matrix.
 */
static double det(const xt::xtensor_fixed<double, xt::xshape<3>> &v1, const xt::xtensor_fixed<double, xt::xshape<3>> &v2, const xt::xtensor_fixed<double, xt::xshape<3>> &v3){
    return v1[0]*(v2[1]*v3[2] - v3[1]*v2[2]) -
           v2[0]*(v1[1]*v3[2] - v3[1]*v1[2]) +
           v3[0]*(v1[1]*v2[2] - v2[1]*v1[2]);
};

template <typename T, class iter>
std::vector<std::vector<T>> combinations(iter first, iter last, int k){
    std::vector<std::vector<T>> out;
    if(k == 1){
        for(iter i = first; i != last; i++) out.push_back(std::vector<T>{*i});
        return out;
    }
    for(iter i = first; i != last; i++){
        std::vector<T> next;
        for(std::vector<T> end: combinations<T>(i+1, last, k-1)){
            next = {*i};
            next.insert(next.end(), end.begin(), end.end());
            out.push_back(next);
        }
    }
    return out;
};
