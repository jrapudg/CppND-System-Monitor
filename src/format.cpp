#include <string>

#include "format.h"

using std::string;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string FormatHelper(int num){
    if (num < 10)
        return "0" + std::to_string(num);
    else
        return std::to_string(num);
}

string Format::ElapsedTime(long seconds) { 
    int hours, mins, sec;
    string HH, MM, SS;

    sec = seconds % 60;
    mins = seconds / 60;
    hours = mins / 60;
    mins = mins % 60;

    HH = FormatHelper(hours);
    MM = FormatHelper(mins);
    SS = FormatHelper(sec);

    return HH + ":" + MM + ":" + SS; 
}