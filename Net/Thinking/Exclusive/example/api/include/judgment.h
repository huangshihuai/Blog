#ifndef API_JUDGMENT_H_
#define API_JUDGMENT_H_
#include <iostream>

namespace judgment {

/**
 * 非负数判断
 * @param src 输入数字
 * @param output_str 错误输出
 * @param command 错误执行的命令
 */
#define NonNegative(src, output_str, command) do { \
    if ((src) < 1) { \
        std::cerr << output_str << std::endl; \
        command; \
    } \
} while(false)

}

#endif // API_JUDGMENT_H__