#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <string>
#include <vector>
#include <string>

enum class ProcessType
{
    exe,
    dll
};

using ProcInfo = struct procInfo
{
    unsigned long pid;
    std::vector<std::string> moduleNames;
};

using TargetProcessFilter = struct targetProcessFilter
{
    bool isSystemProcess;
    ProcessType processType;
    std::vector<std::string> keywords;
};

bool ListAllProcess(std::vector<ProcInfo> &);
void FilterProcessType(const std::vector<ProcInfo> &, std::vector<ProcInfo> &, const TargetProcessFilter &);
bool TerminateProcess(const unsigned long);

#endif