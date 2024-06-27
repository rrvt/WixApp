// Tool Bar Combo Box


#pragma once

struct CbxItem;


class TBCboBx : public CMFCToolBarComboBoxButton {

uint                       id;
int                        maxChars;
CMFCToolBarComboBoxButton* actual;

public:

String caption;

              TBCboBx(uint id) : CMFCToolBarComboBoxButton(id, -1), id(id), maxChars(0),
                                                                                      actual(0) { }
             ~TBCboBx() { }

  TBCboBx*    install(int           noChars);
  TBCboBx*    install(int           idr,              TCchar* caption);
  TBCboBx*    install(const CbxItem cbxItem[], int n, TCchar* caption);

  // The following functions deal with the installed combo box.
  bool        setCaption();
  void        setCaption(TCchar* caption) {this->caption = caption;   setCaption();}
  bool        addItem(TCchar* txt, int val);
  bool        addItemSorted(TCchar* txt, int val);
  void        setWidth();
  void        setHeight();
  bool        getCurSel(String& s, int& data);

  uint        getId() {return id;}

private:

  bool        getActual();

  bool        add(TCchar* txt, int data);
  TBCboBx*    finInstall(TCchar* caption);
  void        setMaxChars(TCchar* txt) {int t;  t = _tcslen(txt);  if (t > maxChars) maxChars = t;}
  };

