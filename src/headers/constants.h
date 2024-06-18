/*******************************************
 * Created by zyuli on 2024/4/16 
 *******************************************/

#ifndef SER_PRO_CONSTANTS_H
#define SER_PRO_CONSTANTS_H

#define ERROR_CODE_ABNORMAL_COMMAND_PARAMETERS     1001
#define ERROR_CODE_UNCOMPACT                       1002
#define ERROR_CODE_REVERSE_ORDER                   1003
#define ERROR_CODE_ABNORMAL_OUTPUT_LIT             1004
#define ERROR_CODE_ABNORMAL_CLAUSES_LENGTH         1005
#define ERROR_CODE_ABNORMAL_LIT                    1006
#define ERROR_CODE_ABNORMAL_SAT_SOLVE              1007
#define ERROR_CODE_ABNORMAL_RHS_LIT                1008
#define ERROR_CODE_PATH_NUM_TOO_LARGE              1009

# define INPUT_NODE_ENCODE                         0
# define AND_NODE_ENCODE                           1
# define OUTPUT_NODE_ENCODE                        2
# define INPUT_NODE_ONE_HOT_ENCODE                 "000"
# define AND_NODE_ONE_HOT_ENCODE                   "010"
# define OUTPUT_NODE_ONE_HOT_ENCODE                "100"

#endif //SER_PRO_CONSTANTS_H