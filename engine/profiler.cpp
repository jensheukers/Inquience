#include "Profiler.h"
#include "Debug.h"

Profiler::Profiler(const std::string& name)
    : m_Name(name), m_StartTime(std::chrono::high_resolution_clock::now())
{
}

Profiler::~Profiler()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime).count();
    Debug::Log(m_Name + ": " + std::to_string(duration) + "us");
}