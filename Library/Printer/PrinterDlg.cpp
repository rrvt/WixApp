// PrinterDlg.cpp : implementation file


#include "pch.h"
#include "PrinterDlg.h"
#include "PrinterList.h"
#include "resource.h"


struct AttrItem {
String txt;
int    item;
};

static AttrItem OrientList[] = {{_T("Landscape"),         DMORIENT_LANDSCAPE},
                                {_T("Portrait"),          DMORIENT_PORTRAIT}
                                };
static AttrItem ColorList[]  = {{_T("Color"),             DMCOLOR_COLOR},
                                {_T("Black & White"),     DMCOLOR_MONOCHROME}
                                };
static AttrItem PlexList[]   = {{_T("Simplex"),           DMDUP_SIMPLEX},
                                {_T("Duplex Horizontal"), DMDUP_HORIZONTAL},
                                {_T("Duplex Vertical"),   DMDUP_VERTICAL}
                                };

// PrinterDlg dialog

IMPLEMENT_DYNAMIC(PrinterDlg, CDialogEx)

PrinterDlg::PrinterDlg(PrinterInfo& prtrInfo, CWnd* pParent) :
                                                CDialogEx(IDD_PrtrDlg, pParent), info(prtrInfo) { }


PrinterDlg::~PrinterDlg() { }


BOOL PrinterDlg::OnInitDialog() {
int j;

  CDialogEx::OnInitDialog();

  initPrtrCbo();   initOrientCbo();   initColorCbo();   initPlexCbo();

  prtrDevAttr.load(info);

  j = prtrSelectCtl.FindStringExact(-1, prtrDevAttr.name);

  prtrSelectCtl.SetCurSel(j >= 0 ? j : 0);   loadAttr();

  return TRUE;
  }


void PrinterDlg::initPrtrCbo() {
PrinterList printerList;
PLIter      iter(printerList);
String*     prtr;

  printerList.get();

  for (prtr = iter(); prtr; prtr = iter++) prtrSelectCtl.AddString(*prtr);

  pageSizeTbl.set(sizeCtl, LetterPSz);
  }


void PrinterDlg::initOrientCbo() {
int i;
int j;

  for (i = 0; i < noElements(OrientList); i++)
      {j = orientCtl.AddString(OrientList[i].txt);   orientCtl.SetItemData(j, OrientList[i].item);}
  }


void PrinterDlg::initColorCbo() {
int i;
int j;

  for (i = 0; i < noElements(ColorList); i++)
        {j = colorCtl.AddString(ColorList[i].txt);     colorCtl.SetItemData(j, ColorList[i].item);}
  }


void PrinterDlg::initPlexCbo() {
int i;
int j;

  for (i = 0; i < noElements(PlexList); i++)
          {j = plexCtl.AddString(PlexList[i].txt);       plexCtl.SetItemData(j, PlexList[i].item);}
  }


void PrinterDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control( pDX, IDC_PrtrSelect, prtrSelectCtl);
  DDX_CBIndex( pDX, IDC_PrtrSelect, prtrSelect);
  DDX_Control( pDX, IDC_Orient,     orientCtl);
  DDX_CBIndex( pDX, IDC_Orient,     orient);
  DDX_Control( pDX, IDC_Copies,     copiesCtl);
  DDX_Text(    pDX, IDC_Copies,     copies);
  DDX_Control( pDX, IDC_Collate,    collateCtl);
  DDX_Check(   pDX, IDC_Collate,    collate);
  DDX_Control( pDX, IDC_Color,      colorCtl);
  DDX_CBString(pDX, IDC_Color,      color);
  DDX_Control( pDX, IDC_Plex,       plexCtl);
  DDX_CBString(pDX, IDC_Plex,       plex);
  DDX_Control( pDX, IDC_SizeName,   sizeCtl);
  DDX_CBString(pDX, IDC_SizeName,   sizeName);
  DDX_Control( pDX, IDC_Width,      widthCtl);
  DDX_Text(    pDX, IDC_Width,      width);
  DDX_Control( pDX, IDC_Length,     lengthCtl);
  DDX_Text(    pDX, IDC_Length,     length);
  DDX_Control( pDX, IDC_TopMgn,     topMgnCtl);
  DDX_Text(    pDX, IDC_TopMgn,     topMgn);
  DDX_Control( pDX, IDC_LeftMgn,    leftMgnCtl);
  DDX_Text(    pDX, IDC_LeftMgn,    leftMgn);
  DDX_Control( pDX, IDC_RightMgn,   rightMgnCtl);
  DDX_Text(    pDX, IDC_RightMgn,   rightMgn);
  DDX_Control( pDX, IDC_BottomMgn,  bottomMgnCtl);
  DDX_Text(    pDX, IDC_BottomMgn,  bottomMgn);
  }


