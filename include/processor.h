#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include "linux_parser.h"
#include <iostream>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

 private:
    long prevJiffies_ = LinuxParser::Jiffies();
    //long prevIdle_ = LinuxParser::IdleJiffies();
    long prevActiveJiffies_ = LinuxParser::ActiveJiffies();
};

#endif