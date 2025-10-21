// Indepedent executing task


/*
// Typical Task that expects commands

Task1 task1;            // Place task object somewhere


void Task1::worker() {
  thDisplay(_T("Task1: starting\n"));

  isRunning.set();                                // Set state of thread,

  loop {

    switch (recv()) {                           // Wait for command
      case NilCmd   : thDisplay(_T("Task1 NilCmd\n"));         continue;
      case OneCmd   : thDisplay(_T("Task1 One Command\n"));    continue;
      case TwoCmd   : thDisplay(_T("Task1 Two command\n"));    continue;
      case ThreeCmd : thDisplay(_T("Task1 Three Command\n"));  continue;
      case EndCmd   :                                          break;;
      }
    break;
    }
  }
*/

#include "pch.h"
#include "Task.h"


void Task::send(int cmd) {cmdLck.own();   command = cmd;   cmdLck.release();   signal.send();}


int Task::recv(int waitTime) {
int cmd;

  signal.wait();   cmdLck.own();   cmd  = command;   command = 0;   cmdLck.release();   return cmd;
  }



/////////////------------

#if 0
Task2 task2;


void Task2::worker() {
  thDisplay(_T("Task2: starting"));

  loop {

    switch (getCmd(2000)) {
      case NilCmd : x++;  if (x >= EndCmd) x = 1;   task1.setCmd(x);
                    thDisplay(_T("Task2 Sent: %i"), x);   continue;
      case EndCmd : break;
      default     : break;
      }
    break;
    }

  thDisplay(_T("Task2 Terminating"));
  }
#if 0
  condVar.notify_one();
#endif
#endif
#if 0
unique_lock<mutex> lock(mtx);
  if (!waitTime) condVar.wait(lock);
  else           condVar.wait_for(lock, chrono::milliseconds(waitTime));
#endif

