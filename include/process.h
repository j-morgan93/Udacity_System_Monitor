#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, string user, string cmd, float cpuutil, string ram,
          long int uptime)
      : pid_(pid),
        user_(user),
        cmd_(cmd),
        cpu_util_(cpuutil),
        ram_(ram),
        uptime_(uptime) {}
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process & a) ;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  string user_;
  string cmd_;
  float cpu_util_;
  string ram_;
  long int uptime_;
};

#endif