// Indepedent executing task

/*
class Task1 : public Task {
int  x;
bool endThrd;

public:
enum TaskCmd {NilCmd, OneCmd, TwoCmd, ThreeCmd, EndCmd};

  Task1() : x(1234), endThrd(false) { }
 ~Task1() { }

  void start() {Task::start(startFn);   isRunning.wait();}
                                                // create thread by call Task::start
                                                // isRunning.wait() is used when the thread is
                                                // required to be waiting for a command immediately
  void stop() {endThrd = true;   setCmd(EndCmd);} // or any method to stop the execution of the
                                                  // thread
  void worker();                                  // Worker function for the thread...

private:

  static void startFn(Task* t) {t->worker();}     // Worker is the body of the thread which is
  };                                              // called by thread after starting thread

extern Task1 task1;                               // If it is to be a global object...
*/


#pragma once
#include "Signal.h"


class IsRunning {

atomic<bool> running;
Signal       signal;

public:

  IsRunning() : running(false) { }
 ~IsRunning() {clear();}

  void clear()       {running = false;}

  void set()         {signal.send();}
  void wait()        {signal.wait();   running = true;}
  bool operator() () {return running;}
  };


class Task {
Lock   cmdLck;                                // Protect setting and getting command value
int    command;
Signal signal;

public:

IsRunning isRunning;

  Task() : command(0) {cmdLck.release();}
 ~Task() {clear();}

          void clear() {command = 0;   isRunning.clear();}

  virtual void start() { }
  virtual void stop()  { }

  virtual void worker() { }

          void send(int cmd);
          int  recv(int waitTime = 0);          // Wait time when not zero (i.e. 0 = infinite)
                                                  // waitTime is in mSec
protected:

          void start(void (*func)(Task* t)) {thread thrd(func, this);   thrd.detach();}
  };



//////////-------------

#if 0
class Task2 : public Task {
int x;

public:

  Task2() : x(0) { }
 ~Task2() { }

  void start() {Task::start(startFn);}
  void stop()  {setCmd(EndCmd);}

  void worker();

private:

  static void startFn(Task* t) {t->worker();}
  };


extern Task2 task2;


//void startTsk1();
//void stopTsk1();
#endif

