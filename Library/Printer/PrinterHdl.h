// Use to Create a printer DC


#pragma once


class PrinterHdl : public CPrintDialog {

String printerName;
String driverName;
String portName;

public:

  PrinterHdl() : CPrintDialog(false) { }
 ~PrinterHdl();

  HDC operator() ();

private:

  bool setDevMode();
  void setDevNames();
  bool findPrinterNames();
  void setAttr(DEVMODE& devMode);
  void setName(TCchar* name, DEVMODE* devMode);
  };




