// Tool Bar Combo Box


#pragma once

struct CbxItem;


class TBCboBx : public CMFCToolBarComboBoxButton {

uint                       id;
int                        maxChars;
int                        percent;                           // width = maxChars * percent / 100;
CMFCToolBarComboBoxButton* actual;

public:

String caption;

              TBCboBx(uint myId);
             ~TBCboBx() { }

  TBCboBx*    install(int           noChars);
  TBCboBx*    install(int           idr,              TCchar* caption);
  TBCboBx*    install(const CbxItem cbxItem[], int n, TCchar* caption);

  // The following functions deal with the installed combo box.
  void        clear() {if (getActual()) actual->RemoveAllItems();   maxChars = 0;}

  void        setWthPercent(int prcnt) {percent = prcnt ? prcnt : 1;}
                                                              // width = maxChars * percent / 100;
  bool        setCaption();
  void        setCaption(TCchar* caption) {this->caption = caption;   setCaption();}
  bool        addItem(TCchar* txt, int val);
  bool        addItemSorted(TCchar* txt, int val);
  uint        getCount() {return actual ? actual->GetCount() : 0;}
  void        setWidth();
  void        setHeight();

  void*       getData(int index);
  bool        getCurSel(String& s, void*& data);
  int         getCurSel();

  int         find(TCchar* tc);
  bool        setCurSel(int index);
  bool        setCurSel(TCchar* tc);

  uint        getId() {return id;}

private:

  bool        getActual() {if (!actual) actual = GetByCmd(id);   return actual != 0;}
  String      findNext(int index);

  bool        add(TCchar* txt, int data);
  TBCboBx*    finInstall(TCchar* caption);
  void        setMaxChars(TCchar* txt) {int t;  t = _tcslen(txt);  if (t > maxChars) maxChars = t;}
  };

