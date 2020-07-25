#ifndef FORMAT_H
#define FORMAT_H

#include <string>

std::string FormatHelper(int num);

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
};                                    // namespace Format


#endif