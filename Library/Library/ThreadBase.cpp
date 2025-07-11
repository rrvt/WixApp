// Thread Base -- Useful for starting independent worker threads


#include "pch.h"
#include "ThreadBase.h"



atomic<bool> aThreadFlag;


void ThreadBase::start(AThreadFunction& fn, void* param) {

  myThread = thread(
    [this, fn, param]() {
      running = true; aThreadFlag = false;
        result = fn(param);
      running = false;
      if (hndl) PostMessage(hndl, msgID, (WPARAM)result, aThreadFlag);
      }
    );

  myThread.detach();
  }



void ThreadBase::join() {try {myThread.join();} catch(...){};}


void ThreadBase::stop() {if (!running) return;   aThreadFlag = true;   join();}



