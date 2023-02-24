#include "Process.h"
#include <iostream>
#include <windows.h>
#include <memory>
#include <filesystem>
#include <psapi.h>
#include <algorithm>
#include <regex>

bool EnablePrivilege()
{
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hToken;
    LUID luid;
    int bRet = 0;

    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
        {

            TOKEN_PRIVILEGES tp;

            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            if (AdjustTokenPrivileges(hToken, 0, &tp, 0, NULL, NULL))
            {
                bRet = (GetLastError() == ERROR_SUCCESS);
            }
        }
    }
    return bRet;
}

bool ListAllProcess(std::vector<ProcInfo> &infos)
{
    // if (!EnablePrivilege())
    // {
    //     return false;
    // }

    auto procs = std::make_unique<DWORD[]>(1024);
    DWORD processArrayByteCount;

    if (!EnumProcesses(procs.get(), sizeof(DWORD) * 1024, &processArrayByteCount))
    {
        return false;
    }

    int activeProcCount = processArrayByteCount / sizeof(DWORD);

    auto getProcessNameByID = [](const auto &id, std::vector<std::string> &moduleNames) -> decltype(auto)
    {
        auto szProcName = std::make_unique<char[]>(MAX_PATH);

        HANDLE hProc = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            0,
            id);

        if (hProc == nullptr)
        {
            return false;
        }

        auto hMods = std::make_unique<HMODULE[]>(1024);
        DWORD moduleArrayByteCount;
        if (EnumProcessModules(hProc, hMods.get(), sizeof(HMODULE) * 1024, &moduleArrayByteCount))
        {
            const auto moduleCount = moduleArrayByteCount / sizeof(HMODULE);
            for (int i = 0; i < moduleCount; i++)
            {
                auto szModName = std::make_unique<char[]>(MAX_PATH);

                if (GetModuleFileNameEx(hProc, hMods.get()[i], szModName.get(), sizeof(szModName.get()) * MAX_PATH / sizeof(char)))
                {
                    moduleNames.emplace_back(std::string(szModName.get()));
                }
            }
        }

        CloseHandle(hProc);

        return true;
    };

    for (int i = 0; i < activeProcCount; i++)
    {
        if (procs.get()[i] != 0)
        {
            std::vector<std::string> moduleNames;

            if (getProcessNameByID(procs.get()[i], moduleNames))
            {
                ProcInfo info{procs.get()[i], std::move(moduleNames)};
                infos.emplace_back(std::move(info));
            }
        }
    }

    if (!infos.size())
    {
        return false;
    }

    return true;
}

void FilterProcessType(const std::vector<ProcInfo> &origin, std::vector<ProcInfo> &filtered, const TargetProcessFilter &filterType)
{
    auto filterModule = [&filterType](const auto &inputNames, auto &outputNames) -> decltype(auto)
    {
        using FileInfo = struct fileInfo
        {
            std::string ext;
            std::string filename;
            bool isSystem;
        };

        auto getFileExt = [](const auto &name) -> decltype(auto)
        {
            namespace fs = std::filesystem;
            FileInfo info{
                fs::path(name).extension().string(),
                fs::path(name).stem().string(),
                static_cast<bool>(name.rfind("C:\\Windows", 0) == 0)};
            return info;
        };

        auto toLowerCase = [](auto &str) -> decltype(auto)
        {
            std::transform(str.begin(), str.end(), str.begin(),
                           [](unsigned char c) -> decltype(auto)
                           { return std::tolower(c); });
        };

        std::copy_if(inputNames.begin(), inputNames.end(), std::back_inserter(outputNames), [&](const auto &name) -> decltype(auto)
                     {
                        auto [ext, filename, isSystem] = getFileExt(name);
                        toLowerCase(ext);

                         if(filterType.processType == ProcessType::exe && ext == ".dll"){
                            return false;
                         }
                         else if(filterType.processType == ProcessType::dll && ext == ".exe"){
                            return false;
                         }

                         if (filterType.isSystemProcess != isSystem)
                         {
                             return false;
                         }

                         bool keywordFound = false;
                         for (auto &&keyword : filterType.keywords)
                         {
                             if (filename.find(keyword) != std::string::npos)
                             {
                                 keywordFound = true;
                                 break;
                             }
                         }
                         if (!keywordFound)
                         {
                             return false;
                         }

                         return true; });
    };

    for (const auto &[pid, moduleNames] : origin)
    {
        std::vector<std::string> moduleNamesFiltered;

        filterModule(moduleNames, moduleNamesFiltered);

        if (moduleNamesFiltered.size() > 0)
        {
            auto procInfoFiltered = ProcInfo{pid, std::move(moduleNamesFiltered)};
            filtered.emplace_back(std::move(procInfoFiltered));
        }
    }
}
bool TerminateProcess(const unsigned long pid)
{
    auto hProc = OpenProcess(
        PROCESS_TERMINATE,
        0,
        pid);

    if (hProc != NULL)
    {
        if (TerminateProcess(hProc, 9))
        {
            CloseHandle(hProc);
            return true;
        }
    }

    return false;
}