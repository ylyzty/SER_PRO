/*******************************************
 * Created by zyuli on 2024/6/24 
 *******************************************/

#ifndef SER_PRO_RECONVERGENCESAT_H
#define SER_PRO_RECONVERGENCESAT_H

#include <map>
#include "logicMaskSAT.h"

extern Minisat::Solver* solver;
extern aiger* circuitModel;
extern AigState* aigState;
extern Fan* fanoutGraph;    // 存储的都是偶数 lhs

typedef struct ReConvergencePath {
    std::vector<AigerPath> pathToAnds;
    int pathNums;
} ReConvergencePath;

/**
 * 重汇聚分析调用入口
 */
void reConvergenceSATEntry();

/**
 * 初始化各种变量
 */
void reConvergenceInit();

/**
 * 计算 所有重汇聚路径
 */
void createReConvergenceMap();

/**
 * DFS 深搜
 * @param andLit
 */
void createRCMDfs(unsigned int andLit);

/**
 * 计算每条路径上的非门个数
 * @param path
 * @return
 */
int getNotGateCount(const std::vector<unsigned int> &path);

/**
 * 添加单挑路径的 SAT 约束
 * @param path
 */
void addPathConstraint(const std::vector<unsigned int> &path);

/**
 * 计算单个与门所有重汇聚路径 SAT 个数
 * @param andLit
 * @return
 */
double getAndGateReConvergenceSATNum(unsigned int andLit);

/**
 * 计算重汇聚路径的 SAT 个数
 * @param path1
 * @param path2
 * @param inputSet
 * @return
 */
double getReConvergencePathSATNum(const std::vector<unsigned int>& path1,
                                  const std::vector<unsigned int>& path2,
                                  std::set<unsigned int>* inputSet);


#endif //SER_PRO_RECONVERGENCESAT_H
