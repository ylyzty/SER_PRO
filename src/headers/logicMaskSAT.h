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
    int pathNums;
} AigerPathToOutputs;

typedef struct Fan {
    std::vector<int> fanouts;
} Fan;


/* Global Variables */
//extern Minisat::Solver* solver;
//extern aiger* circuitModel;
//extern AigState* aigState;
//extern Fan* fanoutGraph;


extern const long long MAX_PATH_NUMS;

/* ========= declare func ======== */
int readAagFile(const char* filename);

/**
 * Get circuit path numbers
 */
long long checkCircuitPaths(const char *aagFile);

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

/**
 * 计算单个与门的 SAT 可满足解的个数
 * 包含多条敏化路径
 * @param andIndex
 * @return
 */
double getAndGateSATNum(unsigned int andIndex);

/**
 * 计算单条敏化路径的 SAT 可满足解个数
 * @param path
 * @param inputSet
 * @return
 */
double getPathSATNum(std::vector<unsigned int> path, std::set<unsigned int>* inputSet);

/**
 * NodeLit  --> NodeID
 *
 * 给每个 Node 分配一个唯一的 ID
 */
void mapCircuit();

/**
 * 将 Circuit Info 写入文件
 * @param targetPath
 */
void writeCircuit(const std::string& targetPath);

/**
 * 校验 Aiger Model 是否规范
 * @param model
 */
void checkAigerModel(aiger* model);
void checkAigerInputs(aiger* model);
void checkAigerAndGates(aiger* model);
void checkAigerOutputs(aiger* model);

void buildLit2And();

/**
 * 释放申请的内存空间
 */
void releaseMem();

#endif //SER_PRO_LOGICMASKSAT_H
