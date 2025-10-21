// Encapsulate unique_lock which is owned after the constructor is executed.


#pragma once


class Lock {
mutex      mtx;
UniqueLock uLock;
public:

  Lock() : uLock(mtx) { }
 ~Lock() { }

  void own()     {uLock.lock();}
  void release() {uLock.unlock();}

  operator UniqueLock&() {return uLock;}
  };

