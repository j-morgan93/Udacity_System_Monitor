#include <unistd.h>
#include <cctype>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_util_ = LinuxParser::Cpuutilization(pid_); }

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    //std::cout << "ram : " << ram_ << "\n";
    return ram_ = LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return Process::user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_ = LinuxParser::Uptime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process & a) { return a.CpuUtilization() < CpuUtilization(); }