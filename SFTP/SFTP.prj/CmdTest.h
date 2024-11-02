// FTP Command Tests -- Just scaffolding


#pragma once
#include "SftpSSL.h"
#include "SiteID.h"


class CmdTest {

String path;

public:

  CmdTest() { }
 ~CmdTest() { }

  void    ftpCmdACCT();          // Account information -- subset of access allowed by USER and PASSWORD
  void    ftpCmdADAT();          // Authentication/Security Data beyond USER and PASSWORD
  void    ftpCmdALLO(int n);     // Allocate sufficient disk space to receive a file.
  void    ftpCmdAPPE();          // Append local file to web file
  bool    sftpAPPE(void* param); // Append Thread body
  LRESULT onFinAPPEMsg(WPARAM wParam, LPARAM lParam);

  void    ftpCmdAUTH();          // Specify AUTHENTICATION/SECURITY MECHANISM
  void    ftpCmdAVBL();          // Return Available Space
  void    ftpCmdCCC();           // Clear Command Channel -- Part of the Security Command Set
  void    ftpCmdCDUP();          // Change to Parent Directory
  void    ftpCmdCONF();          // Confidentiality Protection Command
  void    ftpCmdCSID();          // Client / Server Identification
  void    ftpCmdCWD();           // Change working directory
  void    ftpCmdDELE();          // Delete file
  void    ftpCmdDSIZ();          // Get the directory size
  void    ftpCmdENC();           // Privacy Protected Channel
  void    ftpCmdEPRT() { }       // Specifies an extended address and port to which the server should connect
  void    ftpCmdEPSV() { }       // Enter extended passive mode
  void    ftpCmdFEAT();          // Get the feature list implemented by the server
  void    ftpCmdHELP();          // Returns usage documentation on a command if specified, else a general
                                 // help document is returned
  void    ftpCmdHOST() { }
  void    ftpCmdLANG() { }
  void    ftpCmdLIST();          // Returns information of a file or directory if specified, else
                                 // information of the current working directory is returned
  void    ftpCmdLPRT() { }
  void    ftpCmdLPSV() { }
  void    ftpCmdMDTM();          // Return the last-modified time of a specified file.
  void    ftpCmdMFCT();          // Modify the creation time of a file.
  void    ftpCmdMFF()  { }
  void    ftpCmdMFMT();          // Modify the last modification time of a file.
  void    ftpCmdMIC();           // Integrity Protected Command
  void    ftpCmdMKD();           // Make directory
  void    ftpCmdMLSD();          // Lists the contents of a directory in a standardized machine-readable
                                 // format
  void    ftpCmdMLST();          // Provides data about exactly the object named on its command line in a
                                 // standardized machine-readable format.
  void    ftpCmdMODE();          // Sets the transfer mode (Stream, Block, or Compressed).
  void    ftpCmdNLST();          // Returns a list of file names in a specified directory
  void    ftpCmdNOOP();          // No operation (dummy packet; used mostly on keepalives).
  void    ftpCmdOPTS();          // Select options for a feature
  void    ftpCmdPASS();
  void    ftpCmdPASV() { }
  void    ftpCmdPBSZ() { }
  void    ftpCmdPORT() { }
  void    ftpCmdPROT() { }
  void    ftpCmdPWD();           // Print working directory. Returns the current directory of the host.
  void    ftpCmdQUIT();          // Disconnect.
  void    ftpCmdREIN() { }
  void    ftpCmdREST() { }
  void    ftpCmdRETR();          // Retrieve a copy of the file
  void    ftpCmdRMD();           // Remove a directory.
  void    ftpCmdRMDA();          // Remove a directory tree
  void    ftpCmdRNFR();          // Rename from.
  void    ftpCmdRNTO();          // Rename to.
  void    ftpCmdSITE();          // Sends site specific commands to remote server
  void    ftpCmdSIZE();          // Return the size of a file.
  void    ftpCmdSMNT();          // Mount file structure
  void    ftpCmdSPSV();          // Use single port passive mode (only one TCP port number for both
                                 // control connections and passive-mode data connections)
  void    ftpCmdSTAT();          // Returns information on the server status, including the status of the
                                 // current connection

  void    ftpCmdSTOR();          // Accept the data and to store the data as a file at the server site
  bool    sftpSTOR(void* param); // Store Thread body
  LRESULT onFinSTORMsg(WPARAM wParam, LPARAM lParam);

  void    ftpCmdSTOU();          // Store file uniquely.
  LRESULT onFinSTOUMsg(  WPARAM wParam, LPARAM lParam);
  bool    sftpSTOU(void* param);

  void    ftpCmdSTRU();          // Set file transfer structure
  void    ftpCmdSYST();          // Return system type.
  void    ftpCmdTHMB();          // Get a thumbnail of a remote image file
  void    ftpCmdTYPE();          // Sets the transfer mode (ASCII/Binary).
  void    ftpCmdUSER();          // Authentication username
  void    ftpCmdXCUP();          // Change to the parent of the current working directory
  void    ftpCmdXMKD();          // Make a directory
  void    ftpCmdXPWD();          // Print the current working directory
  void    ftpCmdXRCP() { }
  void    ftpCmdXRMD();          // Remove the directory
  void    ftpCmdXRSQ() { }
  void    ftpCmdXSEM() { }
  void    ftpCmdXSEN() { }
  void    ftpCmdLogout();

  void    ftpCmdNotImpl();

private:

  bool    isLocked(bool prevent = false);
  bool    setupCmd();

  bool    login()     {return siteID.login();}
  bool    siteAvail() {return siteID.siteAvail();}

  bool    open(TCchar* host) {return sftpSSL.open(host);}
  bool    sendCmd(TCchar* cmmd, TCchar* args, String& response)
                                                          {return sftpSSL.sendCmd(cmmd, args, response);}
  bool    initPassiveMode(TCchar* cmmd, TCchar* arg)      {return sftpSSL.initPassiveMode(cmmd, arg);}

  SftpTransport& transport() {return sftpSSL.sftpTransport();}

  void    dspLines(String& s);
  void    dspDataLines();

  void    finishCmd();
  };


extern CmdTest cmdTest;




//  bool testSftpSTOR();            // Store Thread body


