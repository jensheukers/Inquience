#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <string>

class Profiler {
public:
    Profiler(const std::string& name);
    ~Profiler();
private:
    std::string m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};

#endif // PROFILER_H