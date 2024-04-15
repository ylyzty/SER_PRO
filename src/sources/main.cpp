#include <iostream>
#include "../headers/logicMaskSAT.h"
#include "../../minisat/core/Solver.h"

extern aiger* circuitModel;

int main() {
    std::string filename = "../../aigers/xor1.aag";
    readAagFile(filename.c_str());

    return 0;
}