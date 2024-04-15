/*******************************************
 * Created by zyuli on 2024/3/22 
 *******************************************/

#ifndef SER_PRO_TOOLS_H
#define SER_PRO_TOOLS_H
#include <iostream>
#include <vector>
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
 * 向下转为偶数
 * @param t
 * @return
 */
unsigned int toEven(unsigned int t);

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

#endif //SER_PRO_TOOLS_H
