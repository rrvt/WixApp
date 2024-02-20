// Utilities


#pragma once
class Date;


String addTab(   TCchar* tc, int tab);   // Add spaces out to max
String addSepTab(String& s,  int max);   // Add comma to non-null string followed by spaces out to max
String rightAdj( TCchar* tc, int right); // Right adjust s to right boundary
String tabStg(int n);
String normalizeDate(TCchar* date);     // Produce either empty string or mm/dd/yy date
String normalizeTime(TCchar* time);     // Produce either empty string or hh:mm

String nextTok(String& s, int& pos);    // return next token in string after pos delimited by a comma
                                        // or end of string, modify pos to beginning of next token

String getDateNow();                    // Get today's date in mm/dd/yy (normalized) form
String getTimeNow();                    // Get the time now in hh:mm (normalized) form

time_t getDiff(Date& dtUpper, Date& dtLower);
inline double toHours(time_t seconds) {return seconds/3600.0;}

inline void maxLng(String& s, int& mx) {int lng = s.length();  if (lng > mx) mx = lng;}
inline void maxLng(long   id, int& mx) {int i; for (i = 0; id; id /= 10) i++;  if (i > mx) mx = i;}

void    floatingOnly(CEdit& ctrl);

String  addQuotes(TCchar* p);       // Conditionally add quotes to protect interior quotes and commas
String& removeQuotes(String& s);

bool    isEmpty(String* s, ...);


String formatPhone(TCchar* ph, Tchar sep);



