// WixAppDlg.cpp : implementation file
//

#include "pch.h"
#include "WixAppDlg.h"
#include "AboutDlg.h"
#include "DspDirs.h"
#include "PmfDirectories.h"
#include "Product.h"
#include "Solution.h"
#include "StatusBar.h"
#include "WixData.h"
#include "WixOptsDlg.h"


static TCchar* MenuCaption = _T("Popup Menu");


// WixAppDlg dialog

IMPLEMENT_DYNAMIC(WixAppDlg, CDialogEx)


BEGIN_MESSAGE_MAP(WixAppDlg, CDialogEx)

  ON_WM_CREATE()
  ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, &OnResetToolBar)
  ON_NOTIFY_EX(    TTN_NEEDTEXT, 0,       &OnTtnNeedText)         // Do ToolTips
  ON_WM_MOVE()

  ON_COMMAND(      ID_NewProject,         &onNewProject)
  ON_COMMAND(      ID_OpenProject,        &onOpenProject)
  ON_COMMAND(      ID_Solution,           &onGetSolution)
  ON_COMMAND(      ID_Option,             &onOptions)
  ON_COMMAND(      ID_DisplayDir,         &onDisplayDir)
  ON_COMMAND(      ID_Validate,           &onValidate)
  ON_COMMAND(      ID_SaveWxdFile,        &onSaveWXDFile)
  ON_BN_CLICKED(   ID_SaveAllFiles,       &onSaveAllFiles)

  ON_BN_CLICKED(   IDCANCEL,              &onExit)
  ON_COMMAND(      ID_App_Exit,           &onExit)
  ON_COMMAND(      ID_App_About,          &onAppAbout)
  ON_COMMAND(      ID_Help,               &onHelp)

  ON_EN_KILLFOCUS( IDC_ProductName,       &onLeaveProductname)
  ON_EN_KILLFOCUS( IDC_Company,           &onLeaveCompanyname)
  ON_EN_KILLFOCUS( IDC_WixName,           &onLeaveWixname)
  ON_EN_KILLFOCUS( IDC_WixVersion,        &onLeaveWixVersion)
  ON_CBN_SELCHANGE(IDC_CtrlPnlIcn,        &onUpdateCtrlPanelIcon)
  ON_CBN_KILLFOCUS(IDC_CtrlPnlIcn,        &onUpdateCtrlPanelIcon)
  ON_BN_CLICKED(   IDC_BrowseProgFtrIcon, &OnBrowseCtrlPanelIcon)

  ON_CBN_SELCHANGE(IDC_Feature,           &onChangeFeature)
  ON_CBN_KILLFOCUS(IDC_Feature,           &onLeaveFeature)
  ON_CBN_DROPDOWN( IDC_Feature,           &onLeaveFeature)
  ON_BN_CLICKED(   IDC_NewFeature,        &onNewFeature)
  ON_BN_CLICKED(   IDC_DelFeature,        &onDelFeature)

  ON_EN_KILLFOCUS( IDC_ProgFileName,      &onLeavingProgFile)
  ON_EN_KILLFOCUS( IDC_StartMenuName,     &onLeavingStartMenu)

  ON_CBN_SELCHANGE(IDC_Component,         &onChangeComponent)
  ON_CBN_KILLFOCUS(IDC_Component,         &onLeavingComponent)
  ON_CBN_DROPDOWN( IDC_Component,         &onLeavingComponent)
  ON_BN_CLICKED(   IDC_NewComponent,      &onNewComponent)
  ON_BN_CLICKED(   IDC_DelComponent,      &onDeleteComponent)
  ON_BN_CLICKED(   IDC_BrwsCmpntPath,     &onBrowseComponentPath)

  ON_EN_KILLFOCUS( IDC_Description,       &onLeavingDescription)

  ON_BN_CLICKED(   IDC_Startup,           &onStartupClicked)
  ON_BN_CLICKED(   IDC_OnDeskTop,         &onBnClickedOndesktop)
  ON_BN_CLICKED(   IDC_VersionAvail,      &onBnClickedVersionavail)
  ON_BN_CLICKED(   IDC_OnStartMenu,       &onBnClickedOnstartmenu)
  ON_BN_CLICKED(   IDC_OnPath,            &onBnClickedOnPath)

  ON_BN_CLICKED(   IDC_Win7,              &onBnClickedIsWin7)
  ON_BN_CLICKED(   IDC_WinXP,             &onBnClickedIsWinXP)
  ON_BN_CLICKED(   IDC_Win2K,             &onBnClickedIsWin2K)

  ON_CBN_SELCHANGE(IDC_ComponentIcon,     &onUpdateIcon)
  ON_CBN_KILLFOCUS(IDC_ComponentIcon,     &onUpdateIcon)
  ON_BN_CLICKED(   IDC_BrowseForIcon,     &onBrowseForIcon)

