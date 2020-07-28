#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

using std::vector;
using std::string;

// Return the aggregate CPU utilization
double Processor::Utilization() {
  long jiffies = LinuxParser::Jiffies();
  long activeJiffies = LinuxParser::ActiveJiffies();
  // Calculate difference of previous and current jiffies and activeJiffies
  double jiffiesDiff = (jiffies - prevJiffies_);
  double activeJiffiesDiff = (activeJiffies - prevActiveJiffies_);
  // Update prevJiffies and prevActiveJiffies
  prevJiffies_ = jiffies;
  prevActiveJiffies_ = activeJiffies;
  // Avoid division by zero
  if (jiffiesDiff > 0.001) {
    return activeJiffiesDiff /
           jiffiesDiff;  // Calculate CPU Utilization percentage
  } else {
    return 0.0f;
  }
}