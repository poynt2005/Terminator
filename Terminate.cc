#include "Terminater.h"
#include "Process.h"
#include <regex>
#include <string>
#include <vector>
#include <string.h>

int SearchProcessKeywords(const char *keywords, ProcessInfoResult **result, int *resultCount)
{
    std::vector<ProcInfo> infos;
    std::vector<ProcInfo> filterInfos;

    auto splitStr = [](const auto &str, const auto &delimStr, auto &vec) -> decltype(auto)
    {
        auto delim = std::regex(delimStr);
        auto splitStart = std::sregex_token_iterator(str.begin(), str.end(), delim, -1);
        auto splitEnd = std::sregex_token_iterator();
        vec = std::move(std::vector<std::string>(splitStart, splitEnd));
    };

    if (ListAllProcess(infos))
    {
        std::vector<std::string> keywordsVec;
        splitStr(std::string(keywords), ",", keywordsVec);
        TargetProcessFilter filter{false, ProcessType::exe, std::move(keywordsVec)};
        FilterProcessType(infos, filterInfos, filter);

        *resultCount = filterInfos.size();
        *result = new ProcessInfoResult[*resultCount];

        for (int i = 0; i < *resultCount; i++)
        {
            ProcessInfoResult pResult;
            const auto &&[pid, moduleNames] = std::move(filterInfos[i]);

            pResult.pid = pid;
            pResult.moduleCount = moduleNames.size();
            pResult.keywords = new char *[pResult.moduleCount];

            for (int j = 0; j < pResult.moduleCount; j++)
            {
                pResult.keywords[j] = new char[moduleNames[j].length() + 1];
                strcpy(pResult.keywords[j], moduleNames[j].data());
            }
            (*result)[i] = pResult;
        }

        return 1;
    }

    return 0;
}

int TerminateProcessByPid(const unsigned long pid)
{
    return static_cast<int>(TerminateProcess(pid));
}

void ReleaseProcessInfoResult(ProcessInfoResult **toRelease, const int resultCount)
{
    for (int i = 0; i < resultCount; i++)
    {
        for (int j = 0; j < (*toRelease)[i].moduleCount; j++)
        {
            delete[] (((*toRelease)[i]).keywords[j]);
        }
        delete (((*toRelease)[i]).keywords);
        (*toRelease)[i].keywords = nullptr;
    }
    delete[] (*toRelease);
    *toRelease = nullptr;
}
