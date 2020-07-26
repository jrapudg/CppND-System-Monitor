#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> jiffies = LinuxParser::CpuUtilization();
    return 0.0; 
}