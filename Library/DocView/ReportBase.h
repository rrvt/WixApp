// Report Base


#pragma once
#include "Date.h"
#include "Devices.h"
#include "DevStream.h"
#include "Printer.h"

class CScrView;
class Device;
class NotePad;
class Archive;



class ReportBase {
protected:

NotePad&  np;
DevStream dvs;
String    title;
int       maxPages;
Date      dateModified;

bool      printing;

String    productName;
String    companyName;

public:
PrtrOrient prtrOrietn;

               ReportBase(NotePad& notePad, DeviceID id) : np(notePad), dvs(id), maxPages(0),
                                                        printing(false), prtrOrietn(PortOrient) { }
              ~ReportBase() { }

          void setTitle(TCchar* t) {title = t;}

  virtual void display(CScrView& vw);

  virtual void dspHeader(Device& dev, int pageNo = 1) { }
  virtual void dspFooter(Device& dev, int pageNo = 1) { }

  virtual void prtHeader(Device& dev, int pageNo);
  virtual void prtFooter(Device& dev, int pageNo);

  virtual void txtOut(Archive& ar, double tabFactor);

protected:

  virtual void setArchiveAttr(double f);
          void getPageAttr(CScrView& vw);

  virtual void getData() = 0;

private:

  ReportBase() : np(*(NotePad*)0), dvs(ReportID) { }
  };


