// Signal


#pragma once
#include "Lock.h"


class Signal {

condition_variable condVar;
Lock               lock;

public:

  Signal() { }
 ~Signal() { }

  void send() {condVar.notify_one();}
  void wait(int waitTime = 0);
  };

