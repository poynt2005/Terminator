#include "Terminater.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <psapi.h>

int main()
{
    HANDLE handle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS counters;

    ProcessInfoResult *result = NULL;
    int resultCount;

    // GetProcessMemoryInfo(handle, &counters, sizeof(counters));
    // std::cout << counters.WorkingSetSize << '\n';

    SearchProcessKeywords("TortoiseGit", &result, &resultCount);

    for (int i = 0; i < resultCount; i++)
    {
        for (int j = 0; j < result[i].moduleCount; j++)
        {
            // printf("%s\n", result[i].keywords[j]);
        }
        // TerminateProcessByPid(result[i].pid);
    }
    // Sleep(5000);
    // ReleaseProcessInfoResult(&result, resultCount);
    std::cout << "End\n";

    system("PAUSE");
    return 0;
}