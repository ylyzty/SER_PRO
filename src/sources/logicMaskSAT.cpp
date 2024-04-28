/*******************************************
 * Created by zyuli on 2024/3/21 
 *******************************************/
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <queue>
#include <set>
#include <map>
#include <chrono>

#include "../headers/logicMaskSAT.h"

/* ========== GLOBAL VARIABLES ========== */
Minisat::Solver* solver;
aiger* circuitModel;
AigState* aigState;
std::vector<std::vector<Minisat::Lit>>* circuitClauses;

Fan* fanoutGraph;
AigerPathToOutputs** pathMap;

unsigned int inputNums;
unsigned int andNums;
unsigned int outputNums;
unsigned int firstInputIdx;
unsigned int firstAndIdx;

int varNums;

/* ========== MAPS ========== */
std::map<unsigned int, int> outputLitToIndex;

/* ========== FUNCTIONS ========== */

/**
 * Read aiger file
 */
int readAagFile(const char *aagFile) {
//    // 1. 统计电路解析和审查时间
//    std::chrono::steady_clock::time_point circuitAnalysisStart = std::chrono::steady_clock::now();
//    circuitModel = (aiger *) malloc(sizeof(aiger));
//    circuitModel = aiger_init();
//    aiger_open_and_read_from_file(circuitModel, aagFile);
//    aiger_check(circuitModel);
//    std::chrono::steady_clock::time_point circuitAnalysisEnd = std::chrono::steady_clock::now();
//    long long circuitAnalysisElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(circuitAnalysisEnd - circuitAnalysisStart).count();
//    std::cout << "Circuit analysis time: " << circuitAnalysisElapsed << "ms" << std::endl;
//
//    // 根据解析的aag文件完善信息
//    inputNums = circuitModel->num_inputs;
//    andNums = circuitModel->num_ands;
//    outputNums = circuitModel->num_outputs;
//
//    // 检验 aag 文件是否是 紧凑且无逆序的
//    checkAigerModel(circuitModel);
//    for (int i = 0; i < outputNums; i++) {
//        outputLitToIndex[toEven((circuitModel->outputs + i)->lit)] = i;
//    }
//
//    // 2. 统计Fanouts创建时间
//    std::chrono::steady_clock::time_point createFanoutStart = std::chrono::steady_clock::now();
//    fanoutGraph = new Fan[inputNums + andNums];
//    createFanout();
//    std::chrono::steady_clock::time_point creatFanoutEnd = std::chrono::steady_clock::now();
//    long long createFanoutElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(circuitAnalysisEnd - circuitAnalysisStart).count();
//    std::cout << "Create Fanout time: " << createFanoutElapsed << "ms" << std::endl;
//
//    // 3. 统计创建pathMap的时间
//    std::chrono::steady_clock::time_point createPathMapStart = std::chrono::steady_clock::now();
//    pathMap = new AigerPathToOutputs*[inputNums + andNums];
//    for (int i = 0; i < inputNums + andNums; i++) {
//        pathMap[i] = new AigerPathToOutputs[outputNums];
//        for (int j = 0; j < outputNums; j++) {
//            pathMap[i][j].pathToOutputs.clear();
//            pathMap[i][j].pathNums = 0;    // 设置初始值
//        }
//    }
//    createPathMap(false);
//    std::chrono::steady_clock::time_point createPathMapEnd = std::chrono::steady_clock::now();
//    long long createPathMapElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(createPathMapEnd - createPathMapStart).count();
//    std::cout << "Create PathMap time: " << createPathMapElapsed << "ms" << std::endl;
//
//    aigToSATInit();
//    // 4. 统计初始化Solver时间
//    std::chrono::steady_clock::time_point initSATSolverStart = std::chrono::steady_clock::now();
//    aigToSAT();
//    std::chrono::steady_clock::time_point initSATSolverEnd = std::chrono::steady_clock::now();
//    long long initSATSolverElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(initSATSolverEnd - initSATSolverStart).count();
//    std::cout << "Init SAT Solver time: " << initSATSolverElapsed << "ms" << std::endl;
//
//    // 5. 统计刷新Solver时间
//    std::chrono::steady_clock::time_point refreshSolverStart = std::chrono::steady_clock::now();
//    refreshSolver();
//    std::chrono::steady_clock::time_point refreshSolverEnd = std::chrono::steady_clock::now();
//    long long refreshSolverElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(refreshSolverEnd - refreshSolverStart).count();
//    std::cout << "Refresh SAT Solver time: " << refreshSolverElapsed << "ms" << std::endl;
//
//    // 6. 统计 SAT 求解时间
//    std::chrono::steady_clock::time_point SATSolvingStart = std::chrono::steady_clock::now();
//    for (unsigned int startIndex = inputNums; startIndex < (inputNums + andNums); startIndex++) {
//        unsigned int startLit = (startIndex + 1) * 2;
//        auto* affectedOutputs = new std::vector<unsigned int>();
//        getAndLitAffectedOutputs(startLit, affectedOutputs);
//        sort(affectedOutputs->begin(), affectedOutputs->end());
//
//        for (unsigned int endLit : *affectedOutputs) {
//            int endIndex = outputLitToIndex.at(toEven(endLit));
//            int pathNums = pathMap[startIndex][endIndex].pathNums;
//            std::cout << "\n" << startLit << " ==========> " << endLit << ": " << pathNums << std::endl;
//
//            std::vector<unsigned int> path;
//            double SATSum = 0;
//            double SATNum = 0;
//            int pathNo = 0;
//            for (int i = 0; i < pathNums; i++) {
//                path = pathMap[startIndex][endIndex].pathToOutputs.at(i).path;
//                pathNo += 1;
//                std::cout << "Path No." << pathNo << "\tPath length: " << path.size() << std::endl;
//
//                // 计算路径关联的输入个数
//                auto* associatedInputSet = new std::set<unsigned int>();
//                int associatedInputCnt = getPathAssociatedInputs(path, associatedInputSet);
//                std::cout << "Associated input counts " << associatedInputCnt << " / " << inputNums << std::endl;
//
//                if (path.size() <= 1) {
//                    SATNum = 1;
//                }
//                else {
//                    refreshSolver();
//                    SATNum = getPathSATNum(path, associatedInputSet);
//                }
//
//                delete associatedInputSet;
//
//                SATNum = SATNum * pow(2, inputNums - associatedInputCnt);
//                std::cout << "Path SAT Num: " << SATNum << "\n" << std::endl;
//                SATSum += SATNum;
//            }
//            std::cout << "All Path SAT Num: " << SATSum << std::endl;
//        }
//
//        delete affectedOutputs;
//    }
//    std::chrono::steady_clock::time_point SATSolvingEnd = std::chrono::steady_clock::now();
//    long long SATSolvingElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(SATSolvingEnd - SATSolvingStart).count();
//    std::cout << "SAT solving time: " << SATSolvingElapsed << "ms" << std::endl;
//
//    // TODO: 释放内存空间
//    delete solver;
//    delete circuitModel;
//    delete aigState;
//    delete circuitClauses;
//    delete[] fanoutGraph;
//    for (int i = 0; i < (inputNums + andNums); i++) {
//        delete [] pathMap[i];
//    }
//
//    return 0;
}


