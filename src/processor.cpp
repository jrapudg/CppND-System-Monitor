#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long jiffies = LinuxParser::Jiffies();
    long activeJiffies = LinuxParser::ActiveJiffies();

    float jiffiesDiff = (float)(jiffies - prevJiffies_);
    float activeJiffiesDiff = (float)(activeJiffies - prevActiveJiffies_);

    prevJiffies_ = jiffies;
    prevActiveJiffies_= activeJiffies;
    
    if (jiffiesDiff <= 0.001){
      return 0.0f;
    }
    else{
      return activeJiffiesDiff / jiffiesDiff;
    }
}