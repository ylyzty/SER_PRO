#include <iostream>
#include "../headers/logicMaskSAT.h"
#include "../../minisat/core/Solver.h"

extern aiger* circuitModel;

int main() {
    // 标准输出 和 错误输出重定向
    std::string redirectFilename = "logs/adder.txt";
    freopen(redirectFilename.c_str(), "a", stdout);
    freopen(redirectFilename.c_str(), "a", stderr);

    std::string aagFilename = "aigers/adder.aag";
    readAagFile(aagFilename.c_str());

    return 0;
}