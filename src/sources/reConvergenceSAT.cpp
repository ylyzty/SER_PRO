/*******************************************
 * Created by zyuli on 2024/6/24 
 *******************************************/
#include "../headers/reConvergenceSAT.h"
#include <vector>
#include <map>

/* ======== Global variables ======= */
ReConvergencePath** reConvergenceMap;
bool* isVisited;
std::vector<unsigned int>* curPath;

/* ========== ReConvergence ==========  */
std::vector<unsigned int> reConvergencePath1;
std::vector<unsigned int> reConvergencePath2;
std::set<unsigned int>* associatedInputSet1;
std::set<unsigned int>* associatedInputSet2;
std::set<unsigned int>* unionSet;

/* ========== Maps ========== */
std::map<int, unsigned int> indexToAndLit;
std::map<unsigned int, int> andLitToIndex;
std::map<int, double> andNodeIndexToSAT;

/**
 * 重汇聚分析调用入口
 */
void reConvergenceSATEntry() {
    reConvergenceInit();
    createReConvergenceMap();

    for (int i = 0; i < circuitModel->num_ands; i++) {
        // 计算每个节点的 所有路径重汇聚频率总和
        getAndGateReConvergenceSATNum(indexToAndLit[i]);
    }
    printMap(andNodeIndexToSAT);

    releaseReConvergenceSATMem();
}

void reConvergenceInit() {
    // 初始化 reConvergenceMap;
    unsigned int andNums = circuitModel->num_ands;
    reConvergenceMap = new ReConvergencePath*[andNums];
    for (int i = 0; i < andNums; i++) {
        reConvergenceMap[i] = new ReConvergencePath[andNums];
        for (int j = 0; j < andNums; j++) {
            reConvergenceMap[i][j].pathToAnds.clear();
            reConvergenceMap[i][j].pathNums = 0;
        }
    }

    // 初始化 isVisited;
    isVisited = new bool[andNums];
    for (int i = 0; i < andNums; i++) {
        isVisited[i] = false;
    }

    // 初始化 curPath;
    curPath = new std::vector<unsigned int>();
    curPath->clear();

    // 初始化 reConvergence variables
    associatedInputSet1 = new std::set<unsigned int>();
    associatedInputSet2 = new std::set<unsigned int>();
    unionSet = new std::set<unsigned int>();

    // 定义 AndLit 的索引
    for (int i = 0; i < andNums; i++) {
        unsigned int andLit = (circuitModel->ands + i)->lhs;

        indexToAndLit[i] = andLit;
        andLitToIndex[andLit] = i;
    }
}

/**
 * 1. 从 输入 开始，DFS遍历
 * 2. 如果输入连接的与门访问过，则跳过；
 * 3. 如果输入连接的与门没被访问过，则调用 createRCMDfs 方法
 */
void createReConvergenceMap() {
    for (int i = 0; i < circuitModel->num_ands; i++) {
        aiger_and* curAnd = circuitModel->ands + i;

        // 判断与门是否与输入节点直接相连
        if (isConnectedInputs(curAnd->rhs0, curAnd->rhs1, circuitModel->num_inputs) && !isVisited[i]) {
            createRCMDfs(curAnd->lhs);
        }
        isVisited[i] = true;
    }

    std::cout << "Finish create reConvergence map..." << std::endl;
 }


void createRCMDfs(unsigned int andLit) {
    curPath->push_back(andLit);
    int endIndex = andLitToIndex[andLit];

    if (curPath->size() >= 2) {
        for (int i = 0; i < curPath->size() - 1; i++) {
            int startIndex = andLitToIndex[curPath->at(i)];
            if (!isVisited[startIndex]) {
                AigerPath tmpPath;
                tmpPath.path.assign(curPath->begin() + i, curPath->end());
                tmpPath.pathNo = 0;
                reConvergenceMap[startIndex][endIndex].pathToAnds.push_back(tmpPath);
                reConvergenceMap[startIndex][endIndex].pathNums += 1;
            }
        }
    }

    for (unsigned int nextAndLit : fanoutGraph[circuitModel->num_inputs + endIndex].fanouts) {
        createRCMDfs(nextAndLit);
    }

    isVisited[andLitToIndex[andLit]] = true;
    curPath->pop_back();
}

int getNotGateCount(const std::vector<unsigned int> &path) {
    int cnt = 0;
    for (unsigned int i = 0; i < path.size() - 1; i++) {
        unsigned int curLhs = (circuitModel->ands + andLitToIndex[toEven(path.at(i))])->lhs;
        unsigned int nextRhs0 = (circuitModel->ands + andLitToIndex[toEven(path.at(i + 1))])->rhs0;
        unsigned int nextRhs1 = (circuitModel->ands + andLitToIndex[toEven(path.at(i + 1))])->rhs1;

        if (curLhs - nextRhs0 == -1 || curLhs - nextRhs1 == -1) {
            cnt += 1;
        }
    }

    return cnt;
}

void addPathConstraint(const std::vector<unsigned int> &path) {
    int slow = 0;
    int fast = 1;
    unsigned int rhs0, rhs1;
    unsigned int cur, lhs;

    while (fast < path.size()) {
        cur = path[slow];
        lhs = path[fast];
        aiger_and *curAnd = circuitModel->ands + (toEven(lhs) / 2 - (circuitModel->num_inputs) - 1);
        if (toEven(cur) == toEven(curAnd->rhs0)) {
            unary(import(aigState, curAnd->rhs1), false);
        }
        else {
            unary(import(aigState, curAnd->rhs0), false);
        }

        slow += 1;
        fast += 1;
    }
}

double getAndGateReConvergenceSATNum(unsigned int andLit) {
    double satSum = 0;
    double satNum = 0;

    int startIndex = andLitToIndex[andLit];
    for (int endIndex = 0; endIndex < circuitModel->num_ands; endIndex++) {
        // 重汇聚的路径数大于等于2
        int reConvergencePathNums = reConvergenceMap[startIndex][endIndex].pathNums;
        if (reConvergencePathNums >= 2) {
            // 重汇聚路径之间两两计算
            for (int i = 0; i < reConvergencePathNums - 1; i++) {
                for (int j = i + 1; j < reConvergencePathNums; j++) {
                    reConvergencePath1.clear();
                    reConvergencePath2.clear();
                    associatedInputSet1->clear();
                    associatedInputSet2->clear();
                    unionSet->clear();

                    reConvergencePath1 = (reConvergenceMap[startIndex][endIndex].pathToAnds.at(i).path);
                    reConvergencePath2 = (reConvergenceMap[startIndex][endIndex].pathToAnds.at(j).path);

                    // 如果两条路径上非门个数的奇偶性不一致，则跳过这组重汇聚路径
                    if ((getNotGateCount(reConvergencePath1) + getNotGateCount(reConvergencePath2)) % 2 != 0) {
                        continue;
                    }

                    //
                    reConvergencePath1.pop_back();
                    reConvergencePath2.pop_back();
                    getPathAssociatedInputs(reConvergencePath1, associatedInputSet1);
                    getPathAssociatedInputs(reConvergencePath2, associatedInputSet2);
                    setUnion(associatedInputSet1, associatedInputSet2, unionSet);

                    refreshSolver();
                    satNum = getReConvergencePathSATNum(reConvergencePath1, reConvergencePath2, unionSet);
                    satNum = satNum * pow(2, circuitModel->num_inputs - unionSet->size());
                    satSum += satNum;
                }
            }
        }
    }

    andNodeIndexToSAT[startIndex] = satSum;
}

double getReConvergencePathSATNum(const std::vector<unsigned int>& path1,
                                  const std::vector<unsigned int>& path2,
                                  std::set<unsigned int>* inputSet) {
    double satNum = 0;

    addPathConstraint(path1);
    addPathConstraint(path2);

    bool sat = solver->solve();
    Minisat::vec<Minisat::Lit> tmpLits;
    while (sat) {
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

                // 存在未知解, 需跳出 SAT 求解器
                sat = false;
                break;
            }
            else {
                std::cerr << "ERROR: Abnormal SAT solve!" << std::endl;
                exit(ERROR_CODE_ABNORMAL_SAT_SOLVE);
            }
        }

        // 存在未知解或者无可满足解，跳出 while 循环，不继续求解
        if (!sat) {
            break;
        }
        else {
            // 求出可满足解，则屏蔽该可满足解，继续求解
            satNum += 1;
            solver->addClause(tmpLits);
            sat = solver->solve();
        }
    }

    return satNum;
}

void releaseReConvergenceSATMem() {
    // TODO: release memory
    delete associatedInputSet1;
    delete associatedInputSet2;
    delete unionSet;
}