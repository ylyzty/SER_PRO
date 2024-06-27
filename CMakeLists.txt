# CMake 最低版本要求
cmake_minimum_required(VERSION 3.27)

# 项目信息
project(SER_PRO)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)


set(DIR_SRCS
        src/sources/tools.cpp
        src/sources/aiger.c
        minisat/core/Solver.cc
        src/sources/logicMaskSAT.cpp
        src/sources/reConvergenceSAT.cpp
        src/sources/main.cpp
)


# 指定生成目标
add_executable(
    ${PROJECT_NAME} ${DIR_SRCS}
)
