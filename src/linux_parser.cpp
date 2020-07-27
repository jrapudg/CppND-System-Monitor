#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
// Only uncomment line bellow when using std::filesystem funciton for pids
// #include <experimental/filesystem>
// namespace fs = std::experimental::filesystem;

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// Read and return operating system
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

// Read and return kernel
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream filestream(kProcDirectory + kVersionFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Read and return pids
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

// BONUS: Update function above to use std::filesystem
/*
vector<int> LinuxParser::Pids(){
  vector<int> pids;
  string path = kProcDirectory;
  for (const auto & entry : fs::directory_iterator(path)){
    string filename = entry.path();
    if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
    }
  }
  return pids;
}
*/

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
        else if (key == "MemFree") {
          MemFree = float(stoi(value));
          break;
        }
      }
    }
  }
  MemUsage = (MemTotal - MemFree) / MemTotal;
  return MemUsage;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string idleTime, suspendTime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (getline(filestream, line)) {
    std::istringstream linestream(line);
    if (linestream >> suspendTime >> idleTime)
      return float(stoi(suspendTime)) + float(stoi(idleTime));
  }
  return 0;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> cpuData = CpuUtilization();
  long jiffies = 0;
  for (int i = kUser_; i <= kSteal_; i++) {
    jiffies += stol(cpuData[i]);
  }
  return jiffies;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> jiffies;
  string line, jiffy;
  long totalTime{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> jiffy) {
      jiffies.push_back(jiffy);
    }
    if (jiffies.size() >= 21) {
      totalTime = stol(jiffies[13]) + stol(jiffies[14]) + stol(jiffies[15]) +
                  stol(jiffies[16]);
      return totalTime;
    }
  }
  return 0.0l;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpuData = CpuUtilization();
  long idleJiffies = 0;
  for (int i = kIdle_; i <= kIOwait_; i++) {
    idleJiffies += stol(cpuData[i]);
  }
  return idleJiffies;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> states;
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          for (int i = kUser_; i < kGuestNice_; i++) {
            linestream >> value;
            states.push_back(value);
          }
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
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") return stoi(value);
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") return stoi(value);
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    if (line == "") {
      return string();
    } else {
      return line;
    }
  }
  return string();
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          std::ostringstream ram;
          ram.precision(2);
          ram << std::fixed << stof(value) / 1000;
          return ram.str();
        }
      }
    }
  }
  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestrem(line);
      while (linestrem >> key >> value) {
        if (key == "Uid") {
          return value;
        }
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
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> splitter >> uid) {
        if (splitter == "x") {
          if (uid == processUid) return user;
        }
      }
    }
  }
  return string();
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  vector<string> jiffies;
  string line, jiffy;
  long startTime{0};
  float Hertz = sysconf(_SC_CLK_TCK);
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> jiffy) {
      jiffies.push_back(jiffy);
    }
    if (jiffies.size() >= 21) {
      startTime = stol(jiffies[21]) / Hertz;
      return LinuxParser::UpTime() - startTime;
    }
  }
  return 0.0l;
}