#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) : pid_(pid) {}

// Return this process's ID
int Process::Pid() { return Process::pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    long pidTotalTime = LinuxParser::ActiveJiffies(Process::Pid());
    long pidUpTime = LinuxParser::UpTime(Process::Pid());
    float Hertz = sysconf(_SC_CLK_TCK); 
    float cpuUsage = 100 * ((pidTotalTime/Hertz)/pidUpTime);
    return cpuUsage; 
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Process::Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }