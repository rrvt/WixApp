// Pipe Control of another process (application)


#include "stdafx.h"
#include "XTerm.h"
#include "match.h"


XTerm::State XTerm::state   = BeginState;
bool         XTerm::eolSeen = true;


bool XTerm::putChar(Tchar ch, XCmd& xCmd) {
ChClass chClass;

  if (state == BeginState) {
    if (ch == '\x1b')
      {xCmd.clear(); state = EscapeSeen; return false;}

    xCmd.ch = ch; return true;
    }

  if      (ch <  ' ') chClass = CtrlChar;
  else if (ch <  '0') chClass = OtherChar;
  else if (ch <= '9') chClass = Digit;
  else if (ch <  'A') chClass = OtherChar;
  else if (ch <= 'Z') chClass = Letter;
  else if (ch <  'a') chClass = OtherChar;
  else if (ch <= 'z') chClass = Letter;
  else if (ch <= 127) chClass = OtherChar;
  else                chClass = CtrlChar;

  switch (state) {

    case EscapeSeen:
      switch (chClass) {
        case Digit    : xCmd.val = ch - '0'; state = CollectInt; return false;
        case Letter   : xCmd.suffix = ch; state = BeginState; return false;
        case OtherChar: xCmd.prefix = ch; state = LeftSqBkt; return false;
        default       : xCmd.suffix = ch; state = BeginState; return false;
        }
      break;

    case LeftSqBkt:
      switch (chClass) {
        case Digit  : xCmd.val = ch - '0'; state = CollectInt; return false;
        case Letter : xCmd.suffix = ch; state = BeginState; return false;
        default     : if (ch == '.') {xCmd.suffix = ch; state = BeginState; return false;}
                      if (xCmd.prefix2) {xCmd.suffix = ch; state = BeginState; return false;}
                      xCmd.prefix2 = ch; return false;
        }

    case CollectInt:
      switch (chClass) {
        case Digit    : xCmd.val = xCmd.val * 10 + ch - '0'; return false;
        case Letter   : xCmd.suffix = ch; state = BeginState; return false;
        case OtherChar: if (ch == ';') {xCmd.suffix = ch; state = LeftSqBkt; return false;}
        default       : xCmd.suffix = ch; state = BeginState; return false;
        }
      break;

    default: xCmd.ch = ch; return true;
    }

  xCmd.ch = ch; return true;
  }

