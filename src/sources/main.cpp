#include <iostream>
#include "../headers/logicMaskSAT.h"

extern aiger* circuitModel;

/**
 * 命令行参数列表:
 *      0. 可执行文件名: str
 *      1. 输出重定向文件路径: string
 *      2. 读入的 aag 文件路径: string
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    // 入参校验
    if (argc != 3) {
        std::cerr << "Error: Abnormal command line parameters!" << std::endl;
        exit(ERROR_CODE_ABNORMAL_COMMAND_PARAMETERS);
    }

    // 标准输出 和 错误输出重定向, 注意: 目录不存在不会报错
    std::string redirectFilename = argv[1];
    freopen(redirectFilename.c_str(), "w", stdout);
    freopen(redirectFilename.c_str(), "a", stderr);

    std::string aagFilename = argv[2];
    checkCircuitPaths(aagFilename.c_str());

    return 0;
}