

#include "pch.h"
#include "Signal.h"


void Signal::wait(int waitTime) {

  if (!waitTime) condVar.wait(lock);
  else           condVar.wait_for(lock, chrono::milliseconds(waitTime));
  }
