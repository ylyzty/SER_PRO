/*******************************************
 * Created by zyuli on 2024/3/22 
 *******************************************/

#include "../../minisat/core/Solver.h"
#include "../headers/tools.h"

extern Minisat::Solver* solver;

template <class T>
int getArrayLength(T& array) {
    return (sizeof(array) / sizeof(array[0]));
}


/**
 * 将无符号整数 转为 偶数(向下取)
 * @param t
 * @return
 */
unsigned int toEven(unsigned int t) {
    return (t & 1) ? (t - 1) : t;
}

void reverseVectorInPlace(std::vector<unsigned int>& vec) {
    int left = 0;
    int right = vec.size() - 1;

    while (left < right) {
        swap(vec,left, right);

        left += 1;
        right -= 1;
    }
}

std::vector<unsigned int> reverseVectorWithReturns(std::vector<unsigned int> vec) {
    int left = 0;
    int right = vec.size() - 1;

    while (left < right) {
        swap(vec,left, right);

        left += 1;
        right -= 1;
    }

    return vec;
}

void swap(std::vector<unsigned int>& vec, int l, int r) {
    unsigned int tmp = vec.at(l);
    vec.at(l) = vec.at(r);
    vec.at(r) = tmp;
}