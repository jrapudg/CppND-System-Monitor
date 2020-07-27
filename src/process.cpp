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
Process::Process(int pid) : pid_(pid) {
  Process::command_ = LinuxParser::Command(Process::pid_);
}

// Return this process's ID
int Process::Pid() const{ return Process::pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const{ 
    long pidTotalTime = LinuxParser::ActiveJiffies(this->Pid());
    long pidUpTime = LinuxParser::UpTime(this->Pid());
    float Hertz = sysconf(_SC_CLK_TCK); 
    float cpuUsage = ((pidTotalTime/Hertz)/pidUpTime);
    return cpuUsage; 
}

// Return the command that generated this process
string& Process::Command() { return Process::command_; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->Pid()); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(this->Pid()); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    // Uncomment to make comparison with respect to ram
    /*
    long currentRam = stol(LinuxParser::Ram(this->pid_));
    long otherRam = stol(LinuxParser::Ram(a.pid_));
    return currentRam < otherRam;
    */
    // Comparison by Cpu usage
    float currentCpuUsage = this->CpuUtilization();
    float otherCpuUsage = a.CpuUtilization();
    return currentCpuUsage < otherCpuUsage;
}