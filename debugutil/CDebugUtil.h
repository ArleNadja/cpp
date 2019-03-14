/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#ifndef _C_DEBUG_UTIL_H
#define _C_DEBUG_UTIL_H

#include <string>

/**
 * 自作 assert マクロ。
 * assert時に、assert呼び出しされた場所の、ファイル名、関数名、行番号と、
 * stackの情報をログ出力し、assert状態でsuspendする。
 * プログラム実行プロセスにattachし、assertしたスレッドにbacktraceを確認できる。
 * $ gdb
 * (gdb) attach [PID]
 * (gdb) t a a bt
 * (gdb) t [threadID]
 * assert時のログは赤字になる。
 */
#define assert(cond) \
do { \
    if (! (cond)) { \
        CDebugUtil::handleAssert( \
            __FILE__, \
            __FUNCTION__, \
            __LINE__, \
            #cond); \
     } \
} while (0)

/**
 * 関数のIN/OUTをトレースする 自作マクロ。
 * 関数の内部で
 * TRACE_FUNC();
 * を記述すると、関数のIN/OUT時にファイル名、関数名、行番号をログ出力する。
 */
#define TRACE_FUNC() CDebugUtil::CLogger trace_logging(__FILE__, __FUNCTION__, __LINE__)

class CDebugUtil
{
public:
    static void handleAssert(
        const char *file,
        const char *func,
        int line,
        const char *cond);

    class CLogger
    {
    public:
        CLogger(
            const char *file,
            const char *func,
            int line);

        virtual ~CLogger();

    private:
        ::std::string fFile;
        ::std::string fFunc;
        int fLine;
    };

private:
    static void printLog(
        const char *file,
        const char *func,
        int line,
        const char *cond);

    static void suspendIfDebug();

};

#endif /* _C_DEBUG_UTIL_H */
