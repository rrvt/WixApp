// WixDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WixDataDlg.h"
#include "About.h"
#include "DefaultPath.h"
#include "DspDirs.h"
#include "filename.h"
#include "Finish.h"
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

  ON_EN_KILLFOCUS(  IDC_ProductName,         &WixDataDlg::OnLeaveProductname)
  ON_EN_KILLFOCUS(  IDC_CompanyName,         &WixDataDlg::OnLeaveCompanyname)
  ON_EN_KILLFOCUS(  IDC_WixName,             &WixDataDlg::OnLeaveWixname)
  ON_EN_KILLFOCUS(  IDC_WixVersion,          &WixDataDlg::OnLeaveWixVersion)
  ON_BN_CLICKED(    IDC_BrowseProgFtrIcon,   &WixDataDlg::OnBrowseCtrlPanelIcon)

  ON_CBN_SELCHANGE( IDC_Feature,             &WixDataDlg::OnChangeFeature)
  ON_CBN_KILLFOCUS( IDC_Feature,             &WixDataDlg::OnLeaveFeature)
  ON_CBN_DROPDOWN(  IDC_Feature,             &WixDataDlg::OnLeaveFeature)
  ON_BN_CLICKED(    IDC_NewFeature,          &WixDataDlg::OnNewFeature)
  ON_BN_CLICKED(    IDC_DelFeature,          &WixDataDlg::OnDelFeature)

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
  ON_COMMAND(       ID_OPTIONS,              &WixDataDlg::OnOptions)
  ON_COMMAND(       ID_DisplayDirectories,   &WixDataDlg::OnDisplayDirectories)
  ON_COMMAND(ID_FILE_SAVEWXDFILE,             &WixDataDlg::OnSaveWXDfile)
  ON_COMMAND(ID_FILE_EXIT,                    &WixDataDlg::OnExit)
  ON_COMMAND(ID_TEST_VALIDATE,                &WixDataDlg::OnValidate)
END_MESSAGE_MAP()


WixDataDlg::WixDataDlg(CWnd* pParent) : CDialogEx(WixDataDlg::IDD, pParent) {}


WixDataDlg::~WixDataDlg() { }



void WixDataDlg::OnFileNew() {wixData.newFile(this);}    //solution.newFile(); solution.loadEB(*this);}


void WixDataDlg::OnFileOpen() {wixData.openFile(this);}


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
  DDX_Control(pDX, IDC_Feature,       featureCB);
  DDX_Control(pDX, IDC_ProgFileName,  progFileEB);
  DDX_Control(pDX, IDC_StartMenuName, startMenuEB);
  DDX_Control(pDX, IDC_Component,     componentCB);
  DDX_Control(pDX, IDC_ComponentPath, pathEB);
  DDX_Control(pDX, IDC_OnStartMenu,   startMenuCH);
  DDX_Control(pDX, IDC_OnDeskTop,     deskTopCH);
  DDX_Control(pDX, IDC_VersionAvail,  versionAvailCH);
  DDX_Control(pDX, IDC_OnPath,        isOnPathCH);
  DDX_Control(pDX, IDC_ComponentIcon, iconCB);
  DDX_Control(pDX, IDC_Description,   descriptionEB);
  DDX_Control(pDX, IDC_CHECK1,        isWin7ch);
  DDX_Control(pDX, IDC_CHECK2,        isWinXPch);
  DDX_Control(pDX, IDC_CHECK3,        isWin2Kch);
  }


// App command to run the dialog


void WixDataDlg::OnGetSolution() {solution.newFile();}


void WixDataDlg::OnAppAbout() {CAboutDlg aboutDlg; aboutDlg.DoModal();}


void WixDataDlg::OnLeaveProductname() {wixData.setDefaults(this);  UpdateWindow();}
void WixDataDlg::OnLeaveCompanyname() {product.storeCompany(*this);}
void WixDataDlg::OnLeaveWixname() {    product.storeWixName(*this);}
void WixDataDlg::OnLeaveWixVersion() { product.storeWixVer(*this);}

