// Worker Thread


#pragma once
#include "afxwin.h"


enum TErslt {TE_Normal, TE_Exception, TE_Unknown};


class WorkerThrd {

bool lock;

public:

  WorkerThrd() : lock(false) { }
 ~WorkerThrd() { }

  bool isLocked() {return lock;}

  bool start(AFX_THREADPROC thdProc, void* arg, int msg);
  };


extern WorkerThrd workerThrd;



