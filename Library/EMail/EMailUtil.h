// Common constants and Utility functions


#pragma once



extern TCchar    TEolCh;
extern TCchar*   TCrlf;
extern const int Threshold;


void substitute(TCchar* txt, TCchar* forTxt, String& body);         // Substitute txt for forTxt in body