/**
 * Get circuit path numbers
 */
void checkCircuitPaths(const char *aagFile) {
    // 1. 统计电路解析和审查时间
    std::chrono::steady_clock::time_point circuitAnalysisStart = std::chrono::steady_clock::now();
    circuitModel = (aiger *) malloc(sizeof(aiger));
    circuitModel = aiger_init();
    aiger_open_and_read_from_file(circuitModel, aagFile);
    aiger_check(circuitModel);
    std::chrono::steady_clock::time_point circuitAnalysisEnd = std::chrono::steady_clock::now();
    long long circuitAnalysisElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(circuitAnalysisEnd - circuitAnalysisStart).count();
    std::cout << "Circuit analysis time: " << circuitAnalysisElapsed << "ms" << std::endl;

    // 根据解析的aag文件完善信息
    inputNums = circuitModel->num_inputs;
    andNums = circuitModel->num_ands;
    outputNums = circuitModel->num_outputs;

    // 检验 aag 文件是否是 紧凑且无逆序的
    checkAigerModel(circuitModel);
    for (int i = 0; i < outputNums; i++) {
        outputLitToIndex[toEven((circuitModel->outputs + i)->lit)] = i;
    }

    long long pathCount = createPathMap(false);
    std::cout << pathCount << std::endl;
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
 * @param andLit
 * @param affectedOutputLitVector
 * @return
 */
int getAndLitAffectedOutputs(unsigned int andLit, std::vector<unsigned int>* affectedOutputLitVector) {
    std::queue<unsigned int> queue;
    std::set<unsigned int> AllOutputLitSet;
    std::set<unsigned int> addedOutputLitSet;

    int affectedOutputCnt = 0;
    int tmpVisited[inputNums + andNums];
    for (int i = 0; i < (inputNums + andNums); i++) {
        tmpVisited[i] = 0;
    }

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
            for (int fanout : fanouts) {
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
 * 计算每个 And-Gate 被影响的输入
 * 添加的是输入的 Lit
 * @param andLit
 * @param associatedInputLitSet
 * @return
 */
int getAndLitAssociatedInputs(unsigned int andLit, std::set<unsigned int> *associatedInputLitSet) {
    std::queue<unsigned int> queue;
    std::set<unsigned int> addedAndLitSet;

    int associatedInputCnt = 0;
    int tmpVisited[inputNums + andNums];
    // 局部数组的默认初始化为随机值, 需要手动初始化
    for (int i = 0; i < (inputNums + andNums); i++) {
        tmpVisited[i] = 0;
    }

    queue.push(andLit);
    while (!queue.empty()) {
        unsigned int tmpV = queue.front();

        if (tmpV < 2 * (inputNums + 1)) {
            // 到达输入节点，将其添加到 关联输入集合 中
            associatedInputLitSet->insert(toEven(tmpV));
            associatedInputCnt += 1;
        }
        else {
            aiger_and *curAnd = circuitModel->ands + (toEven(tmpV) / 2 - inputNums - 1);
            if (tmpVisited[toEven(curAnd->rhs0) / 2 - 1] == 0) {
                queue.push(curAnd->rhs0);
                tmpVisited[toEven(curAnd->rhs0) / 2 - 1] = 1;
            }

            if (tmpVisited[toEven(curAnd->rhs1) / 2 - 1] == 0) {
                queue.push(curAnd->rhs1);
                tmpVisited[toEven(curAnd->rhs1) / 2 - 1] = 1;
            }
        }

        queue.pop();
    }

    return associatedInputCnt;
}


/**
 * 计算一条路径关联到的输入个数
 * @param path
 * @param associatedInputLitSet
 * @return
 */
int getPathAssociatedInputs(std::vector<unsigned int> path, std::set<unsigned int> *associatedInputLitSet) {
    if (path.size() <= 1) {
        return 0;
    }
    
    for (int i = 1; i < path.size(); i++) {
        unsigned int andLit = path.at(i);
        unsigned int rhs = path.at(i - 1);
        aiger_and *curAnd = circuitModel->ands + (toEven(andLit) / 2 - inputNums - 1);
        if (curAnd->rhs0 == rhs) {
            getAndLitAssociatedInputs(curAnd->rhs1, associatedInputLitSet);
        }
        else if (curAnd->rhs1 == rhs) {
            getAndLitAssociatedInputs(curAnd->rhs0, associatedInputLitSet);
        }
        else {
            std::cerr << "Error: Abnormal RHS!" << std::endl;
            exit(ERROR_CODE_ABNORMAL_RHS_LIT);
        }
    }

    return (int) associatedInputLitSet->size();
}


/**
 * 从输出开始寻找到输入的所有路径, 构建 pathmap
 * @param isStore: true means to store path, while false means not.
 */
long long createPathMap(bool isStore) {
    std::vector<unsigned int> mainStack;
    std::stack<std::vector<unsigned int>> assistStack;

    AigerPath tmpPath;
    std::vector<unsigned int> tmpVec;
    long long allPathCount = 0;

    for (int j = 0; j < outputNums; j++) {
        // 格式化主栈和辅栈
        mainStack.clear();
        while (!assistStack.empty()) {
            assistStack.pop();
        }

        // 初始化主栈
        unsigned int outputLit = (circuitModel->outputs + j)->lit;
        mainStack.push_back(outputLit);

        // 初始化辅栈
        aiger_and *curAnd = circuitModel->ands + (toEven(outputLit) / 2 - inputNums - 1);
        tmpVec.clear();
        tmpVec = {curAnd->rhs1, curAnd->rhs0};
        assistStack.push(tmpVec);

        while (!mainStack.empty()) {
            if (isStore) {
                tmpPath.path = reverseVectorWithReturns(mainStack);
                pathMap[toEven(mainStack.back()) / 2 - 1][j].pathToOutputs.push_back(tmpPath);
                pathMap[toEven(mainStack.back()) / 2 - 1][j].pathNums += 1;
            }

            // 判断队尾元素是否为输入
            if (toEven(mainStack.back()) <= inputNums * 2) {
                // 到达一次输入，则说明多一条 path
                allPathCount += 1;

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
                tmpVec.clear();
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

    return allPathCount;
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
    solver = new Minisat::Solver();
    circuitClauses = new std::vector<std::vector<Minisat::Lit>>();

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
                          import(aigState, curAnd->rhs1),
                          true);

    }

//    std::string filename = "cnfs/adder.txt";
//    FILE* f = fopen(filename.c_str(), "w+");
//    if (f == nullptr) {
//        printf("");
//    }
//
//    fprintf(f, "write to txt");
//    fclose(f);
//    solver->toDimacs(filename.c_str());
}

void refreshSolver() {
    delete solver;

    solver = new Minisat::Solver();
    int cnt = varNums;
    varNums = 0;

    //  还原 Solver 变量
    for (int i = 0; i < cnt; i++) {
        createNewVar();
    }

    //  还原 Solver 约束
    for (auto cur : *circuitClauses) {
        if (cur.size() == 2) {
            solver->addClause(cur.at(0), cur.at(1));
        }
        else if (cur.size() == 3) {
            solver->addClause(cur.at(0), cur.at(1), cur.at(2));
        }
        else if (cur.size() == 1) {
            solver->addClause(cur.at(0));
        }
        else {
            std::cerr << "Error: Abnormal length of clauses!" << std::endl;
            exit(ERROR_CODE_ABNORMAL_CLAUSES_LENGTH);
        }
    }
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
        std::cerr << "Error: Abnormal lit!" << std::endl;
        exit(ERROR_CODE_ABNORMAL_LIT);
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
void unary(int p, bool flag) {
    Minisat::Lit litA = (p > 0) ? Minisat::mkLit(p, false) : Minisat::mkLit(-p, true);
    solver->addClause(litA);

    if (flag) {
        std::vector<Minisat::Lit> vec;
        vec.push_back(litA);
        circuitClauses->push_back(vec);
    }
}

/**
 * 添加二元约束
 * @param p
 * @param q
 */
void binary(int p, int q, bool flag) {
    Minisat::Lit litA = ((p > 0) ? Minisat::mkLit(p, false) : Minisat::mkLit(-p, true));
    Minisat::Lit litB = ((q > 0) ? Minisat::mkLit(q, false) : Minisat::mkLit(-q, true));
    solver->addClause(litA, litB);

    if (flag) {
        std::vector<Minisat::Lit> vec;
        vec.push_back(litA);
        vec.push_back(litB);
        circuitClauses->push_back(vec);
    }
}

void ternary(int p, int q, int r, bool flag) {
    Minisat::Lit litA = (p > 0) ? Minisat::mkLit(p, false) : Minisat::mkLit(-p, true);
    Minisat::Lit litB = (q > 0) ? Minisat::mkLit(q, false) : Minisat::mkLit(-q, true);
    Minisat::Lit litC = (r > 0) ? Minisat::mkLit(r, false) : Minisat::mkLit(-r, true);
    solver->addClause(litA, litB, litC);

    if (flag) {
        std::vector<Minisat::Lit> vec;
        vec.push_back(litA);
        vec.push_back(litB);
        vec.push_back(litC);
        circuitClauses->push_back(vec);
    }
}

void andGateConstraint(int lhs, int rhs0, int rhs1, bool flag) {
    binary(-lhs, rhs0, flag);
    binary(-lhs, rhs1, flag);
    ternary(lhs, -rhs0, -rhs1, flag);
}

int getAndGateSATNum(unsigned int andLit) {
    std::vector<unsigned int>* affectedOutputLitVector;
    getAndLitAffectedOutputs(andLit, affectedOutputLitVector);

    return 0;
}

/**
 * 计算敏化路径可以被敏化的 输入向量个数
 * @param path
 * @return
 */
double getPathSATNum(std::vector<unsigned int> path, std::set<unsigned int>* inputSet) {
    double res = 0;

    int slow = 0;
    int fast = 1;
    unsigned int rhs0, rhs1;
    unsigned int cur, lhs;

    while (fast < path.size()) {
        cur = path[slow];
        lhs = path[fast];
        aiger_and *curAnd = circuitModel->ands + (toEven(lhs) / 2 - inputNums - 1);
        if (toEven(cur) == toEven(curAnd->rhs0)) {
            unary(import(aigState, curAnd->rhs1), false);
        }
        else {
            unary(import(aigState, curAnd->rhs0), false);
        }

        slow += 1;
        fast += 1;
    }

    bool sat = solver->solve();
    Minisat::vec<Minisat::Lit> tmpLits;
    while (sat) {
        res += 1;
        tmpLits.clear();
        for (unsigned int inputLit : *inputSet) {
            aiger_symbol *curInput = circuitModel->inputs + (inputLit / 2 - 1);
            Minisat::Lit curLit = varToLit(import(aigState, curInput->lit));
            Minisat::lbool value = solver->modelValue(curLit);

            if (value == Minisat::lbool((uint8_t) 0)) {
                // TRUE ~P
                tmpLits.push(varToLit(import(aigState, curInput->lit + 1)));
            }
            else if (value == Minisat::lbool((uint8_t) 1)) {
                // FALSE P
                tmpLits.push(varToLit(import(aigState, curInput->lit)));
            }
            else if (value == Minisat::lbool((uint8_t) 2)) {
                std::cout << "WARNING: Unknown SAT solve!" << std::endl;

                // 求出未知解, 需跳出 SAT 求解器
                sat = false;
                break;
            }
            else {
                std::cerr << "ERROR: Abnormal SAT solve!" << std::endl;
                exit(ERROR_CODE_ABNORMAL_SAT_SOLVE);
            }
        }

        // 求出未知解，跳出 while 循环，不继续求解
        if (!sat) {
            break;
        }

        solver->addClause(tmpLits);

//        std::chrono::steady_clock::time_point solveStart = std::chrono::steady_clock::now();
        sat = solver->solve();
//        std::chrono::steady_clock::time_point solveEnd = std::chrono::steady_clock::now();
//        long long solveElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(solveEnd - solveStart).count();
//        std::cout << "Solve time: " << solveElapsed << "ms" << std::endl;
    }

    return res;
}


void checkAigerModel(aiger* model) {
    checkAigerInputs(model);
    checkAigerAndGates(model);
    checkAigerOutputs(model);
}

void checkAigerInputs(aiger* model) {
    for (int i = 1; i < inputNums; i++) {
        if ((model->inputs + i)->lit - (model->inputs + i - 1)->lit != 2) {
            std::cerr << "Error: Aag file with inputs not compact!" << std::endl;
            exit(ERROR_CODE_UNCOMPACT);
        }
    }
}

void checkAigerAndGates(aiger* model) {
    for (int i = 1; i < andNums; i++) {
        if ((model->ands + i)->lhs < (model->ands + i - 1)->lhs) {
            std::cerr << "Error: Aag file with and gate reverse order!" << std::endl;
            exit(ERROR_CODE_REVERSE_ORDER);
        }

        if ((model->ands + i)->lhs - (model->ands + i - 1)->lhs != 2) {
            std::cerr << "Error: Aag file with and gates not compact!" << std::endl;
            exit(ERROR_CODE_UNCOMPACT);
        }
    }
}

void checkAigerOutputs(aiger* model) {
    for (int i = 1; i <= outputNums; i++) {
        // 输出下标小于输入，不合理
        if ((model->outputs + (i - 1))->lit < 2 * (inputNums + 1)) {
            std::cerr << "Error: Aag file with abnormal output lit!" << std::endl;
            exit(ERROR_CODE_ABNORMAL_OUTPUT_LIT);
        }
    }
}