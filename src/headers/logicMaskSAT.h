/*******************************************
 * Created by zyuli on 2024/3/21 
 *******************************************/

#ifndef SER_PRO_LOGICMASKSAT_H
#define SER_PRO_LOGICMASKSAT_H

#include <vector>
#include <stack>
#include <set>
#include <algorithm>

#include "../headers/constants.h"
#include "../headers/tools.h"
#include "../../minisat/core/Solver.h"

extern "C" {
#include "aiger.h"
}

typedef struct Latch {
    int lit;
    int next;
} Latch;

typedef struct AigState {
    int time;
    int* outputs;
    int* inputs;
    Latch* latches;
    int* ands;
    int* bad;
    int oneBad;
    int assume;
} AigState;

typedef struct AigerPath {
    std::vector<unsigned int> path;
    int pathNo;
} AigerPath;

typedef struct AigerPathToOutputs {
    std::vector<AigerPath> pathToOutputs;
    std::vector<int> solvedResult;
    int pathNums;
} AigerPathIn2Out;

typedef struct Fan {
    std::vector<int> fanouts;
} Fan;


/* ========= declare func ======== */
int readAagFile(const char* filename);

void checkAigerModel(aiger* model);
void checkAigerInputs(aiger* model);
void checkAigerAndGates(aiger* model);

/**
 * 根据 Aiger 类型的 CircuitModel, 建立电路的连接关系
 */
void createFanout();

/**
 * 计算每个 And-Gate 能够影响到的输出
 * @param andLit
 * @param affectedOutputLitVector
 * @return
 */
int getAndLitAffectedOutputs(unsigned int andLit, std::vector<unsigned int> *affectedOutputLitVector);

/**
 * 计算每个 And-Gate 关联到的输入个数
 * @param andLit
 * @param associatedInputLitSet
 * @return
 */
int getAndLitAssociatedInputs(unsigned int andLit, std::set<unsigned int> *associatedInputLitSet);

/**
 * 计算一条路径关联到的输入个数
 * @param path
 * @param associatedInputLitSet
 * @return
 */
int getPathAssociatedInputs(std::vector<unsigned int> path, std::set<unsigned int> *associatedInputLitSet);

/**
 * 从输出开始寻找到输入的所有路径, 构建 pathmap
 * @param pathMap
 */
long long createPathMap(bool isStore);

/**
 * 给 AigState 分配内存
 */
void aigToSATInit();

/**
 * 将 Aiger 电路转化为 SAT约束
 */
void aigToSAT();

/**
 * 刷新 Minisat::Solver
 */
void refreshSolver();

/**
 * 创建新的 Minisat::Lit 变量下标
 * @return
 */
int createNewVar();

/**
 * 数值类型 转 Lit类型
 * @param var
 * @return
 */
Minisat::Lit varToLit(int var);

/**
 * 计算 Lit 对应的下标
 * @param state
 * @param lit
 * @return
 */
int import(AigState* state, unsigned int lit);

/**
 * 一元约束
 * @param p
 * @param flag: 是否是电路初始约束
 */
void unary(int p, bool flag);

/**
 * 二元约束
 * @param p
 * @param q
 * @param flag: 是否是电路初始约束
 */
void binary(int p, int q, bool flag);

/**
 * 三元约束
 * @param p
 * @param q
 * @param r
 * @param flag: 是否是电路初始约束
 */
void ternary(int p, int q, int r, bool flag);

/**
 * 与门约束
 * @param lhs
 * @param rhs0
 * @param rhs1
 */
void andGateConstraint(int lhs, int rhs0, int rhs1, bool flag);

double getPathSATNum(std::vector<unsigned int> path, std::set<unsigned int>* inputSet);

void buildLit2And();

#endif //SER_PRO_LOGICMASKSAT_H
