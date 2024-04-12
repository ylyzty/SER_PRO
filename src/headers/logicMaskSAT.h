/*******************************************
 * Created by zyuli on 2024/3/21 
 *******************************************/

#ifndef SER_PRO_LOGICMASKSAT_H
#define SER_PRO_LOGICMASKSAT_H

#include <vector>
#include <stack>
#include <algorithm>

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

typedef struct AigerPathIn2Out {
    std::vector<AigerPath> pathToOutputs;
    std::vector<int> solvedResult;
    int pathIn2OutNum;
} AigerPathIn2Out;

typedef struct Fan {
    std::vector<int> fanouts;
} Fan;


/* ========= declare func ======== */
int readAagFile(const char* filename);

/**
 * 根据 Aiger 类型的 CircuitModel, 建立电路的连接关系
 */
void createFanout();

/**
 * 计算每个 And-Gate 能够影响到的输出
 */
int getAffectedOutputs(unsigned int andLit, std::vector<unsigned int> *affectedOutputLitVector);

/**
 * 从输出开始寻找到输入的所有路径, 构建 pathmap
 * @param pathMap
 */
void createPathMap(AigerPathIn2Out **pathMap);

/**
 * 给 AigState 分配内存
 */
void aigToSATInit();

/**
 * 将 Aiger 电路转化为 SAT约束
 */
void aigToSAT();

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
 */
void unary(int p);

/**
 * 二元约束
 * @param p
 * @param q
 */
void binary(int p, int q);

/**
 * 三元约束
 * @param p
 * @param q
 * @param r
 */
void ternary(int p, int q, int r);

/**
 * 与门约束
 * @param lhs
 * @param rhs0
 * @param rhs1
 */
void andGateConstraint(int lhs, int rhs0, int rhs1);

int getPathSATNum(std::vector<unsigned int> path);

void buildLit2And();

#endif //SER_PRO_LOGICMASKSAT_H
