#include <iostream>
#include "../headers/logicMaskSAT.h"
#include "../../minisat/core/Solver.h"

extern aiger* circuitModel;

int main() {
    std::string filename = "../../aigers/xor1.aag";
    readAagFile(filename.c_str());

    return 0;
}


//using namespace Minisat;
//
//int main() {
//    using Minisat::Solver;
////    using Minisat::Lit;
//
//    auto* solver = new Solver;
//
//    // Variables Define
//    int A = solver->newVar();
//    int B = solver->newVar();
//    int C = solver->newVar();
//
//    // Clauses Define
//    solver->addClause(Minisat::mkLit(C, true));
//    solver->addClause(Minisat::mkLit(A, false), Minisat::mkLit(B, false), Minisat::mkLit(C, true));
//    solver->addClause(Minisat::mkLit(A, false), Minisat::mkLit(B, false), Minisat::mkLit(C, false));
//    solver->addClause(Minisat::mkLit(A, true), Minisat::mkLit(B, false), Minisat::mkLit(C, false));
//    solver->addClause(Minisat::mkLit(A, false), Minisat::mkLit(B, true), Minisat::mkLit(C, false));
//
//    bool sat = solver->solve();
//    while (sat) {
//        std::cout << "SAT model found: \n";
//        std::cout << "A: ";
//        std::cout << (solver->modelValue(A) == l_True ? "True" : "False");
//        std::cout << "\n";
//
//        std::cout << "B: ";
//        std::cout << (solver->modelValue(B) == l_True ? "True" : "False");
//        std::cout << "\n";
//
//        std::cout << "C: ";
//        std::cout << (solver->modelValue(C) == l_True ? "True" : "False");
//        std::cout << "\n";
//
//        Minisat::Lit clauseA = solver->modelValue(A) == l_True ? ~mkLit(A) : mkLit(A);
//        Minisat::Lit clauseB = solver->modelValue(B) == l_True ? ~mkLit(B) : mkLit(B);
//        Minisat::Lit clauseC = solver->modelValue(C) == l_True ? ~mkLit(C) : mkLit(C);
//        solver->addClause(clauseA, clauseB, clauseC);
//        sat = solver->solve();
//    }
//
//    return 0;
//}