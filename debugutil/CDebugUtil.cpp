/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#include <stdio.h>
#include <pthread.h>
#include <execinfo.h>
#include "CDebugUtil.h"

// public member method.
void CDebugUtil::handleAssert(
    const char *file,
    const char *func,
    int line,
    const char *cond)
{
    printLog(file, func, line, cond);
    suspendIfDebug();
}

// private member method.
void CDebugUtil::printLog(
    const char *file,
    const char *func,
    int line,
    const char *cond)
{
    fprintf(stderr, "\033[31m");
    fprintf(stderr, "Handle Assert: assert(%s) %s %s() line:%d\n", cond, file, func, line);
    static const int kTraceBufSize = 20;
    void *traceBuf[kTraceBufSize];
    int traceSize = backtrace(traceBuf, kTraceBufSize);
    static const int kStderrFd = 2;
    backtrace_symbols_fd(traceBuf, traceSize, kStderrFd);
    fprintf(stderr, "\033[0m");
    fflush(stderr);
}

void CDebugUtil::suspendIfDebug()
{
#ifdef NDEBUG
    return;
#else
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);

    pthread_cond_t cond;
    pthread_cond_init(&cond, NULL);
    pthread_cond_wait(&cond, &mutex);
#endif
}

/***********************/
/* CDebugUtil::CLogger */
/***********************/
CDebugUtil::CLogger::CLogger(
    const char *file,
    const char *func,
    int line)
: fFile(file)
, fFunc(func)
, fLine(line)
{
    printf("ENTER %s %s() Line:%d\n", fFile.c_str(), fFunc.c_str(), fLine);
}

CDebugUtil::CLogger::~CLogger()
{
    printf("EXIT  %s %s() Line:%d\n", fFile.c_str(), fFunc.c_str(), fLine);
    fflush(stdout);
}

