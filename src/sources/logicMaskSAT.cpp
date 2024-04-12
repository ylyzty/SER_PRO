/*******************************************
 * Created by zyuli on 2024/3/21 
 *******************************************/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <queue>
#include <set>
#include <map>

#include "../headers/logicMaskSAT.h"
#include "../headers/tools.h"



/* ========== GLOBAL VARIABLES ========== */
//std::map<unsigned int, aiger_and*> lit2And;
Minisat::Solver* solver;
aiger* circuitModel;
AigState* aigState;

Fan* fanoutGraph;
AigerPathIn2Out** pathMap;

unsigned int inputNums;
unsigned int andNums;
unsigned int outputNums;
unsigned int firstInputIdx;
unsigned int firstAndIdx;

int varNums;

/* ========== FUNCTIONS ========== */

/**
 * Read aiger file
 */
int readAagFile(const char *aagFile) {
    circuitModel = (aiger *) malloc(sizeof(aiger));
    circuitModel = aiger_init();
    aiger_open_and_read_from_file(circuitModel, aagFile);
    aiger_check(circuitModel);

    // 根据解析的aag文件完善信息
    inputNums = circuitModel->num_inputs;
    andNums = circuitModel->num_ands;
    outputNums = circuitModel->num_outputs;

    // TODO: 待释放空间
    fanoutGraph = new Fan[inputNums + andNums];
    createFanout();

    // TODO: 待释放空间
    pathMap = new AigerPathIn2Out*[inputNums + andNums];
    for (int i = 0; i < inputNums + andNums; i++) {
        pathMap[i] = new AigerPathIn2Out[outputNums];
    }
    createPathMap(pathMap);

    aigToSATInit();
    aigToSAT();

    int SATNum = getPathSATNum(pathMap[2][0].pathToOutputs.at(0).path);
    std::cout << "SATNum: " << SATNum << std::endl;

    // TODO: 释放内存空间
    delete solver;
    delete[] fanoutGraph;
    for (int i = 0; i < (inputNums + andNums); i++) {
        delete [] pathMap[i];
    }

    return 0;
}


/**
 * 根据 Aiger 类型的 CircuitModel, 建立电路的连接关系
 */
void createFanout() {
    std::queue<unsigned int> queue;
    int tmpVisited[inputNums + andNums];

    for (int i = 0; i < inputNums + andNums; i++) {
        tmpVisited[i] = 0;
    }

    for (int i = 0; i < outputNums; i++) {
        // 将所有输出下标添加到队列
        queue.push((circuitModel->outputs + i)->lit);
    }

    while (!queue.empty()) {
        unsigned int tmpV = queue.front();
        tmpVisited[toEven(tmpV) / 2 - 1] = 1;

        if (toEven(tmpV) <= inputNums * 2) {
            queue.pop();    // 弹出队首元素
        } else {
            /**
             * 对 aag 文件的约束：
             *     1. 输入索引需要严格递增, 如 2, 4, 6, ...,
             *     2. 与门的输出也需要严格递增, 如 8, 10, 12, ...
             *     3. 否则代码可能会出现数组访问越界, 造成问题
             */
            unsigned int rhs0, rhs1;
            rhs0 = (circuitModel->ands + toEven(tmpV) / 2 - inputNums - 1)->rhs0;
            rhs1 = (circuitModel->ands + toEven(tmpV) / 2 - inputNums - 1)->rhs1;

            if (tmpVisited[toEven(rhs0) / 2 - 1] == 0) {
                queue.push(rhs0);
                tmpVisited[toEven(rhs0) / 2 - 1] = 1;
            }

            if (tmpVisited[toEven(rhs1) / 2 - 1] == 0) {
                queue.push(rhs1);
                tmpVisited[toEven(rhs1) / 2 - 1] = 1;
            }

            fanoutGraph[(toEven(rhs0) / 2 - 1)].fanouts.push_back((int) tmpV);
            fanoutGraph[(toEven(rhs1) / 2 - 1)].fanouts.push_back((int) tmpV);

            queue.pop();    // 弹出队首元素
        }
    }
}


/**
 * 计算每个 And-Gate 能够影响到的输出
 */
