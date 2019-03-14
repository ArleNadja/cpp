#include "CDebugUtil.h"

void trace_test()
{
    TRACE_FUNC();
}

void bar()
{
    TRACE_FUNC();
    assert(false && "TEST ASSERT");
}

void foo()
{
    TRACE_FUNC();
    bar();
}

void hoge()
{
    TRACE_FUNC();
    foo();
}

int main()
{
    TRACE_FUNC();
    trace_test();
    hoge();
    return 0;
}
