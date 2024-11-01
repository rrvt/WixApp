// Icon storage -- only one in the app


#pragma once
#include "EntityStore.h"
#include "RelPath.h"
#include "WixOut.h"
#include "WixAppDlg.h"




class IconDesc : public Data {
RelPath relPath;

public:

bool     inUse;

            IconDesc() : inUse(false) { }
            IconDesc(IconDesc& d) : inUse(false) {copyObj(d);}
           ~IconDesc() { }

  void      clear() {id.clear(); relPath.clear(); inUse = false;}

  bool      isValid() {return inUse && !id.isEmpty();}

  IconDesc& operator= (RelPath& rp) {relPath = rp; return *this;}

  void      readWixData( TCchar* section);
  void      writeWixData(TCchar* section);

  void      setWixID(bool isStartupApp);

  IconDesc& operator= (IconDesc& d) {copyObj(d); return *this;}

  bool      validate(bool rptErrors = true);
  void      outputOne(int tab);

  void      saveData(Archive& ar) {relPath.saveData(ar);}

private:

  void      copyObj(IconDesc& d)
                              {id = d.id; wixID = d.wixID; relPath = d.relPath; inUse = d.inUse;}
  };


class Icons {

typedef EntityStore<IconDesc, 1> IconList;
typedef EStoreIter< IconDesc, 1> ListIter;

IconList iconList;

public:

  Icons() { }
 ~Icons() { }

  void      readWixData();
  void      writeWixData();

  int       nIcons() {return iconList.data.end();}

  String    getPath(bool isStartupApp);

  bool      getFromList(IconDesc& icon);
  bool      updateList(IconDesc& icon);

  void      loadCB(ComboBox& cb) {iconList.loadCB(cb);}

  void      setCur(String& s, ComboBox& cb) {iconList.setCur(s, cb);}

  void      markIcon(String& id) {IconDesc* ic = find(id);    if (ic) ic->inUse = true;}
  void      markDfltDir();

  IconDesc* find(String& s) {return iconList.findItem(s);}

  bool      validate(bool rptErrors = true);
  void      output(int tab);

  void      saveData(Archive& ar);
private:

  IconDesc* getNew() {return iconList.addNil(_T("Icon"));}

  void oneIconAvail();
  };


extern Icons icons;


