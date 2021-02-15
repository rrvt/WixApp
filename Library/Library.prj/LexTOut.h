// LexT virtual Output class


#pragma once
class NotePad;


struct LexTOut {

               LexTOut() {}
  virtual     ~LexTOut() {}
  virtual void set(void* p) {}
  virtual void initialize() {}
  virtual void operator()(NotePad& np, TCchar* fmt, ...);
  };

