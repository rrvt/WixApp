// CSV records, one record per line


#include "stdafx.h"
#include "CSVrecords.h"


CSVfield* CSVrecords::getDatum() {
  if (!lineInit) {records[i].startStore(); lineInit = true;}

  return records[i].nextDatum();
  }

