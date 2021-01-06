#include "processor.h"
#include "linux_parser.h"
#include<string>
#include<vector>
#include<iostream>

using std::stof;
using std::string;

// Return the aggregate CPU utilization
/** Taken almost verbatim from:
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux **/

float Processor::Utilization() { 
  
  std::vector<string> cpu_util = LinuxParser::CpuUtilization();
  
  float idle = stof(cpu_util[LinuxParser::kIdle_]);
  float iowait = stof(cpu_util[LinuxParser::kIOwait_]);
  float user = stof(cpu_util[LinuxParser::kUser_]);
  float nice = stof(cpu_util[LinuxParser::kNice_]);
  float system  = stof(cpu_util[LinuxParser::kSystem_]);
  float irq = stof(cpu_util[LinuxParser::kIRQ_]);
  float softirq = stof(cpu_util[LinuxParser::kSoftIRQ_]);
  float steal = stof(cpu_util[LinuxParser::kSteal_]);
  
  //std::cout << "idle : " << idle << "iowait : " << iowait << "\n";

  float PrevIdle = previdle_ + previowait_;
  float Idle = idle + iowait;
  
  float PrevNonIdle = prevuser_ + prevnice_ + prevsystem_ + previrq_ + prevsoftirq_ + prevsteal_;
  float NonIdle = user + nice + system + irq + softirq + steal;
  
  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;

// differentiate: actual value minus the previous one (not done for proj)
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;
  float percent = (totald - idled)/totald;

  previdle_ = idle;
  previowait_ = iowait;
  prevuser_ = user;
  prevnice_ = nice;
  prevsystem_ = system;
  previrq_ = irq;
  prevsoftirq_ = softirq;
  prevsteal_ = steal;
  //std::cout << "percentage is : " << percent << "\n";
  
  return percent; 
}