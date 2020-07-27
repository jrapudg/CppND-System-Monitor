#include "format.h"
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string FormatHelper(int num) {
  if (num < 10)
    return "0" + std::to_string(num);
  else
    return std::to_string(num);
}

string Format::ElapsedTime(long seconds) {
  int hours, mins, sec;
  string HH, MM, SS;
  // Calculate seconds, minutes and hours from seconds period
  sec = seconds % 60;
  mins = seconds / 60;
  hours = mins / 60;
  mins = mins % 60;
  // Change to two digits string format
  HH = FormatHelper(hours);
  MM = FormatHelper(mins);
  SS = FormatHelper(sec);
  return HH + ":" + MM + ":" + SS;
}