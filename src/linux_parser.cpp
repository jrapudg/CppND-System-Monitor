#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key, value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, value;
  string line;
  float MemTotal, MemFree, MemUsage;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") 
          MemTotal = float(stoi(value));
        else if (key == "MemFree"){
          MemFree = float(stoi(value));
          break;
        }
      }
    }
  }
  MemUsage = (MemTotal - MemFree)/MemTotal;
  return MemUsage; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string idleTime, suspendTime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(getline(filestream, line)){
    std::istringstream linestream(line);
    if(linestream >> suspendTime >> idleTime)
      return float(stoi(suspendTime)) + float(stoi(idleTime));
  }
  return 0; }

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> cpuData = CpuUtilization();
  long jiffies = 0;
  for (int i = kUser_; i <= kSteal_; i++){
    jiffies += stol(cpuData[i]);
  }
  return jiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpuData = CpuUtilization();
  long idleJiffies = 0;
  for (int i = kIdle_; i <= kIOwait_; i++){
    idleJiffies += stol(cpuData[i]);
  }
  return idleJiffies; 
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> states;
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    while(linestream >> key){
      if (key == "cpu"){
        for (int i = kUser_; i<kGuestNice_; i++){
          linestream >> value;
          states.push_back(value);
        }
      }
    }
  }
  return states;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "processes")
          return stoi(value);
      }
    }
  }
  return 0; }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "procs_running")
          return stoi(value);
      }
    }

  }
  return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  while(std::getline(filestream, line)){
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestrem(line);
    while (linestrem >> key >> value){
      if (key == "Uid"){
        return value;
      }
    }
  }
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string processUid = LinuxParser::Uid(pid);
  string user, splitter, uid;
  string line;
  std::ifstream filestream(kPasswordPath);
  while(std::getline(filestream, line)){
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while(linestream >> user >> splitter >> uid){
      if (splitter == "x"){
        if (uid == processUid)
          return user;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }