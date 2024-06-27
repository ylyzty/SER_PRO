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
 * 定义泛型函数, 打印 map
 * @tparam P
 * @tparam Q
 */
template<class P, class Q>
void printMap(std::map<P, Q> curMap);


/**
 * 将无符号整数 转为 偶数(向下取)
 * @param t
 * @return
 */
unsigned int toEven(unsigned int t) {
    return (t & 1) ? (t - 1) : t;
}

bool isEven(unsigned int t) {
    return (t & 1) == 0;
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

void printAigerPath(std::vector<unsigned int> path) {
    for (int i = 0; i < path.size(); i++) {
        if (i == path.size() - 1) {
            std::cout << path.at(i) << std::endl;
        }
        else {
            std::cout << path.at(i) << " -> ";
        }
    }
}

bool isRequired(long long pathNums, long long maxNums) {
    if (pathNums >= maxNums) {
        return false;
    }

    return true;
}

void split(const std::string& origin, const std::string& delim, std::vector<std::string>& tokens) {
    tokens.clear();

    auto start = origin.find_first_not_of(delim, 0);
    auto pos = origin.find_first_of(delim, start);
    while (pos != std::string::npos || start != std::string::npos) {
        tokens.emplace_back(std::move(origin.substr(start, pos - start)));

        start = origin.find_first_not_of(delim, pos);
        pos = origin.find_first_of(delim, start);
    }
}

bool isConnectedInputs(unsigned int rhs0, unsigned int rhs1, unsigned int inputCnt) {
    if (toEven(rhs0) <= (2 * inputCnt)) {
        return true;
    }

    if (toEven(rhs1) <= (2 * inputCnt)) {
        return true;
    }

    return false;
}

void setUnion(std::set<unsigned int>* set1,
              std::set<unsigned int>* set2,
              std::set<unsigned int>* curSet) {

    for (unsigned int input : *set1) {
        curSet->insert(input);
    }
    for (unsigned int input : *set2) {
        curSet->insert(input);
    }
}

void setIntersection(std::set<unsigned int>* set1,
                     std::set<unsigned int>* set2,
                     std::set<unsigned int>* curSet) {

    for (unsigned int input : *set1) {
        if (set2->find(input) != set2->end()) {
            curSet->insert(input);
        }
    }
}