BEGIN_MESSAGE_MAP(PrinterDlg, CDialogEx)
  ON_CBN_SELCHANGE( IDC_PrtrSelect, &onPrtrselect)
  ON_CBN_SELCHANGE( IDC_SizeName,   &onSizeName)
//ON_CBN_SELCHANGE( IDC_Orient,     &onOrient)
  ON_EN_KILLFOCUS(  IDC_Width,      &onLeaveWidth)
  ON_EN_KILLFOCUS(  IDC_Length,     &onLeaveLength)
  ON_EN_KILLFOCUS(  IDC_TopMgn,     &onLeaveTopMgn)
  ON_EN_KILLFOCUS(  IDC_LeftMgn,    &onLeaveLeftMgn)
  ON_EN_KILLFOCUS(  IDC_RightMgn,   &onLeaveRightMgn)
  ON_EN_KILLFOCUS(  IDC_BottomMgn,  &onLeaveBottomMgn)
END_MESSAGE_MAP()


// PrinterDlg message handlers

void PrinterDlg::onPrtrselect() {
int     i = prtrSelectCtl.GetCurSel();   if (i < 0) return;
Cstring name;

  try {prtrSelectCtl.GetLBText(i, name);}   catch(...) {return;}

  prtrDevAttr.load(name, info);   loadAttr();
  }


void PrinterDlg::loadAttr() {
int i;
int j;

  orient = prtrDevAttr.orient;

  for (i = 0; i < noElements(OrientList); i++) if (OrientList[i].item == orient) {

    j = orientCtl.FindStringExact(-1, OrientList[i].txt);

    if (j >= 0) orientCtl.SetCurSel(j);   break;
    }

  copies  = prtrDevAttr.copies;   copiesCtl.SetWindowText(copies);

  collate = prtrDevAttr.collate;   collateCtl.SetCheck(collate ? BST_CHECKED : BST_UNCHECKED);

  for (i = 0; i < noElements(ColorList); i++) if (ColorList[i].item == prtrDevAttr.color)
            {j = colorCtl.FindStringExact(-1, ColorList[i].txt);   colorCtl.SetCurSel(j);   break;}

  for (i = 0; i < noElements(PlexList); i++) if (PlexList[i].item == prtrDevAttr.plex)
            {j = plexCtl.FindStringExact(-1, PlexList[i].txt);     plexCtl.SetCurSel(j);    break;}

  i = pageSizeTbl.findItem(prtrDevAttr.paperSz);   sizeName = pageSizeTbl.getName(i);

  j = sizeCtl.FindStringExact(-1, sizeName);   if (j >= 0) sizeCtl.SetCurSel(j);

  widthCtl.SetWindowText(prtrDevAttr.width);
  lengthCtl.SetWindowText(prtrDevAttr.length);
  topMgnCtl.SetWindowText(prtrDevAttr.topMgn);
  leftMgnCtl.SetWindowText(prtrDevAttr.leftMgn);
  rightMgnCtl.SetWindowText(prtrDevAttr.rightMgn);
  bottomMgnCtl.SetWindowText(prtrDevAttr.bottomMgn);
  }


void PrinterDlg::onSizeName() {setSize();}
//void PrinterDlg::onOrient()   {setSize();}


void PrinterDlg::onLeaveWidth() {
Cstring txt;

  widthCtl.GetWindowText(txt);   if (!isFloatingPt(txt)) widthCtl.SetFocus();
  }


void PrinterDlg::onLeaveLength() {
Cstring txt;

  lengthCtl.GetWindowText(txt);    if (!isFloatingPt(txt)) lengthCtl.SetFocus();
  }


void PrinterDlg::onLeaveTopMgn() {
Cstring txt;

  topMgnCtl.GetWindowText(txt);    if (!isFloatingPt(txt)) topMgnCtl.SetFocus();
  }


void PrinterDlg::onLeaveLeftMgn() {
Cstring txt;

  leftMgnCtl.GetWindowText(txt);    if (!isFloatingPt(txt)) leftMgnCtl.SetFocus();
  }