#ifdef DialogSizable
  ON_WM_SIZE()
#endif

END_MESSAGE_MAP()


WixAppDlg::WixAppDlg(TCchar* helpPth, CWnd* pParent) :
                                             helpPath(helpPth), CDialogEx(IDD_WixDlg, pParent) { }


int WixAppDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) {

  if (CDialogEx::OnCreate(lpCreateStruct) == -1) return -1;

  return 0;
  }


BOOL WixAppDlg::OnInitDialog() {

  CDialogEx::OnInitDialog();

  GetWindowRect(&winRect);   winPos.setDLUToPxls(winRect, DlgWidth, DlgDepth);

  if (!toolBar.create(this, IDR_TOOLBAR)) return false;

  if (!statusBar.create(this, IDC_StatusBar)) return false;

  GetWindowRect(&winRect);   toolBar.move(winRect);   SetBackgroundColor(RGB(255,255,255));

  winPos.initialPos(this, winRect);   isInitialized = true;   statusBar.setReady();   return true;
  }


// MainFrame message handlers

LRESULT WixAppDlg::OnResetToolBar(WPARAM wParam, LPARAM lParam) {setupToolBar();  return 0;}


void WixAppDlg::setupToolBar() {
  toolBar.addButton(ID_NewProject,   _T(" New Project "));
  toolBar.addButton(ID_OpenProject,  _T(" Open Project "));
  toolBar.addButton(ID_SaveAllFiles, _T(" Save All Files "));
  toolBar.addButton(ID_SaveWxdFile,  _T(" Save Wxd File "));
  }


#ifdef DialogSizable

void WixAppDlg::OnSize(UINT nType, int cx, int cy) {
CRect winRect;

  CDialogEx::OnSize(nType, cx, cy);

  if (!isInitialized) return;

  GetWindowRect(&winRect);   winPos.set(winRect);   toolBar.move(winRect);   statusBar.move(winRect);
  }
#endif


// Do ToolTips

BOOL WixAppDlg::OnTtnNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
                                                                  {return toolBar.OnTtnNeedText(pNMHDR);}

void WixAppDlg::OnMove(int x, int y)
            {CRect winRect;   GetWindowRect(&winRect);   winPos.set(winRect);   CDialogEx::OnMove(x, y);}


void WixAppDlg::onNewProject()  {if (!wixData.newProject(this)) onExit();}
void WixAppDlg::onGetSolution() {solution.newProject(false);}

void WixAppDlg::onOpenProject() {wixData.openProject(this);}


void WixAppDlg::onOptions() {
WixOptsDlg opt;

  opt.allowSameVer = product.isSameVerAllowed;
  opt.addLic       = product.isLicenseReq;
  opt.licPath      = product.licenseDsc;

  if (opt.DoModal() == IDOK) {
    product.isSameVerAllowed = opt.allowSameVer != 0;
    product.isLicenseReq     = opt.addLic;
    product.licenseDsc       = opt.licPath;
    }
  }


void WixAppDlg::onValidate()   {wixData.validate();}
void WixAppDlg::onDisplayDir() {DspDirs dspDirs;   dspDirs.output();}


void WixAppDlg::onSaveWXDFile() {wixData.outputWxd();}


// Create both the stored representation of the dialog information and also the Wix Output file

void WixAppDlg::onSaveAllFiles() {
FinishDlg dlg;

  pmfDirectories.initFixedDirs();   if (!wixData.validate()) return;

  CDialogEx::OnOK();   wixData.outputProduct();   wixData.outputWxd();    dlg.DoModal();
  }


void WixAppDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_ProductName,   productNameEB);
  DDX_Control(pDX, IDC_Company,       companyEB);
  DDX_Control(pDX, IDC_WixName,       wixNameEB);
  DDX_Control(pDX, IDC_WixVersion,    wixVersionEB);
  DDX_Control(pDX, IDC_CtrlPnlIcn,    prodIconCB);

  DDX_Control(pDX, IDC_Feature,       featureCB);
  DDX_Control(pDX, IDC_ProgFileName,  progFileEB);
  DDX_Control(pDX, IDC_StartMenuName, startMenuEB);

  DDX_Control(pDX, IDC_Component,     componentCB);
  DDX_Control(pDX, IDC_ComponentPath, pathEB);
  DDX_Control(pDX, IDC_Description,   descriptionEB);
  DDX_Control(pDX, IDC_OnStartMenu,   isStartMenu);
  DDX_Control(pDX, IDC_OnDeskTop,     isDeskTop);
  DDX_Control(pDX, IDC_VersionAvail,  isVersionAvail);
  DDX_Control(pDX, IDC_OnPath,        isOnPathCH);
  DDX_Control(pDX, IDC_Startup,       isStartupApp);
  DDX_Control(pDX, IDC_Win7,          isWin7ch);
  DDX_Control(pDX, IDC_WinXP,         isWinXPch);
  DDX_Control(pDX, IDC_Win2K,         isWin2Kch);
  DDX_Control(pDX, IDC_ComponentIcon, iconCB);
  }


