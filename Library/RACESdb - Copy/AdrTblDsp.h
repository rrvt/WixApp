// Address Table Display in Doc/View Apps



#pragma once
#include "AdrTbl.h"


class AdrRcdDsp : public AdrRcd {


public:

  AdrRcdDsp() { }
 ~AdrRcdDsp() { }

  virtual void display();
  };


class AdrTblDsp : public AdrTbl {

  AdrTblDsp() { }
 ~AdrTblDsp() { }

  virtual void display();

private:


  void setTabs();
  };

