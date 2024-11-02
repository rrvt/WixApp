// SFTPDoc.h : interface of the SFTPDoc class


#pragma once
#include "CDoc.h"
#include "CmdTest.h"
#include "MainFrame.h"
#include "SiteID.h"


enum DataSource {NotePadSrc, StoreSrc, PrvSiteSrc, CurSiteSrc, UpdSiteSrc, StrTxtSrc, FontSrc};

class SiteFileDsc;
class WebNode;


class SFTPDoc : public CDoc {

PathDlgDsc  pathDlgDsc;

DataSource  dataSource;

bool        savePrv;
int         noFiles;

protected:                                                              // create from serialization only

  SFTPDoc() noexcept;
  DECLARE_DYNCREATE(SFTPDoc)

public:

  virtual     ~SFTPDoc();

  void         defaultSite();
  void         setNoFiles(int n) {noFiles = n;}

  bool         isLocked(bool prevent = false);

  bool         saveData(DataSource ds, TCchar* path)     {dataSource = ds; return OnSaveDocument(path);}

  DataSource   dataSrc() {return dataSource;}
  void         display(DataSource ds = NotePadSrc);

  virtual void serialize(Archive& ar);

  void         debugDsp(TCchar* title);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

private:

  void saveFile(TCchar* title, TCchar* suffix, TCchar* fileType);

  void testLine(int n);
  void wholePage();

// Generated message map functions

protected:

  DECLARE_MESSAGE_MAP()

public:

  afx_msg void onNewSite();
  afx_msg void onPickSite();
  afx_msg void onEditSite();
  afx_msg void onDelSite();

  afx_msg void onPrepWebCmprCmd()   {prepWebCmprCmd.start();}
  afx_msg void onPrepPrevCmpr();

  afx_msg void onUpdateSite();

          bool loadPrevSiteData();
          void updatePrevSite() {if (savePrv) {saveSite(PrvSiteSrc);  savePrv = false;}}

  afx_msg void onRemoteDir() {webFiles.start();}

  afx_msg void OnFileSave();
  afx_msg void onEditCopy();

  afx_msg LRESULT onFinSTOUMsg(WPARAM wParam, LPARAM lParam);


//  afx_msg void onCurrentTest() {cmdTest.ftpCmdSTOR();}

