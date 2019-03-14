#include <iostream>
#include <string>
#include <unistd.h>
#include <assert.h>
#include "CCommandDispatcher.h"
#include "ICommand.h"

/**
 * コンストラクタの引数に渡したlogを1秒おきにsec秒間表示する。
 */
class CCommand:
    public ICommand
{
public:
    CCommand(
        const char *log,
        unsigned int sec)
    : fLog(log)
    , fSeconds(sec)
    {
        // Nothing to do.
    }

    // ICommand's method.
    virtual void execute()
    {
        ::std::cout << "---- ENTER CCommand::execute() ----" << ::std::endl;
        for (unsigned int i = 0; i < fSeconds; i++) {
            ::std::cout << fLog << ::std::endl;
            sleep(1);
        }
        ::std::cout << "---- EXIT  CCommand::execute() ----" << ::std::endl;
    }

private:
    ::std::string fLog;
    unsigned int fSeconds;

};

int main()
{
    CCommandDispatcher *dispatcher = CCommandDispatcher::createInstance();
    assert(dispatcher != 0);

    dispatcher->pushBack(new CCommand("1st", 5));
    dispatcher->pushBack(new CCommand("2nd", 3));
    dispatcher->pushBack(new CCommand("3rd", 7));
    
    ::std::cout << "Before waitForAllCommandsExecuted() call." << ::std::endl;
    dispatcher->waitForAllCommandsExecuted();
    ::std::cout << "After  waitForAllCommandsExecuted() call." << ::std::endl;

    dispatcher->pushBack(new CCommand("4th", 2));
    dispatcher->pushBack(new CCommand("5th", 5));
    dispatcher->pushBack(new CCommand("6th", 3));

    for (int i = 0; i < 10; i++) {
        ::std::cout << "Main thread" << ::std::endl;
        sleep(1);
    }

    delete dispatcher;

    return 0;
}