void WixAppDlg::onLeaveProductname()    {wixData.setDefaults(this);  UpdateWindow();}
void WixAppDlg::onLeaveCompanyname()    {product.storeCompany(*this);}
void WixAppDlg::onLeaveWixname()        {product.storeWixName(*this);}
void WixAppDlg::onLeaveWixVersion()     {product.storeWixVer(*this);}
void WixAppDlg::onUpdateCtrlPanelIcon() {product.updateIcon(*this);}
void WixAppDlg::updateCtrlPanelIconCB() {product.updateIconCB(prodIconCB);}
void WixAppDlg::OnBrowseCtrlPanelIcon() {product.browseIcon(*this);   updateIconCB();   UpdateWindow();}


// A Wix file is composed of a collection of features.  A feature is a collection of components.
// A Component is one installable entity (usually a file) along with any shortcut link
// files associated with the entity.

void WixAppDlg::onChangeFeature()    {features.changeFeature(*this); UpdateWindow();}
void WixAppDlg::onLeaveFeature()     {features.storeFeatureData(*this);}
void WixAppDlg::onNewFeature()       {features.newFeature(*this);    UpdateWindow();}
void WixAppDlg::onDelFeature()    {features.delFeature(*this);  features.loadCB(*this);   UpdateWindow();}
void WixAppDlg::onLeavingProgFile()  {features.getCurFeature()->storeProgFileName(*this);}
void WixAppDlg::onLeavingStartMenu() {features.getCurFeature()->storeMenuName(*this);}


// Component ...

void WixAppDlg::onChangeComponent()  {features.getCurFeature()->changeComponent(*this); UpdateWindow();}
void WixAppDlg::onLeavingComponent() {features.getCurFeature()->storeComponentData(*this);}
void WixAppDlg::onNewComponent()     {features.getCurFeature()->newComponent(*this);    UpdateWindow();}


void WixAppDlg::onDeleteComponent() {
Feature* ftr = features.getCurFeature();

  if (ftr) ftr->delComponent(*this);  ftr->loadComponent(*this);    UpdateWindow();
  }


void WixAppDlg::onBrowseComponentPath()   {features.browseCurComponent(*this);}


void WixAppDlg::onLeavingDescription()    {features.getCurComponent()->storeDescription(*this);}


void WixAppDlg::onStartupClicked()        {features.getCurComponent()->storeStartup(*this);}
void WixAppDlg::onBnClickedOndesktop()    {features.getCurComponent()->storeIsDeskTop(*this);}
void WixAppDlg::onBnClickedVersionavail() {features.getCurComponent()->storeIsVersionAvail(*this);}
void WixAppDlg::onBnClickedOnstartmenu()  {features.getCurComponent()->storeIsStartMenu(*this);}
void WixAppDlg::onBnClickedOnPath()       {features.getCurComponent()->storeIsOnPath(*this);}
void WixAppDlg::onBnClickedIsWin7()       {features.getCurComponent()->storeIsWin7( *this);}
void WixAppDlg::onBnClickedIsWinXP()      {features.getCurComponent()->storeIsWinXP(*this);}
void WixAppDlg::onBnClickedIsWin2K()      {features.getCurComponent()->storeIsWin2K(*this);}

void WixAppDlg::onUpdateIcon()            {features.getCurComponent()->updateIcon(*this);}
void WixAppDlg::updateIconCB()            {features.getCurComponent()->updateIconCB(iconCB);}
void WixAppDlg::onBrowseForIcon()
                {features.getCurComponent()->browseIcon(*this); updateCtrlPanelIconCB(); UpdateWindow();}


void WixAppDlg::onHelp() {
String topic = helpPath; topic += _T(">Introduction");

  ::HtmlHelp(GetSafeHwnd(), topic,  HH_DISPLAY_TOC, 0);
  }


void WixAppDlg::onAppAbout() {AboutDlg aboutDlg; aboutDlg.DoModal();}


void WixAppDlg::onExit()            {CDialogEx::OnOK();}


