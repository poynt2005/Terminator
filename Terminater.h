#ifndef __TERMINATE_H__
#define __TERMINATE_H__

#ifdef BUILDTERMINATERAPI
#define EXPORTTERMINATERAPI __declspec(dllexport)
#else
#define EXPORTTERMINATERAPI __declspec(dllimport)
#endif

typedef struct _processInfoResult
{
    int moduleCount;
    char **keywords;
    unsigned long pid;
} ProcessInfoResult;

#ifdef __cplusplus
extern "C"
{
#endif
    EXPORTTERMINATERAPI int SearchProcessKeywords(const char *, ProcessInfoResult **, int *);
    EXPORTTERMINATERAPI int TerminateProcessByPid(const unsigned long);
    EXPORTTERMINATERAPI void ReleaseProcessInfoResult(ProcessInfoResult **toRelease, const int resultCount);
#ifdef __cplusplus
}
#endif

#endif