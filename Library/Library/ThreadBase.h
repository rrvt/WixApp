// Thread Base -- Useful for starting independent worker threads


#pragma once
#include <atomic>
#include <thread>


/*
The threadObj is defined:

  int WINAPI threadFn(void* param);

  class ThreadObj : public ThreadBase {
    ...
    void start(int param) {ThreadBase::start(threadFn, (void*) param);}
    void stop()           {ThreadBase::stop();}
    }

During initialization of the app:

  threadObj.set(hWnd, UserMsg);         // Window handle to which message is sent

When the thread is completed it should send a message (userMsg) as set below before starting the
thread

The message is caught in the Message Handling block:
  ON_MESSAGE(UserMsg,  &onUserMsg)

The UserMsg function is defined:
  afx_msg LRESULT onUserMsg(WPARAM wParam, LPARAM lParam);

The easiest way to block waiting for a thread object to finish is to create a dialog box, start it
with DoModule(), and install a message handler in the dialog box class that performs an OnOK()
function.
*/


typedef int WINAPI AThreadFunction(void* param);

extern atomic<bool> aThreadFlag;


class ThreadBase {

thread myThread;
HWND   hndl;
bool   running;
int    result;
int    msgID;

public:

  ThreadBase() : hndl(0), running(false), result(0) { }
 ~ThreadBase() { }

  void set(HWND hwnd, int userMsg) {hndl = hwnd;   msgID = userMsg;}

  bool isRunning()  {return running;}       // Call from app to determine if thread running

  void start(AThreadFunction& fn, void* param);

  bool isJoinable() {return myThread.joinable();}

  void stop();                              // Sets flag to stop thread and waits until it stops
                                            // only if joinable (see isJoinable)
private:

  void join();
  };


