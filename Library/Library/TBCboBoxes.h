// Tool Bar ComboBoxes


#pragma once
#include "CbxItem.h"
#include "ExpandableP.h"
#include "IterT.h"
#include "ToolBarDim.h"


class TBCboBox : public MfcTBCboBtn {         // Allows access to protected data in Button
uint         id;
MfcTBCboBtn* tbcb{0};
int          maxChars{0};
int          percent{100};                    // width = maxChars * percent / 100;

public:

String caption;

            TBCboBox(uint myID) : MfcTBCboBtn(myID, -1), id(myID) { }
            TBCboBox(TBCboBox& cbo) {copy(cbo);}
           ~TBCboBox() { }

  void      clear() {(*this)()->RemoveAllItems();   maxChars = 0;   percent = 100;}

  uint      getId() {return id;}
  bool      isInstalled() {return tbcb != 0;}

  void      set(int     noChars);
  void      set(TCchar* caption);
  void      setImage(int imageIndex) {SetImage(imageIndex);}
  void      resetCaption() {set(caption);}
  void      setWthPercent(int prcnt) {percent = prcnt ? prcnt : 12;}
  void      setHeight();

  void      set(MfcTBCboBtn* mfcCbo) {tbcb = mfcCbo;}

  bool      setItems(int idr);
  bool      setItems(CCbxItem cbxItem[], int n);

  void      update();


  // Manipulate tuple in the ComboBox

  bool      addItem(    TCchar* txt, ulongP data);
  bool      addItemSrtd(TCchar* txt, ulongP data);

  bool      setCurSel(int index) {return (*this)()->SelectItem(index, false);}

  bool      setCurSel(TCchar* tc) {return (*this)()->SelectItem(tc);}

  int       getCurSel() {return (*this)()->GetCurSel();}
  bool      getCurSel(String& s, ulongP& data);
  ulongP    getCmdId();
  ulongP    getData(int index) {return (*this)()->GetItemData(index);}

  int       find(TCchar* tc) {return (*this)()->FindItem(tc);}
  uintP     getCount()       {return (*this)()->GetCount();}

  TBCboBox* operator() () {return tbcb ? (TBCboBox*) tbcb : this;}  // Get ComboBox object ptr

  TBCboBox& operator=  (TBCboBox& cbo) {copy(cbo); return *this;}
  bool      operator>= (TBCboBox& cbo) {return id >= cbo.id;}
  bool      operator== (uint key)      {return id == key;}

  MfcTBCboBtn* getByCmd(uint id) {return MfcTBCboBtn::GetByCmd(id);}

private:

  void      copy(TBCboBox& cbo)
                 {id = cbo.id;  tbcb = cbo.tbcb; maxChars = cbo.maxChars;   percent = cbo.percent;}
 TBCboBox() { }
 };


class TBCboBoxes;
typedef DatumPtrT<TBCboBox, uint>   TBCboBoxP;
typedef IterT<TBCboBoxes, TBCboBox> TBCboIter;


class TBCboBoxes {

ExpandableP<TBCboBox, uint, TBCboBoxP, 2> data;

public:

  TBCboBoxes() { }
 ~TBCboBoxes() {clear();}

  void clear() {data.clear();}

  TBCboBox& create(uint id);

  TBCboBox* install(uint id, int noChars);

  TBCboBox* find(uint id) {return data.find(id);}

  MfcTBCboBtn* getByCmd(uint id) {return MfcTBCboBtn::GetByCmd(id);}

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  TBCboBox* datum(int i) {return 0 <= i && i < nData() ? data[i] : (TBCboBox*) 0;}

  int       nData()      {return data.end();}                // returns number of data items in array
                                                             // not necessarily private
  void      removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename TBCboIter;
  };

