/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#ifndef _C_COMMAND_DISPATCHER_H
#define _C_COMMAND_DISPATCHER_H

#include <list>
#include "ICommand.h"
#include "IRunnable.h"
#include "CThread.h"
#include "CSynchronizer.h"

/**
 * ICommandをキューイングし、ワーカースレッドでICommandのexecute()を逐次実行するクラスです。
 * ワーカースレッド数は1です。
 * キューイングしたICommandはCCommandDispatcherがdeleteします。クライアント側でdeleteしないでください。
 */
class CCommandDispatcher:
    public IRunnable
{
private:
    CCommandDispatcher();

public:
    virtual ~CCommandDispatcher();

    /**
     * CCommandDispatcherのインスタンスを生成します。
     * インスタンスの生成に失敗すると 0 が返ります。
     */
    static CCommandDispatcher *createInstance();
    
    /**
     * ICommandをキューイングします。
     * キューイングされたICommandは逐次ワーカースレッドでexecute()呼び出しがされます。
     * キューイングしたICommandは、execute()呼び出しが終わるとCCommandDispatcherによりdeleteされます。
     */
    void pushBack(
        ICommand *command);
    
    /**
     * キューイングされているICommandのexecute()呼び出しが全て完了するのを待ち合わせます。
     */
    void waitForAllCommandsExecuted();

    // IRunnable's method.
    virtual void run();

private:
    class CTerminateCommand:
        public ICommand
    {
    public:
        CTerminateCommand(
            bool &hasTerminated);

        // ICommand's method.
        virtual void execute();

    private:
         bool &fHasTerminated;

    };

    class CQueue
    {
    public:
        CQueue(
            CSynchronizer *sync);

        virtual ~CQueue();

        ICommand *getFront();

        void removeFront();

        void pushBack(
            ICommand *command);

        void waitForEmpty();

    private:
        ::std::list<ICommand *> fCommands;
        CSynchronizer *fSync;
    };

    bool hasErrorOccurred();

    bool fError;
    bool fHasTerminated;
    CSynchronizer *fQueueSync;
    CQueue *fQueue;
    CThread *fThread;

};

#endif /* _C_COMMAND_DISPATCHER_H */
