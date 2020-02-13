// Handle Class


#pragma once
#ifdef Win2K
#include <atlbase.h>
#endif


class Handle {

HANDLE m_h;

public:

  Handle(void) : m_h (NULL) {}
  Handle(HANDLE p_h) : m_h (p_h) {}

 ~Handle(void) {Close();}

  Handle& operator= (HANDLE p_h) {Close(); m_h = p_h; return *this;}

  LPHANDLE operator& (void) {Close (); return &m_h;}

  void Close (void);

  operator HANDLE() const {return m_h;}

private:

  // Disallow copy constructor and assignment operator.

  Handle(const CHandle& rhs);
  Handle& operator= (const CHandle& rhs);
  };