int getAffectedOutputs(unsigned int andLit, std::vector<unsigned int>* affectedOutputLitVector) {
    std::queue<unsigned int> queue;
    std::set<unsigned int> AllOutputLitSet;
    std::set<unsigned int> addedOutputLitSet;

    int affectedOutputCnt = 0;
    int tmpVisited[inputNums + andNums];

    for (int i = 0; i < outputNums; i++) {
        AllOutputLitSet.insert(toEven((circuitModel->outputs + i)->lit));
    }

    queue.push(andLit);
    while (!queue.empty()) {
        unsigned int tmpV = queue.front();

        /*
         * 扇出列表为空: 该节点为输出端
         * addedOutputLitSet 不包含: 该节点未被插入 affectedOutputLitVector 列表
         * */
        if (fanoutGraph[toEven(tmpV) / 2 - 1].fanouts.empty()) {
            if (addedOutputLitSet.find(toEven(tmpV)) == addedOutputLitSet.end()) {
                affectedOutputCnt += 1;
                addedOutputLitSet.insert(toEven(tmpV));
                affectedOutputLitVector->push_back(toEven(tmpV));
            }

            queue.pop();
            continue;
        } else {
            std::vector<int> fanouts = fanoutGraph[toEven(tmpV) / 2 - 1].fanouts;
            for (int fanout: fanouts) {
                if (tmpVisited[toEven(fanout) / 2 - 1] == 0) {
                    queue.push(fanout);
                    tmpVisited[toEven(fanout) / 2 - 1] = 1;
                }
            }

            queue.pop();
        }
    }

    return affectedOutputCnt;
}


/**
 * 从输出开始寻找到输入的所有路径, 构建 pathmap
 * @param pathMap
 */
void createPathMap(AigerPathIn2Out **pathMap) {
    for (int j = 0; j < outputNums; j++) {
        std::vector<unsigned int> mainStack;
        std::stack<std::vector<unsigned int>> assistStack;

        unsigned int outputLit = (circuitModel->outputs + j)->lit;

        // 初始化主栈
        mainStack.push_back(outputLit);

        // 初始化辅栈
        aiger_and *curAnd = circuitModel->ands + (toEven(outputLit) / 2 - inputNums - 1);
        std::vector<unsigned int> tmpVec = {curAnd->rhs1, curAnd->rhs0};
        assistStack.push(tmpVec);

        while (!mainStack.empty()) {
            AigerPath tmpPath;
            tmpPath.path = reverseVectorWithReturns(mainStack);
            pathMap[toEven(mainStack.back()) / 2 - 1][j].pathToOutputs.push_back(tmpPath);

            // 取队尾元素
            if (toEven(mainStack.back()) <= inputNums * 2) {
                // 更新辅栈
                while ((!mainStack.empty()) && (!assistStack.empty()) && assistStack.top().empty()) {
                    mainStack.pop_back();
                    assistStack.pop();
                }
            }

            if (!mainStack.empty() && !assistStack.empty()) {
                // 从辅栈弹出一个值压入主栈
                unsigned int next = assistStack.top().back();
                assistStack.top().pop_back();
                mainStack.push_back(next);

                int index = (int) (toEven(next) / 2 - inputNums - 1);
                if (index >= 0) {
                    tmpVec = {(circuitModel->ands + index)->rhs1, (circuitModel->ands + index)->rhs0};
                }
                else {
                    tmpVec = {};
                }
                assistStack.push(tmpVec);
            }
        }
    }
}


/**
 * 给 AigState 分配内存
 */
void aigToSATInit() {
    aigState = (AigState*) malloc(sizeof(AigState));

    aigState->inputs = (int*) malloc(circuitModel->num_inputs * sizeof(int));
    aigState->latches = (Latch*) malloc(circuitModel->num_latches * sizeof(Latch));
    aigState->ands = (int*) malloc(circuitModel->num_ands * sizeof(int));
}


/**
 * 将 Aiger 电路转化为 SAT约束
 */
void aigToSAT() {
    varNums = 0;
    // TODO: 会造成内存泄漏，需调整写法
    solver = new Minisat::Solver();
    createNewVar();     // 跳过 0

    firstInputIdx = 1;
    firstAndIdx = firstInputIdx + circuitModel->num_inputs;

    for (int i = 0; i < circuitModel->num_inputs; i++) {
        aigState->inputs[i] = createNewVar();
    }

    for (int i = 0; i < circuitModel->num_ands; i++) {
        int lhsVar = createNewVar();
        aigState->ands[i] = lhsVar;

        aiger_and *curAnd = circuitModel->ands + i;
        andGateConstraint(lhsVar,
                          import(aigState, curAnd->rhs0),
                          import(aigState, curAnd->rhs1));

    }

//    solver->toDimacs("aig2sat_cnf.txt");
}


