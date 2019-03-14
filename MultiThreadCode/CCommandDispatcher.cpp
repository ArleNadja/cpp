/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#include <iostream>
#include <assert.h>
#include "CSynchronized.h"
#include "CCommandDispatcher.h"

// constructor and destructor
CCommandDispatcher::CCommandDispatcher()
: fError(true)
, fHasTerminated(false)
, fQueueSync(0)
, fQueue(0)
, fThread(0)
{
    fQueueSync = CSynchronizer::createInstance();
    if (fQueueSync == 0) {
        ::std::cout << 
            "Failed CSynchronizer::createInstance()" << ::std::endl;
        assert(false);
        return;
    }

    fQueue = new CQueue(fQueueSync);

    fThread = CThread::createInstance(this);
    if (fThread == 0) {
        ::std::cout << 
            "Failed CThread::createInstance()" << ::std::endl;
        assert(false);
        return;
    }
    if (! fThread->start()) {
        ::std::cout << "Failed CThread::start()" << ::std::endl;
        assert(false);
        return;
    }

    fError = false;
}

CCommandDispatcher::~CCommandDispatcher()
{
    fQueue->pushBack(new CTerminateCommand(fHasTerminated));
    fThread->join();

    delete fThread;
    delete fQueue;
    delete fQueueSync;
}

// public member methods.
CCommandDispatcher *CCommandDispatcher::createInstance()
{
    CCommandDispatcher *instance = new CCommandDispatcher();
    if (instance->hasErrorOccurred()) {
        delete instance;
        return 0;
    }
    return instance;
}

void CCommandDispatcher::pushBack(
    ICommand *command)
{
    fQueue->pushBack(command);
}

void CCommandDispatcher::waitForAllCommandsExecuted()
{
    fQueue->waitForEmpty();
}

// IRunnable's method.
void CCommandDispatcher::run()
{
    while (! fHasTerminated) {
        ICommand *command = fQueue->getFront();
        command->execute();
        fQueue->removeFront();
    }
}

// private member method.
bool CCommandDispatcher::hasErrorOccurred()
{
    return fError;
}

/*****************************************/
/* CCommandDispatcher::CTerminateCommand */
/*****************************************/
// constructor and destructor
CCommandDispatcher::CTerminateCommand::CTerminateCommand(
    bool &hasTerminated)
: fHasTerminated(hasTerminated)
{
    // Nothing to do.
}

// public member methods.
// ICommand's method.
void CCommandDispatcher::CTerminateCommand::execute()
{
    fHasTerminated = true;
}

/******************************/
/* CCommandDispatcher::CQueue */
/******************************/
// constructor and destructor
CCommandDispatcher::CQueue::CQueue(
    CSynchronizer *sync)
: fSync(sync)
, fCommands()
{
    // Nothing to do.
}

CCommandDispatcher::CQueue::~CQueue()
{
    assert(fCommands.empty());
}

// public member methods.
ICommand *CCommandDispatcher::CQueue::getFront() 
{
    CSynchronized sync(fSync);
    while (fCommands.empty()) {
        sync.wait();
    }
    return fCommands.front();
}

void CCommandDispatcher::CQueue::removeFront()
{
    CSynchronized sync(fSync);
    if  (fCommands.empty()) {
        ::std::cout << "Failed removeFront() [Queue is empty]" << ::std::endl;
        assert(false);
        return;
    }
    ICommand *command = fCommands.front();
    fCommands.pop_front();
    delete command;
    sync.notifyAll();
}

void CCommandDispatcher::CQueue::pushBack(
    ICommand *command)
{
    CSynchronized sync(fSync);
    fCommands.push_back(command);
    sync.notifyAll();
}

void CCommandDispatcher::CQueue::waitForEmpty()
{
    CSynchronized sync(fSync);
    while (! fCommands.empty()) {
        sync.wait();
    }
}

