// CSV records, one record per line


#pragma once
#include "CSVrecord.h"


class CSVrecords {
int                       i;
bool                      lineInit;
Expandable <CSVrecord, 128> records;

public:
int maxFields;

  CSVrecords() : i(0), lineInit(false), maxFields(0) {init();}

  void      init() {i = 0; lineInit = false; maxFields = 0; records.clr();}

  void      startStore() {records.clr(); i = 0; maxFields = 0; lineInit = false;}
  void      nextStore()  {i++; lineInit = false;}
  CSVfield* getDatum();
  void      setMax()     {int n = records[i].notEmptyNo(); if (i > 0 && n > maxFields) maxFields = n;}
  int       numbr()      {return records.end();}

  void      dspRecords() {
            CSVrecord* line;
              for (line = startLoop(); line; line = nextRcd()) line->dspFields(maxFields);
              }


  CSVrecord* startLoop() {i = 0; return i < records.end() ? &records[i] : 0;}
  CSVrecord* nextRcd()   {i++;   return i < records.end() ? &records[i] : 0;}
  };
