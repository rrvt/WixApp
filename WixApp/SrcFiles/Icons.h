// Icon storage -- only one in the app


#pragma once
#include "EntityStore.h"
#include "PathDesc.h"
#include "WixOut.h"
#include "WixDataDlg.h"




class IconDesc : public Data {
PathDesc pathDsc;

public:
bool     inUse;

  IconDesc();
  IconDesc(IconDesc& d) : inUse(false) {copyObj(d);}
 ~IconDesc() { }

  void clear() {id.clear(); pathDsc.clear(); inUse = false;}

  void readWixData( TCchar* section);
  void writeWixData(TCchar* section);
  void setWixID();

  void browse();

  void parse(String& fullPath);

  IconDesc& operator= (IconDesc& d) {copyObj(d); return *this;}

  bool validate();
  void outputOne(int tab);

private:

  void copyObj(IconDesc& d) {id = d.id; wixID = d.wixID; pathDsc = d.pathDsc; inUse = d.inUse;}
  };


class Icons {

typedef EntityStore<IconDesc, 1> IconList;
typedef EStoreIter< IconDesc, 1> ListIter;

IconList iconList;

public:

  Icons();
 ~Icons() { }

  //IconDesc* update(String& id, String& path);
  bool      getFromList(IconDesc& icon);
  bool      updateList(IconDesc& icon);

  void      loadCB(ComboBox& cb) {iconList.loadCB(cb);}

  void      setCur(String& s, ComboBox& cb)
                              {if (!s.isEmpty()) iconList.setCur(s, cb); else cb.SetWindowText(_T(""));}

  IconDesc* find(String& s) {return iconList.find(s);}

  bool      validate();
  void      output(int tab);

private:

  IconDesc* getNew() {return iconList.newItem();}

  void oneIconAvail();
  };


extern Icons icons;