void WixDataDlg::OnBrowseCtrlPanelIcon() {product.browseIcon(*this);  UpdateWindow();}


// A Wix file is composed of a collection of features.  A feature is a collection of components.
// A Component is one installable entity (usually a file) along with any shortcut link
// files associated with the entity.

void WixDataDlg::OnLeaveFeature()     {features.storeFeatureData(*this);}
void WixDataDlg::OnLeavingProgFile()  {features.getCurFeature()->storeProgFileName(*this);}
void WixDataDlg::OnLeavingStartMenu() {features.getCurFeature()->storeMenuName(*this);}

void WixDataDlg::OnChangeFeature() {features.changeFeature(*this); UpdateWindow();}
void WixDataDlg::OnNewFeature()    {features.newFeature(*this);    UpdateWindow();}


void WixDataDlg::OnDelFeature() {features.delFeature(*this);  features.loadCB(*this);    UpdateWindow();}



void WixDataDlg::loadFeatures() {features.loadCB(*this); UpdateWindow();}


// Component ...

void WixDataDlg::OnLeavingComponent() {features.getCurFeature()->storeComponentData(*this);}
void WixDataDlg::OnChangeComponent()  {features.getCurFeature()->changeComponent(*this); UpdateWindow();}
void WixDataDlg::OnNewComponent()     {features.getCurFeature()->newComponent(*this);    UpdateWindow();}


void WixDataDlg::OnDeleteComponent() {
Feature* ftr = features.getCurFeature();

  if (ftr) ftr->delComponent(*this);  ftr->loadComponent(*this);    UpdateWindow();
  }


void WixDataDlg::OnBrowseComponentPath()   {features.getCurComponent()->browsePath(*this);}
void WixDataDlg::OnLeavingDescription()    {features.getCurComponent()->storeDescription(*this);}
void WixDataDlg::OnBnClickedOnstartmenu()  {features.getCurComponent()->storeIsStartMenu(*this);}
void WixDataDlg::OnBnClickedOndesktop()    {features.getCurComponent()->storeIsDeskTop(*this);}
void WixDataDlg::OnBnClickedVersionavail() {features.getCurComponent()->storeIsVersionAvail(*this);}
void WixDataDlg::OnBnClickedOnPath()       {features.getCurComponent()->storeIsOnPath(*this);}
void WixDataDlg::OnBrowseForIcon()         {features.getCurComponent()->browseIcon(*this); UpdateWindow();}
void WixDataDlg::OnUpdateIcon()            {features.getCurComponent()->updateIcon(*this);}
void WixDataDlg::OnBnClickedIsWin7()       {features.getCurComponent()->storeIsWin7( *this);}
void WixDataDlg::OnBnClickedIsWinXP()      {features.getCurComponent()->storeIsWinXP(*this);}
void WixDataDlg::OnBnClickedIsWin2K()      {features.getCurComponent()->storeIsWin2K(*this);}


void WixDataDlg::OnLbnSelchangeList1() {
  // TODO: Add your control notification handler code here
}




void WixDataDlg::OnDisplayDirectories() {
DspDirs dspDirs;

  dspDirs.output();
  }


// Create both the stored representation of the dialog information and also the Wix Output file

void WixDataDlg::createProduct() {
Finish finish;
String path;
String wixDataPath;

  if (!wixData.validate()) return;

  CDialogEx::OnOK();

  wixData.output();

  wixData.getWxdPath(wixDataPath);

  wixData.writeWixData(wixDataPath);

  finish.DoModal();
  }


void WixDataDlg::OnBnClickedCancel() {CDialogEx::OnCancel();}


void WixDataDlg::OnSaveWXDfile() {
String title        = _T("Wix Data File");
String iniFileName;
String defExt       = _T("wxd");
String extPat       = _T("*.wxd");
String path;

  wixData.getWxdPath(iniFileName);

  if (getSaveAsPathDlg(title, iniFileName, defExt, extPat, path))
                                                   {wixData.validate(false); wixData.writeWixData(path);}
  }


void WixDataDlg::OnExit() {CDialogEx::OnOK();}


void WixDataDlg::OnValidate() {wixData.validate();}
