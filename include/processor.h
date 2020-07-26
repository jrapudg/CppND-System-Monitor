#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include "linux_parser.h"
#include <iostream>
#include <vector>

class Processor {
 public:
  float Utilization();

 private:
    long prevJiffies_ = LinuxParser::Jiffies();
    long prevActiveJiffies_ = LinuxParser::ActiveJiffies();
};

#endif