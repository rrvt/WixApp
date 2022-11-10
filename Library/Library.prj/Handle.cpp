// Handle


#include "pch.h"
#include "Handle.h"



void Handle::Close (void) {
  if (m_h != NULL && m_h != INVALID_HANDLE_VALUE) CloseHandle(m_h);

  m_h = NULL;
  }

