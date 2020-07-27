#ifndef FORMAT_H
#define FORMAT_H

#include <string>

std::string FormatHelper(int num);  // Helper to ensure 2 digits format

namespace Format {
std::string ElapsedTime(long times);  // Format seconds to HH::MM::SS
};                                    // namespace Format

#endif