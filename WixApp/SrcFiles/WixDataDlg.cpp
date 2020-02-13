// WixDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WixDataDlg.h"
#include "About.h"
#include "DefaultPath.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "MessageBox.h"
#include "Options.h"
#include "Product.h"
#include "Solution.h"
#include "RequiredOSs.h"
#include "WixData.h"


// WixDataDlg dialog

IMPLEMENT_DYNAMIC(WixDataDlg, CDialogEx)

BEGIN_MESSAGE_MAP(WixDataDlg, CDialogEx)
  ON_COMMAND(       ID_ABOUT,                &WixDataDlg::OnAppAbout)
  ON_COMMAND(       ID_FileNew,              &WixDataDlg::OnFileNew)
  ON_COMMAND(       ID_FileOpen,             &WixDataDlg::OnFileOpen)
  ON_COMMAND(       ID_FileRecent,           &WixDataDlg::OnFileRecent)

  ON_EN_KILLFOCUS(  IDC_WixName,             &WixDataDlg::OnLeaveWixname)
  ON_EN_KILLFOCUS(  IDC_WixVersion,          &WixDataDlg::OnLeaveWixVersion)
  ON_EN_KILLFOCUS(  IDC_CompanyName,         &WixDataDlg::OnLeaveCompanyname)
  ON_EN_KILLFOCUS(  IDC_ProductName,         &WixDataDlg::OnLeaveProductname)
  ON_BN_CLICKED(    IDC_BrowseProgFtrIcon,   &WixDataDlg::OnBrowseCtrlPanelIcon)

  ON_CBN_SELCHANGE( IDC_Group,               &WixDataDlg::OnChangeGroup)
  ON_CBN_KILLFOCUS( IDC_Group,               &WixDataDlg::OnLeavingGroup)
  ON_CBN_DROPDOWN(  IDC_Group,               &WixDataDlg::OnLeavingGroup)
  ON_BN_CLICKED(    IDC_NewGroup,            &WixDataDlg::OnNewGroup)
  ON_BN_CLICKED(    IDC_DelGroup,            &WixDataDlg::OnDeleteGroup)

  ON_CBN_SELCHANGE( IDC_Component,           &WixDataDlg::OnChangeComponent)
  ON_CBN_KILLFOCUS( IDC_Component,           &WixDataDlg::OnLeavingComponent)
  ON_CBN_DROPDOWN(  IDC_Component,           &WixDataDlg::OnLeavingComponent)
  ON_BN_CLICKED(    IDC_NewComponent,        &WixDataDlg::OnNewComponent)
  ON_BN_CLICKED(    IDC_DelComponent,        &WixDataDlg::OnDeleteComponent)

  ON_BN_CLICKED(    IDC_BrowseComponentPath, &WixDataDlg::OnBrowseComponentPath)

  ON_EN_KILLFOCUS(  IDC_Description,         &WixDataDlg::OnLeavingDescription)

  ON_CBN_SELCHANGE( IDC_ComponentIcon,       &WixDataDlg::OnUpdateIcon)
  ON_CBN_KILLFOCUS( IDC_ComponentIcon,       &WixDataDlg::OnUpdateIcon)
  ON_BN_CLICKED(    IDC_BrowseForIcon,       &WixDataDlg::OnBrowseForIcon)

  ON_EN_KILLFOCUS(  IDC_ProgFileName,        &WixDataDlg::OnLeavingProgFile)

  ON_EN_KILLFOCUS(  IDC_StartMenuName,       &WixDataDlg::OnLeavingStartMenu)

  ON_BN_CLICKED(    IDOK,                    &WixDataDlg::createProduct)
  ON_BN_CLICKED(    IDCANCEL,                &WixDataDlg::OnBnClickedCancel)

  ON_LBN_SELCHANGE( IDC_LIST1,               &WixDataDlg::OnLbnSelchangeList1)
  ON_BN_CLICKED(    IDC_OnStartMenu,         &WixDataDlg::OnBnClickedOnstartmenu)
  ON_BN_CLICKED(    IDC_OnDeskTop,           &WixDataDlg::OnBnClickedOndesktop)
  ON_BN_CLICKED(    IDC_VersionAvail,        &WixDataDlg::OnBnClickedVersionavail)
  ON_BN_CLICKED(    IDC_OnPath,              &WixDataDlg::OnBnClickedOnPath)
  ON_BN_CLICKED(    IDC_CHECK1,              &WixDataDlg::OnBnClickedIsWin7)
  ON_BN_CLICKED(    IDC_CHECK2,              &WixDataDlg::OnBnClickedIsWinXP)
  ON_BN_CLICKED(    IDC_CHECK3,              &WixDataDlg::OnBnClickedIsWin2K)

  ON_COMMAND(       ID_SOLUTION,             &WixDataDlg::OnGetSolution)
  ON_COMMAND(ID_OPTIONS, &WixDataDlg::OnOptions)
END_MESSAGE_MAP()


WixDataDlg::WixDataDlg(CWnd* pParent) : CDialogEx(WixDataDlg::IDD, pParent) {}


WixDataDlg::~WixDataDlg() { }



void WixDataDlg::OnFileNew() {solution.newFile(); solution.loadEB(*this);}


void WixDataDlg::OnFileOpen() {

  if (!wixData.readWixData()) return;

  solution.loadEB(*this);
  product.loadCB(*this);
  loadFeatures();
  }


void WixDataDlg::OnFileRecent() { }




void WixDataDlg::OnOptions() {
Options opt;

  opt.allowSameVer = product.isSameVerAllowed;
  opt.addLic       = product.isLicenseReq;
  opt.licPath      = product.licenseDsc;

  if (opt.DoModal() == IDOK) {
    product.isSameVerAllowed = opt.allowSameVer != 0;
    product.isLicenseReq     = opt.addLic;
    product.licenseDsc       = opt.licPath;
    }
  }


void WixDataDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_WixName,       wixNameEB);
  DDX_Control(pDX, IDC_WixVersion,    wixVersionEB);
  DDX_Control(pDX, IDC_CompanyName,   companyEB);
  DDX_Control(pDX, IDC_CtrlPnlIcn,    progFtrIconEB);
  DDX_Control(pDX, IDC_ProductName,   productNameEB);
  DDX_Control(pDX, IDC_Group,         groupCB);
  DDX_Control(pDX, IDC_Component,     componentCB);
  DDX_Control(pDX, IDC_ComponentPath, pathEB);
  DDX_Control(pDX, IDC_OnStartMenu,   startMenuCH);
  DDX_Control(pDX, IDC_OnDeskTop,     deskTopCH);
  DDX_Control(pDX, IDC_VersionAvail,  versionAvailCH);
  DDX_Control(pDX, IDC_OnPath,        isOnPathCH);
  DDX_Control(pDX, IDC_ComponentIcon, iconCB);
  DDX_Control(pDX, IDC_Description,   descriptionEB);
  DDX_Control(pDX, IDC_ProgFileName,  progFileEB);
  DDX_Control(pDX, IDC_StartMenuName, startMenuEB);
  DDX_Control(pDX, IDC_CHECK1,        isWin7ch);
  DDX_Control(pDX, IDC_CHECK2,        isWinXPch);
  DDX_Control(pDX, IDC_CHECK3,        isWin2Kch);
  }


// App command to run the dialog


void WixDataDlg::OnGetSolution() {solution.newFile();}


void WixDataDlg::OnAppAbout() {CAboutDlg aboutDlg; aboutDlg.DoModal();}


void WixDataDlg::OnLeaveWixname() {product.storeWixName(*this);}


void WixDataDlg::OnLeaveWixVersion() {product.storeWixVer(*this);}


void WixDataDlg::OnLeaveCompanyname() {product.storeCompany(*this);}


void WixDataDlg::OnLeaveProductname() {product.storeProduct(*this);}


void WixDataDlg::OnBrowseCtrlPanelIcon() {product.browseIcon(*this);  UpdateWindow();}


// A Wix file is composed of a collection of component groups.  A feature is the name for a list of
// Group.  A Component is one installable entity (usually a file) along with any shortcut link
// files associated with the entity.

void WixDataDlg::OnChangeGroup() {feature.changeGroup(*this); UpdateWindow();}


void WixDataDlg::OnLeavingGroup() {feature.updateID(*this);}



void WixDataDlg::OnNewGroup()
                  {feature.newGroup();   feature.loadCB(*this);   groupCB.SetFocus();   UpdateWindow();}


void WixDataDlg::OnDeleteGroup() {
  feature.delGroup(*this);  feature.loadCB(*this);    UpdateWindow();
  }



void WixDataDlg::loadFeatures() {feature.loadCB(*this); UpdateWindow();}


// Component ...

void WixDataDlg::OnChangeComponent()  {feature.getCurGroup()->changeComponent(*this);  UpdateWindow();}
void WixDataDlg::OnLeavingComponent() {feature.getCurGroup()->updateComponent(*this);}
void WixDataDlg::OnNewComponent()     {feature.getCurGroup()->newComponent(*this);}


void WixDataDlg::OnDeleteComponent() {
Group* grp = feature.getCurGroup();

  if (grp) grp->delComponent(*this);  grp->loadCB(*this);    UpdateWindow();
  }


void WixDataDlg::OnBrowseComponentPath()   {feature.getCurComponent()->browsePath(*this);}
void WixDataDlg::OnLeavingDescription()    {feature.getCurComponent()->storeDescription(*this);}
void WixDataDlg::OnBnClickedOnstartmenu()  {feature.getCurComponent()->storeIsStartMenu(*this);}
void WixDataDlg::OnBnClickedOndesktop()    {feature.getCurComponent()->storeIsDeskTop(*this);}
void WixDataDlg::OnBnClickedVersionavail() {feature.getCurComponent()->storeIsVersionAvail(*this);}
void WixDataDlg::OnBnClickedOnPath()       {feature.getCurComponent()->storeIsOnPath(*this);}
void WixDataDlg::OnBrowseForIcon()         {feature.getCurComponent()->browseIcon(*this); UpdateWindow();}
void WixDataDlg::OnUpdateIcon()            {feature.getCurComponent()->updateIcon(*this);}
void WixDataDlg::OnLeavingProgFile()       {feature.getCurComponent()->storeProgFileName(*this);}
void WixDataDlg::OnLeavingStartMenu()      {feature.getCurComponent()->storeMenuName(*this);}
void WixDataDlg::OnBnClickedIsWin7()       {feature.getCurComponent()->storeIsWin7( *this);}
void WixDataDlg::OnBnClickedIsWinXP()      {feature.getCurComponent()->storeIsWinXP(*this);}
void WixDataDlg::OnBnClickedIsWin2K()      {feature.getCurComponent()->storeIsWin2K(*this);}


void WixDataDlg::OnLbnSelchangeList1() {
  // TODO: Add your control notification handler code here
}


// Create both the stored representation of the dialog information and also the Wix Output file

void WixDataDlg::createProduct() {

  if (!wixData.validate()) return;

  CDialogEx::OnOK();

  wixData.output();
  }


void WixDataDlg::OnBnClickedCancel() {
  // TODO: Add your control notification handler code here
  CDialogEx::OnCancel();
  }
