// ToolBar Edit Box


#pragma once

struct TBBtnCtx;


class TBEditBox : public CMFCToolBarEditBoxButton {
public:

  TBEditBox(uint id);
 ~TBEditBox() { }

         void       install(TBBtnCtx& ctx);

         void       getText(String& txt) {txt = m_strContents;}
         void       setText(TCchar* txt) {CString cs = txt; SetContents(cs);}

  static TBEditBox* get(int id);
  };