  afx_msg void onSftpAcct()    {cmdTest.ftpCmdACCT();}
  afx_msg void onSftpADAT()    {cmdTest.ftpCmdADAT();}
  afx_msg void onSftpALLO()    {cmdTest.ftpCmdALLO(1024);}
  afx_msg void onSftpAPPE()    {cmdTest.ftpCmdAPPE();}
  afx_msg void onSftpAUTH()    {cmdTest.ftpCmdAUTH();}
  afx_msg void onSftpAVBL()    {cmdTest.ftpCmdAVBL();}
  afx_msg void onSftpCCC()     {cmdTest.ftpCmdCCC();}
  afx_msg void onSftpCDUP()    {cmdTest.ftpCmdCDUP();}
  afx_msg void onSftpCONF()    {cmdTest.ftpCmdCONF();}
  afx_msg void onSftpCSID()    {cmdTest.ftpCmdCSID();}
  afx_msg void onSftpCWD()     {cmdTest.ftpCmdCWD();}
  afx_msg void onSftpDELE()    {cmdTest.ftpCmdDELE();}
  afx_msg void onSftpDSIZ()    {cmdTest.ftpCmdDSIZ();}
  afx_msg void onSftpENC()     {cmdTest.ftpCmdENC();}
  afx_msg void onSftpFEAT()    {cmdTest.ftpCmdFEAT();}
  afx_msg void onSftpHELP()    {cmdTest.ftpCmdHELP();}
  afx_msg void onSftpLIST()    {cmdTest.ftpCmdLIST();}
  afx_msg void onSftpMDTM()    {cmdTest.ftpCmdMDTM();}
  afx_msg void onSftpMFCT()    {cmdTest.ftpCmdMFCT();}
  afx_msg void onSftpMIC()     {cmdTest.ftpCmdMIC();}
  afx_msg void onSftpMKD()     {cmdTest.ftpCmdMKD();}
  afx_msg void onSftpMLSD()     {cmdTest.ftpCmdMLSD();}
  afx_msg void onSftpMLST()     {cmdTest.ftpCmdMLST();}
  afx_msg void onSftpMODE()     {cmdTest.ftpCmdMODE();}
  afx_msg void onSftpNLST()     {cmdTest.ftpCmdNLST();}
  afx_msg void onSftpNOOP()     {cmdTest.ftpCmdNOOP();}
  afx_msg void onSftpOPTS()     {cmdTest.ftpCmdOPTS();}
  afx_msg void onSftpPWD()      {cmdTest.ftpCmdPWD();}
  afx_msg void onSftpQUIT()     {cmdTest.ftpCmdQUIT();}
  afx_msg void onSftpRETR()     {cmdTest.ftpCmdRETR();}
  afx_msg void onSftpRMD()      {cmdTest.ftpCmdRMD();}
  afx_msg void onSftpRMDA()     {cmdTest.ftpCmdRMDA();}
  afx_msg void onSftpRNFR()     {cmdTest.ftpCmdRNFR();}
  afx_msg void onSftpRNTO()     {cmdTest.ftpCmdRNTO();}
  afx_msg void onSftpSITE()     {cmdTest.ftpCmdSITE();}
  afx_msg void onSftpSIZE()     {cmdTest.ftpCmdSIZE();}
  afx_msg void onSftpSMNT()     {cmdTest.ftpCmdSMNT();}
  afx_msg void onSftpSPSV()     {cmdTest.ftpCmdSPSV();}
  afx_msg void onSftpSTAT()     {cmdTest.ftpCmdSTAT();}
  afx_msg void onSftpSTOR()     {cmdTest.ftpCmdSTOR();}
  afx_msg void onSftpSTOU()     {cmdTest.ftpCmdSTOU();}
  afx_msg void onSftpSTRU()     {cmdTest.ftpCmdSTRU();}
  afx_msg void onSftpSYST()     {cmdTest.ftpCmdSYST();}
  afx_msg void onSftpTHMB()     {cmdTest.ftpCmdTHMB();}
  afx_msg void onSftpTYPE()     {cmdTest.ftpCmdTYPE();}
  afx_msg void onSftpUSER()     {cmdTest.ftpCmdUSER();}
  afx_msg void onSftpXCUP()     {cmdTest.ftpCmdXCUP();}
  afx_msg void onSftpXMKD()     {cmdTest.ftpCmdXMKD();}
  afx_msg void onSftpXPWD()     {cmdTest.ftpCmdXPWD();}
  afx_msg void onSftpXRMD()     {cmdTest.ftpCmdXRMD();}
  afx_msg void onSftpLogout()   {siteID.logout();}

  afx_msg void onSftpNotImpl()  {cmdTest.ftpCmdNotImpl();}

private:

  void    findDeletedFiles();
  void    prepPrevCmprDsp();

  bool    loadSiteDescriptors();

  String& dbPath(String& path) {return siteID.dbPath(path);}

  BOOL    OnSaveDocument(LPCTSTR lpszPathName);

  void    saveSite(DataSource src);

  bool    doEditSite();

  bool    loadSiteID(String& sectName)   {return siteID.loadData(sectName);}
  bool    loginSite()                    {return siteID.login();}
  void    logoutSite()                   {siteID.logout();}
  void    clearSiteID()                  {siteID.clear();}
  bool    pickSite()                     {return siteID.pickSite();}
  bool    editSite()                     {return siteID.edit();}
  bool    setCurrentSite()               {return siteID.setCurrentSite();}
  };



//       LRESULT onGetThrdMsg( WPARAM wParam, LPARAM lParam);
//       LRESULT onPickThrdMsg(WPARAM wParam, LPARAM lParam);
//          void finLoadSite();
//  void    startWkrThrd(AFX_THREADPROC thdProc, uint arg, int n);

//  bool    finWkrThrd(LPARAM lParam);

//  afx_msg void onGetSite();                   // start thread to do the hard work and
//          void finGetSite(LPARAM lParam);     // finish get site command
//          void finLoadSite(LPARAM lParam);    // finish Load Site

//  void         finConfirm(LPARAM lParam);


//          void confirmUpdate();
//  bool    put(SiteFileDsc& uf);
//  bool    get(SiteFileDsc& uf);
//  bool    del(SiteFileDsc& uf);
//  void    dspUpdates(bool onlyChkd = false);
//  afx_msg void onPrepDownLoadSite() {prepDwnLdCmd.start();}
//  bool    loadCurFileDscs();
//  void    cmprFileDsp(TCchar* prefix, TCchar* path, int& noFiles);

