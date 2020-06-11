#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(const int pid) : pid_(pid), cpuUsage_(CpuUtilization()) {}


int Process::Pid() { return pid_; }


float Process::CpuUtilization() { 
    const std::vector<long int> processInfo = LinuxParser::Cpu(pid_);
    long int total_t;
    float seconds, cpuUsage;
    total_t= processInfo[processInfoElem::utime_] + processInfo[processInfoElem::stime_]+processInfo[processInfoElem::cutime_] + processInfo[processInfoElem::cstime_];
    seconds = (float) Process::UpTime();
    cpuUsage = ((total_t / sysconf(_SC_CLK_TCK)) / seconds);
    return cpuUsage; 
}


string Process::Command() { 
   
    return LinuxParser::Command(pid_); 
}


string Process::Ram() { 
    return LinuxParser::Ram(pid_);
}


string Process::User() { 
   
    return LinuxParser::User(pid_); 
}


long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); 
}


bool Process::operator>(Process const& a) const { 
    return (cpuUsage_ > a.cpuUsage_);
}