void PrinterDlg::onLeaveRightMgn() {
Cstring txt;

  rightMgnCtl.GetWindowText(txt);    if (!isFloatingPt(txt)) rightMgnCtl.SetFocus();
  }


void PrinterDlg::onLeaveBottomMgn() {
Cstring txt;

  bottomMgnCtl.GetWindowText(txt);    if (!isFloatingPt(txt)) bottomMgnCtl.SetFocus();
  }


bool PrinterDlg::isFloatingPt(Cstring txt) {
int   i;
int   n = txt.length();
Tchar ch;

  for (i = 0; i < n; i++)
                  {ch = txt[i];  if ((ch < _T('0') || _T('9') < ch) && ch != _T('.')) return false;}
  return true;
  }



void PrinterDlg::OnOK() {
int    j;
uint   pos;
String s;

  setSize();   CDialogEx::OnOK();

  j = prtrSelectCtl.GetCurSel();   if (j >= 0) prtrSelectCtl.GetLBText(j, prtrDevAttr.name);
  j = orientCtl.GetCurSel();       if (j >= 0) orient    = (int) orientCtl.GetItemData(j);
  j = colorCtl.GetCurSel();        if (j >= 0) colorItem = (int) colorCtl.GetItemData(j);
  j = plexCtl.GetCurSel();         if (j >= 0) plexItem  = (int) plexCtl.GetItemData(j);

  prtrDevAttr.orient    = (PrtrOrnt) orient;
  prtrDevAttr.copies    = copies.stoi(pos);
  prtrDevAttr.collate   = collate;
  prtrDevAttr.color     = colorItem;
  prtrDevAttr.plex      = plexItem;
  prtrDevAttr.paperSz   = paperSz;
  prtrDevAttr.width     = width;
  prtrDevAttr.length    = length;
  prtrDevAttr.topMgn    = topMgn;
  prtrDevAttr.leftMgn   = leftMgn;
  prtrDevAttr.rightMgn  = rightMgn;
  prtrDevAttr.bottomMgn = bottomMgn;

  prtrDevAttr.save();
  }


// Set paperSize, Orientation, Width and Length so that they are all consistent
// User defined

void PrinterDlg::setSize() {
int    i      = sizeCtl.GetCurSel();     if (i < 0) return;
uint   j      = (uint) sizeCtl.GetItemData(i);  if (j < 0) return;

  paperSz = pageSizeTbl.getItem(j);   if (pageSizeTbl.isUserDefined(j)) return;

   widthCtl.SetWindowText(pageSizeTbl.getWidth(j));
  lengthCtl.SetWindowText(pageSizeTbl.getLength(j));
  }



///////////-------------------
#if 0
PrinterList     printerList;
PLIter          iter(printerList);
String*         prtr;
  printerList.get();

  for (prtr = iter(); prtr; prtr = iter++) prtrSelectCtl.AddString(*prtr);
  pageSizeTbl.set(sizeCtl, LetterPSz);

  for (i = 0; i < noElements(OrientList); i++)
    {j = orientCtl.AddString(OrientList[i].txt);   orientCtl.SetItemData(j, OrientList[i].item);}

  for (i = 0; i < noElements(ColorList); i++)
    {j = colorCtl.AddString(ColorList[i].txt);     colorCtl.SetItemData(j, ColorList[i].item);}

  for (i = 0; i < noElements(PlexList); i++)
    {j = plexCtl.AddString(PlexList[i].txt);       plexCtl.SetItemData(j, PlexList[i].item);}
#endif
#if 0
int    i = sizeCtl.GetCurSel();     if (i < 0) return;
uint   j = sizeCtl.GetItemData(i);  if (j < 0) return;
String wth;
String lng;


  paperSz = pageSizeTbl.getItem(j);

  orient = orientCtl.GetItemData(j);

  if (!pageSizeTbl.isUserDefined(j)) {
    widthCtl.SetWindowText( pageSizeTbl.getWidth(j));
    lengthCtl.SetWindowText(pageSizeTbl.getLength(j));
    }
#endif
#if 0
bool PrinterDlg::validatePaperSz() {
int     i       = sizeCtl.GetCurSel();     if (i < 0) return false;
uint    j       = sizeCtl.GetItemData(i);
uint    k;
Cstring wth;
Cstring lng;

  lengthCtl.GetWindowText(wth);   widthCtl.GetWindowText(lng);    k = pageSizeTbl.find(wth, lng);

  return j == k;
  }
#endif

