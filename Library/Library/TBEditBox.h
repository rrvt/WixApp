// ToolBar Edit Box


#pragma once


class TBEditBox : public CMFCToolBarEditBoxButton {

uint id;

public:

int maxChars;

                    TBEditBox(uint id);
                   ~TBEditBox() { }

         void       install(int noChars);

         uint       getId() {return id;}

         CString    getText() {return CMFCToolBarEditBoxButton::GetContentsAll(id);}
         void       setText(TCchar* txt) {CString cs = txt; SetContents(cs);}

  static TBEditBox* get(int id);
  };

