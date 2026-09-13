// List of printers on current PC -- uses EnumPrinters


#include "pch.h"
#include "PrinterList.h"
#include <winspool.h>


void PrinterList::get() {
DWORD bytesNeeded  = 0;
DWORD printerCount = 0;
BOOL  bSuccess;
Byte* pBuffer;
NewArray(Byte);

  data.clear();

  // Use PRINTER_INFO_4 for speed and efficiency
                                            // First call determines the required buffer size
  ::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, 0, 4, 0,
                                                                   0, &bytesNeeded, &printerCount);
  if (bytesNeeded == 0) return;

  pBuffer = AllocArray(bytesNeeded);

  // Second call retrieves the actual printer data

  bSuccess = ::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, 0, 4, pBuffer,
                                                         bytesNeeded, &bytesNeeded, &printerCount);
  if (bSuccess) {
    PRINTER_INFO_4* pPrinterInfo = reinterpret_cast<PRINTER_INFO_4*> (pBuffer);

    for (DWORD i = 0; i < printerCount; ++i) data.nextData() = pPrinterInfo[i].pPrinterName;
    }

  FreeArray(pBuffer);
  }


