// LexT virtual Output class


#pragma once


struct LexTOut {

               LexTOut() {}
  virtual     ~LexTOut() {}
  virtual void set(void* p) {}
  virtual void initialize() {}
  virtual void operator()(TCchar* fmt, ...);
  };

