// Worker Thread


#include "pch.h"
#include "WorkerThrd.h"
#include "SendMsg.h"
#include "SftpSSL.h"



struct ThreadArgs {
AFX_THREADPROC thrdFn;
void*          arg;
int            msg;
};


static ThreadArgs threadArgs;


// AFX_THREADPROC is defined to be:  UINT MyControllingFunction( LPVOID pParam );

WorkerThrd workerThrd;

static uint controllingFunction(void* args);


bool WorkerThrd::start(AFX_THREADPROC threadProc, void* arg, int msg) {
CWinThread* winThread;

  if (lock) return false;   lock = true;

  threadArgs.thrdFn = threadProc;   threadArgs.arg = arg;   threadArgs.msg = msg;

  winThread = AfxBeginThread(controllingFunction, (void*)&threadArgs);

  lock = false;   return winThread != 0;
  }



// Call a function with one arbitrary argument, post a message and stop ssl threads

uint controllingFunction(void* args) {
ThreadArgs& threadArgs = *(ThreadArgs*) args;
uint        rslt;

  try {(*threadArgs.thrdFn)(threadArgs.arg);   rslt = TE_Normal;}
  catch (...) {                                rslt = TE_Exception;}

  sendMsg(threadArgs.msg, rslt, 0);   sftpSSL.openSSLThreadStop();   return 0;
  }