/**
 * 创建新的 Minisat::Var 变量
 * @return
 */
int createNewVar() {
    int i = solver->newVar();
    return varNums++;
}

/**
 * 数值类型 转 Lit类型
 * @param var
 * @return
 */
Minisat::Lit varToLit(int var) {
    return var > 0 ? Minisat::mkLit(var, false) : Minisat::mkLit(-var, true);
}

/**
 * 计算 Lit 对应的上一级与门/输入
 * @param state
 * @param lit
 * @return
 */
int import(AigState* state, unsigned int lit) {
    int tmpIdx = (int) lit / 2;
    int idx = 0;
    int res = 0;

    if (tmpIdx <= 0) {
        printf("Error: abnormal index!");
        return 0;
    }
    else if (tmpIdx < firstAndIdx) {
        idx = state->inputs[tmpIdx - 1];
    }
    else {
        idx = state->ands[tmpIdx - firstAndIdx];
    }

    res = (lit & 1) ? -idx : idx;
    return res;
}

/**
 * 添加一元约束
 * @param p
 */
void unary(int p) {
    Minisat::Lit litA = (p > 0) ? Minisat::mkLit(p, false) : Minisat::mkLit(-p, true);
    solver->addClause(litA);
}

/**
 * 添加二元约束
 * @param p
 * @param q
 */
void binary(int p, int q) {
    Minisat::Lit litA = ((p > 0) ? Minisat::mkLit(p, false) : Minisat::mkLit(-p, true));
    Minisat::Lit litB = ((q > 0) ? Minisat::mkLit(q, false) : Minisat::mkLit(-q, true));

    solver->addClause(litA, litB);
}

void ternary(int p, int q, int r) {
    Minisat::Lit litA = (p > 0) ? Minisat::mkLit(p, false) : Minisat::mkLit(-p, true);
    Minisat::Lit litB = (q > 0) ? Minisat::mkLit(q, false) : Minisat::mkLit(-q, true);
    Minisat::Lit litC = (r > 0) ? Minisat::mkLit(r, false) : Minisat::mkLit(-r, true);

    solver->addClause(litA, litB, litC);
}

void andGateConstraint(int lhs, int rhs0, int rhs1) {
    binary(-lhs, rhs0);
    binary(-lhs, rhs1);
    ternary(lhs, -rhs0, -rhs1);
}

int getAndGateSATNum(unsigned int andLit) {
    std::vector<unsigned int>* affectedOutputLitVector;
    getAffectedOutputs(andLit, affectedOutputLitVector);
}

/**
 * 计算敏化路径可以被敏化的 输入向量个数
 * @param path
 * @return
 */
int getPathSATNum(std::vector<unsigned int> path) {
    int res = 0;

    int slow = 0;
    int fast = 1;
    unsigned int rhs0, rhs1;
    unsigned int cur, lhs;

    while (fast < path.size()) {
        cur = path[slow];
        lhs = path[fast];
        aiger_and *curAnd = circuitModel->ands + (toEven(lhs) / 2 - inputNums - 1);
        if (toEven(cur) == toEven(curAnd->rhs0)) {
            unary(import(aigState, curAnd->rhs1));
        }
        else {
            unary(import(aigState, curAnd->rhs0));
        }

        slow += 1;
        fast += 1;
    }

    bool sat = solver->solve();
    Minisat::vec<Minisat::Lit> tmpLits;
    while (sat) {
        res += 1;
        tmpLits.clear();
        for (int i = 0; i < inputNums; i++) {
            aiger_symbol *curInput = circuitModel->inputs + i;
            Minisat::Lit curLit = varToLit(import(aigState, curInput->lit));
            Minisat::lbool value = solver->modelValue(curLit);

            if (value == Minisat::lbool((uint8_t)0)) {
                // TRUE ~P
                tmpLits.push(varToLit(import(aigState, curInput->lit + 1)));
                std::cout << "TRUE  ";
            }
            else {
                // FALSE P
                tmpLits.push(varToLit(import(aigState, curInput->lit)));
                std::cout << "FALSE  ";
            }
        }
        std::cout << std::endl;

        solver->addClause(tmpLits);
        sat = solver->solve();
    }

    return res;
}
