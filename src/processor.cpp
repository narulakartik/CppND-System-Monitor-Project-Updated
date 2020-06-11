#include "processor.h"


#include "linux_parser.h"

#include <vector>
#include <string>
#include <algorithm>

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> cpu_elements_string = LinuxParser::CpuUtilization();
    std::vector<float> cpu_elements_float(cpu_elements_string.size());
    std::transform(cpu_elements_string.begin(), cpu_elements_string.end(), cpu_elements_float.begin(), [](const std::string &str){
        return std::stof(str);
    });

    const float idle = IdleTime(cpu_elements_float);
    const float non_idle = NonIdleTime(cpu_elements_float);
    const float total = idle + non_idle;

  
    if (previous_cpu_elements.size() == 0 ){
        previous_cpu_elements = cpu_elements_float;
        return non_idle/total; 
    }
    
    
    const float prev_idle = IdleTime(previous_cpu_elements);
    const float prev_non_idle = NonIdleTime(previous_cpu_elements);
    const float prev_total = prev_idle + prev_non_idle;

    const float total_delta = total - prev_total;
    const float non_idle_delta = non_idle - prev_non_idle;

    previous_cpu_elements = cpu_elements_float;
    return non_idle_delta/total_delta;
}
