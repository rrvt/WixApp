// Debug Module for outputing print statements in a Message Box
// rrvt 05/12/2015


#include "stdafx.h"
#include "Debug.h"


Debug dbg;


void Debug::title(String& h) {hdr = h;}
void Debug::title(Tchar*  h) {hdr = h;}


void Debug::stg(String& t) {text = t; messageBox();}
void Debug::stg(Tchar*  t) {text = t; messageBox();}


void Debug::num(String& t,    int n) {text = t; text += (long) n; messageBox();}
void Debug::num(Tchar*  t,    int n) {text = t; text += (long) n; messageBox();}
void Debug::num(String& t, double n) {text = t; text += n; messageBox();}
void Debug::num(Tchar*  t, double n) {text = t; text += n; messageBox();}

