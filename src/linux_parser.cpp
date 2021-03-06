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
  string key;
  string value;
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
  string os, version, kernel;
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
  float totalvalue, freevalue;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    	std::istringstream linestream(line);
    	linestream >> key >> value;
      	if(key == "MemTotal:") {
          totalvalue = std::stof(value);
        }
      	if(key == "MemFree:") {
          freevalue = std::stof(value);
          break;
        }
    }
  }
  return 1 - freevalue / totalvalue;
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string value;
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    uptime = std::stol(value);
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float LinuxParser::Cpuutilization(int pid) {
  string uid = LinuxParser::Uid(pid);
  string user, key;
  float cpu_util;
  float utime, stime, starttime;
  int i = 0;
  float hertz = sysconf(_SC_CLK_TCK);
  float ttime, seconds;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        i += 1;
        switch(i) {
          case 14 :
          utime = std::stof(key);
          break;
          case 15 :
          stime = std::stof(key);
          break;
          case 22 :
          starttime = std::stof(key);
          break;
        }
      }
    }
  }    
  ttime = utime + stime;
  seconds = (float) LinuxParser::UpTime() - (starttime / hertz);
  if(seconds == 0) { 
  cpu_util = 0.0;
  } else {
  cpu_util = 100 * ((ttime / hertz) / seconds );
  }
  return cpu_util;
  }

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_detail;
  string key, val, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    if(key == "cpu" ) {
      for (int i=0; i < 10; i++){
        linestream >> val;
        cpu_detail.push_back(val);
      }
    }
  }
  return cpu_detail;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value;
  int  proc = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    	std::istringstream linestream(line);
    	linestream >> key >> value;
      	if(key == "processes") {
          proc = std::stoi(value);
        }
    }
  }
  return proc;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value;
  int  proc = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    	std::istringstream linestream(line);
    	linestream >> key >> value;
      	if(key == "procs_running") {
          proc = std::stoi(value);
        }
    }
  }
  return proc;
}

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string value;
  string ram;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    	std::istringstream linestream(line);
    	linestream >> value;
    }
  }
  return value;
  }

//Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string key, value;
  int val;
  string ram;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    	std::istringstream linestream(line);
    	linestream >> key >> value;
      if(key == "VmSize:") {
        val = std::stof(value) / 1000;
        ram = std::to_string(val);
      }
    }
  }
  return ram;
  }

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string key, value;
  string uid;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    	std::istringstream linestream(line);
    	linestream >> key >> value;
      if(key == "Uid:") {
        uid = value;
      }
    }
  }
  return uid;
  }


// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string user, key, x, value;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (uid == value) {
          user = key;
        }
      }
    }
  }        
  return user;
  }

// TODO: Read and return the uptime of a process
long LinuxParser::Uptime(int pid) {
  string uid = LinuxParser::Uid(pid);
  long int sys_uptime = LinuxParser::UpTime();
  string user, key, x, value;
  long int starttime;
  long int i = 0;
  long int hertz = sysconf(_SC_CLK_TCK);
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        i += 1;
        switch(i) {
          case 22 :
          starttime = std::stol(key);
          break;
        }
      }
    }
  }
  return sys_uptime - (starttime / hertz);
  }