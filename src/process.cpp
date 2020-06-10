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
    long int total_time = processInfo[processInfoElem::utime_] + processInfo[processInfoElem::stime_];
    total_time += processInfo[processInfoElem::cutime_] + processInfo[processInfoElem::cstime_];
    const float seconds = (float) Process::UpTime();
    const float cpuUsage = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
    return cpuUsage; 
}


string Process::Command() { 
    if (command_.length() == 0)
        command_ = LinuxParser::Command(pid_);
    return command_; 
}


string Process::Ram() { 
    return LinuxParser::Ram(pid_);
}


string Process::User() { 
    if (user_.length() == 0)
        user_ = LinuxParser::User(pid_);
    return user_; 
}


long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); 
}


bool Process::operator>(Process const& a) const { 
    return (cpuUsage_ > a.cpuUsage_);
}
