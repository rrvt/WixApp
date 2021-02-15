// Utilities


#pragma once
class Date;


String addTab(   String& s, int max);   // Add spaces out to max
String addSepTab(String& s, int max);   // Add comma to non-null string followed by spaces out to max

String normalizeDate(TCchar* date);     // Produce either empty string or mm/dd/yy date
String normalizeTime(TCchar* time);     // Produce either empty string or hh:mm

String nextTok(String& s, int& pos);    // return next token in string after pos delimited by a comma
                                        // or end of string, modify pos to beginning of next token

String getDateNow();                    // Get today's date in mm/dd/yy (normalized) form
String getTimeNow();                    // Get the time now in hh:mm (normalized) form

time_t getDiff(Date& dtUpper, Date& dtLower);
inline double toHours(time_t seconds) {return seconds/3600.0;}

inline void getMaxLng(String& s, int& max) {int lng = s.length();  if (lng > max) max = lng;}

void    floatingOnly(CEdit& ctrl);

String& removeQuotes(String& s);

