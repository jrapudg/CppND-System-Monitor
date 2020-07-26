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
    std::vector<std::string> prevJiffies = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};
};

#endif