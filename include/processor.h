#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <iostream>
#include <string>
#include <vector>
#include "linux_parser.h"

class Processor {
 public:
  double Utilization();

 private:
  long prevJiffies_ = LinuxParser::Jiffies();
  long prevActiveJiffies_ = LinuxParser::ActiveJiffies();
};

#endif