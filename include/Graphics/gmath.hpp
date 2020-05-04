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
static double norm(glm::dvec3 &vec){
    double mag = 0;
    for(int i = 0; i < vec.length(); i++) mag += vec[i]*vec[i];
    return sqrt(mag);
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
static double det(glm::dvec3 &v1, glm::dvec3 &v2, glm::dvec3 &v3){
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
