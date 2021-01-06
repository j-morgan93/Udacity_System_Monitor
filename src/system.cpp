#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    processes_.clear();
    vector<int> proclist = LinuxParser::Pids();
    string user;
    string cmd;
    float cpu_util_;
    string ram;
    long int uptime;

    for (int i : proclist) {
        user = LinuxParser::User(i);
        cpu_util_ = LinuxParser::Cpuutilization(i);
        ram = LinuxParser::Ram(i);
        uptime = LinuxParser::Uptime(i);
        cmd = LinuxParser::Command(i);
        //std::cout << "uptime is : " << uptime << "\n";
        processes_.emplace_back(i,user,cmd,cpu_util_,ram,uptime);
        //std::cout << "proc id : " << processes_[i].Pid() << "\n";
    }
    std::sort(processes_.begin(), processes_.end(),[](Process a, Process b)
    {return a.CpuUtilization() > b.CpuUtilization();});
    return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }