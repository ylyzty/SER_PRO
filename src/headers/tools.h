/*******************************************
 * Created by zyuli on 2024/3/22 
 *******************************************/

#ifndef SER_PRO_TOOLS_H
#define SER_PRO_TOOLS_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "logicMaskSAT.h"

/**
 * 计算数组长度
 * @tparam T
 * @param array
 * @return
 */
template <class T>
int getArrayLength(T& array);

/**
 * 定义泛型函数, 打印 map
 * @tparam P
 * @tparam Q
 */
template<class P, class Q>
void printMap(std::map<P, Q> curMap) {
    for (auto iter = curMap.begin(); iter != curMap.end(); ++iter) {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }
}

/**
 * 向下转为偶数
 * @param t
 * @return
 */
unsigned int toEven(unsigned int t);

/**
 * 判断是否为偶数
 * @param t
 * @return
 */
bool isEven(unsigned int t);

/**
 * 翻转 vector
 * @param vec
 */
void reverseVectorInPlace(std::vector<unsigned int>& vec);
std::vector<unsigned int> reverseVectorWithReturns(std::vector<unsigned int> vec);
void swap(std::vector<unsigned int>& vec, int l, int r);

/**
 * 打印 path 路径
 * @param path
 */
void printAigerPath(std::vector<unsigned int> path);

/**
 * 路径数量是否可求解
 * @param pathNums
 * @param maxNums
 * @return
 */
bool isRequired(long long pathNums, long long maxNums);

/**
 * 字符串分割
 * @param origin
 * @param delim
 * @param tokens
 * @return
 */
void split(const std::string& origin, const std::string& delim, std::vector<std::string>& tokens);

/**
 * 判断该与门是否直接关联输入节点
 * @param rhs0
 * @param rhs1
 * @return
 */
bool isConnectedInputs(unsigned int rhs0, unsigned int rhs1, unsigned int inputCnt);

/**
 * 并集
 * @param set1
 * @param set2
 * @return
 */
void setUnion(std::set<unsigned int>* set1,
                                 std::set<unsigned int>* set2,
                                 std::set<unsigned int>* curSet);

/**
 * 交集
 * @param set1
 * @param set2
 * @return
 */
void setIntersection(std::set<unsigned int>* set1,
                                        std::set<unsigned int>* set2,
                                        std::set<unsigned int>* curSet);

#endif //SER_PRO_TOOLS_H
