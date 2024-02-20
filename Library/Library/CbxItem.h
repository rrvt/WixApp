// Combo Box Item


#pragma once


struct CbxItem {
String  txt;
int     data;

  CbxItem()                   : data(0) { }
  CbxItem(TCchar* txt, int d) : txt(txt), data(d) { }
  CbxItem(CbxItem& item) {copy(item);}

  // >=" and "=="
  bool     operator>= (CbxItem& item) {return txt >= item.txt;}
  bool     operator== (CbxItem& item) {return txt == item.txt;}

  CbxItem& operator=  (CbxItem& item) {copy(item); return *this;}

private:

  void copy(CbxItem& item) {txt = item.txt; data = item.data;}
  };

