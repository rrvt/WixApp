


#include "pch.h"
#include "CmdTest.h"
#include "FileName.h"
#include "GetPathDlg.h"
#include "NotePad.h"
#include "Resource.h"
#include "SFTP.h"
#include "SFTPDoc.h"
#include "SftpDataIter.h"
#include "Utilities.h"
#include "WorkerThrd.h"



CmdTest cmdTest;

UINT sftpAPPEThrd(LPVOID param);
UINT sftpSTORThrd(LPVOID param);
UINT sftpSTOUThrd(LPVOID param);

//UINT testSftpSTORThrd(LPVOID param);


// Authentication/Security Data beyond USER and PASSWORD

/*
 ACCT FTP command

The ACCT command is used to provide an account name to the server. The account is not necessarily related to the USER . A client may log in using their username and password, however, an account may be required for specific access to server resources.

Common responses:

    202 , 230
    421
    500 , 502 , 503 , 550

ACCOUNT (ACCT) -- From RFC 959:

    The argument field is a Telnet string identifying the user's
    account.  The command is not necessarily related to the USER
    command, as some sites may require an account for login and
    others only for specific access, such as storing files.  In
    the latter case the command may arrive at any time.

    There are reply codes to differentiate these cases for the
    automation: when account information is required for login,
    the response to a successful PASSword command is reply code
    332.  On the other hand, if account information is NOT
    required for login, the reply to a successful PASSword
    command is 230; and if the account information is needed for
    a command issued later in the dialogue, the server should
    return a 332 or 532 reply depending on whether it stores
    (pending receipt of the ACCounT command) or discards the
    command, respectively.
*/

void CmdTest::ftpCmdACCT() {
String s;
String user;   siteID.userID >> user;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("ACCT"), user, s)) dspLines(s);   finishCmd();
  }


// Authentication/Security Data beyond USER and PASSWORD

/*
If the server requires additional security information, then the
client and server will enter into a security data exchange.  The
client will send an ADAT command containing the first block of
security data.  The server's reply will indicate if the data exchange
is complete, if there was an error, or if more data is needed.  The
server's reply can optionally contain security data for the client to
interpret.  If more data is needed, the client will send another ADAT
command containing the next block of data, and await the server's
reply.  This exchange can continue as many times as necessary.  Once
this exchange completes, the client and server have established a
security association.  This security association may include
authentication (client, server, or mutual) and keying information for
integrity and/or confidentiality, depending on the mechanism in use.

The term "security data" here is carefully chosen.  The purpose of
the security data exchange is to establish a security association,
which might not actually include any authentication at all, between
the client and the server as described above.  For instance, a
Diffie-Hellman exchange establishes a secret key, but no
authentication takes place.  If an FTP server has an RSA key pair but
the client does not, then the client can authenticate the server, but
the server cannot authenticate the client.                                _T("rrvt")
*/

void CmdTest::ftpCmdADAT() {
String s;
String user;   siteID.userID >> user;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("ADAT"), user, s)) dspLines(s);   finishCmd();
  }


// Allocate sufficient disk space to receive a file.
/*
 ALLO FTP command

The ALLO command may be sent to a server that requires the necessary space
for an uploaded to be reserved before the transfer takes place. The argument
shall be a decimal integer representing the number of bytes (using the
logical byte size) of storage to be reserved for the file. A server that does
not require space to be reserved in advance should treat the command as a NOOP
operation.
*/

void CmdTest::ftpCmdALLO(int n) {
String nn(n);
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("allo"), nn, s)) dspLines(s);   finishCmd();
  }


// Append local file to web file
/*
 APPE FTP command

A client issue the APPE command after successfully establishing a data connection
when it wishes to upload data to the server. The client provides the file name it
wishes to use for the upload. If the file already exists on the server, the data
is appended to the existing file. If the file does not exist, it is created.

APPEND (with create) (APPE)

This command causes the server-DTP to accept the data
transferred via the data connection and to store the data in
a file at the server site.  If the file specified in the
pathname exists at the server site, then the data shall be
appended to that file; otherwise the file specified in the
pathname shall be created at the server site.
*/

void CmdTest::ftpCmdAPPE() {
PathDlgDsc dsc = {_T("Test File"), _T("test.txt"), _T("txt"), _T("*.txt")};
String     dst = siteID.remoteRoot + _T("test.txt");

  if (isLocked()) return;

  setupCmd();

  if (getOpenDlg(dsc, path) && login()) workerThrd.start(sftpAPPEThrd, (void*) dst.str(), ID_FinAPPEMsg);

  finishCmd();
  }


// bool start(AFX_THREADPROC thdProc, void* arg, int msg);

// Start Append Thread

UINT sftpAPPEThrd(void* param) {
#if 1

  return cmdTest.sftpAPPE(param);

#else

SFTPDoc& d     = *doc();
HWND       hWnd  = mainFrm()->m_hWnd;
uint       msgID = (uint) param;
uint       rslt;
SiteFileDsc   uf;
String     web;
                                                                // successfully completed
setLogging(_T("APPE"), _T("Set"));

  try {cmdTest.sftpAPPE();   rslt = TE_Normal;}
  catch (...) {              rslt = TE_Exception;}

clrLogging(_T("APPE"), _T("Clr"));

  ::PostMessage(hWnd, msgID, ID_EndThread, rslt);   sftpSSL.openSSLThreadStop();   return 0;
#endif
  }


// Append Thread Body

bool CmdTest::sftpAPPE(void* param) {
String dst = (TCchar*) param;

  if (!sftpSSL.getLocalFile(path)) return false;

  return sftpSSL.append(toRemotePath(dst));
  }


LRESULT CmdTest::onFinAPPEMsg(WPARAM wParam, LPARAM lParam) {return 0;}


// Accept the data and to store the data as a file at the server site
/*
STORE (STOR)

This command causes the server?DTP to accept the data
transferred via the data connection and to store the data as
a file at the server site. If the file specified in the
pathname exists at the server site, then its contents shall
be replaced by the data being transferred. A new file is
created at the server site if the file specified in the
pathname does not already exist.
*/

void CmdTest::ftpCmdSTOR() {
PathDlgDsc dsc = {_T("Test File"), _T("test.txt"), _T("txt"), _T("*.txt")};

  if (isLocked()) return;

  if (setupCmd() && getOpenDlg(dsc, path))
                                        workerThrd.start(sftpSTORThrd, (void*) path.str(), ID_FinSTORMsg);
  finishCmd();
  }


UINT sftpSTORThrd(void* param) {
#if 1

  cmdTest.sftpSTOR(param);  return 0;

#else

HWND     hWnd  = mainFrm()->m_hWnd;
uint     msgID = (uint) param;
uint     rslt  = false;

  try {cmdTest.sftpSTOR();   rslt = TE_Normal;}
  catch (...) {                  rslt = TE_Exception;}

  ::PostMessage(hWnd, msgID, ID_EndThread, rslt);   sftpSSL.openSSLThreadStop();   return 0;
#endif
  }



// Store Thread body

bool CmdTest::sftpSTOR(void* param) {
String path = (TCchar*) param;
String dst;

  if (!sftpSSL.getLocalFile(path)) return false;

  dst = removePath(path);   dst = siteID.remoteRoot + dst;

  return sftpSSL.stor(toRemotePath(dst));
  }


LRESULT CmdTest::onFinSTORMsg(WPARAM wParam, LPARAM lParam) {return 0;}


UINT sftpSTOUThrd(void* param) {
#if 1

  cmdTest.sftpSTOU(param);   return 0;

#else
SFTPDoc& d     = *doc();
HWND       hWnd  = mainFrm()->m_hWnd;
uint       msgID = (uint) param;
uint       rslt;
SiteFileDsc   uf;
String     web;

  try {   rslt = TE_Normal;}
  catch (...) {              rslt = TE_Exception;}

  ::PostMessage(hWnd, msgID, ID_EndThread, rslt);   sftpSSL.openSSLThreadStop();   return 0;
#endif
  }


// Store file uniquely.
/*
STORE UNIQUE (STOU)
This command behaves like STOR except that the resultant
file is to be created in the current directory under a name
unique to that directory. The 250 Transfer Started response
must include the name generated.
*/

void CmdTest::ftpCmdSTOU() {
PathDlgDsc dsc = {_T("Test File"), _T("test.txt"), _T("txt"), _T("*.txt")};

  setupCmd();

    if (getOpenDlg(dsc, path) && login())
                                        workerThrd.start(sftpSTOUThrd, (void*) path.str(), ID_FinSTOUMsg);
  finishCmd();
  }


LRESULT CmdTest::onFinSTOUMsg(  WPARAM wParam, LPARAM lParam) {return 0;}


// Store Uniquely Thread body

bool CmdTest::sftpSTOU(void* param) {
String fileName;

  if (!sftpSSL.getLocalFile(path)) return false;

  if (!sftpSSL.stou(path, fileName)) return false;

  notePad << _T("New File Name: ") << fileName << nCrlf;   return true;
  }




/*
Specify AUTHENTICATION/SECURITY MECHANISM (AUTH)

The argument field is a Telnet string identifying a supported
mechanism.  This string is case-insensitive.  Values must be
registered with the IANA, except that values beginning with "X-"
are reserved for local use.

If the server does not recognize the AUTH command, it must respond
with reply code 500.  This is intended to encompass the large
deployed base of non-security-aware ftp servers, which will
respond with reply code 500 to any unrecognized command.  If the
server does recognize the AUTH command but does not implement the
security extensions, it should respond with reply code 502.

If the server does not understand the named security mechanism, it
should respond with reply code 504.

If the server is not willing to accept the named security
mechanism, it should respond with reply code 534.

If the server is not able to accept the named security mechanism,
such as if a required resource is unavailable, it should respond
with reply code 431.

If the server is willing to accept the named security mechanism,
but requires security data, it must respond with reply code 334.

If the server is willing to accept the named security mechanism,
and does not require any security data, it must respond with reply
code 234.

If the server is responding with a 334 reply code, it may include
security data as described in the next section.

Some servers will allow the AUTH command to be reissued in order
to establish new authentication.  The AUTH command, if accepted,
removes any state associated with prior FTP Security commands.
The server must also require that the user reauthorize (that is,
reissue some or all of the USER, PASS, and ACCT commands) in this
case (see section 4 for an explanation of "authorize" in this
context).
*/

void CmdTest::ftpCmdAUTH() {
String s;

  if (isLocked()) return;

  siteAvail();   if (!sftpSSL.open(siteID.url)) return;

  if (sftpSSL.sendCmd(_T("AUTH"), _T("TTT"), s)) dspLines(s);
  if (sftpSSL.sendCmd(_T("AUTH"), _T("SSL"), s)) dspLines(s);
  if (sftpSSL.sendCmd(_T("AUTH"), _T("TLS"), s)) dspLines(s);

  finishCmd();
  }


void CmdTest::ftpCmdAVBL() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("AVBL"), 0, s)) dspLines(s);   finishCmd();
  }


// Clear Command Channel -- Part of the Security Command Set
/*
CLEAR COMMAND CHANNEL (CCC)

This command does not take an argument.

It is desirable in some environments to use a security mechanism
to authenticate and/or authorize the client and server, but not to
perform any integrity checking on the subsequent commands.  This
might be used in an environment where IP security is in place,
insuring that the hosts are authenticated and that TCP streams
cannot be tampered, but where user authentication is desired.

If unprotected commands are allowed on any connection, then an
attacker could insert a command on the control stream, and the
server would have no way to know that it was invalid.  In order to
prevent such attacks, once a security data exchange completes
successfully, if the security mechanism supports integrity, then
integrity (via the MIC or ENC command, and 631 or 632 reply) must
be used, until the CCC command is issued to enable non-integrity
protected control channel messages.  The CCC command itself must
be integrity protected.

Once the CCC command completes successfully, if a command is not
protected, then the reply to that command must also not be
protected.  This is to support interoperability with clients which
do not support protection once the CCC command has been issued.

This command must be preceded by a successful security data
exchange.

If the command is not integrity-protected, the server must respond
with a 533 reply code.

If the server is not willing to turn off the integrity
requirement, it should respond with a 534 reply code.

Otherwise, the server must reply with a 200 reply code to indicate
that unprotected commands and replies may now be used on the
command channel.
*/

void CmdTest::ftpCmdCCC() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("CCC"), 0, s)) dspLines(s);   finishCmd();
  }


/*
The following new optional commands are introduced in this specification:

AUTH (Authentication/Security Mechanism),
ADAT (Authentication/Security Data),
CCC (Clear Command Channel),
CONF (Confidentiality Protected Command),
ENC (Privacy Protected Command).
MIC (Integrity Protected Command),
PBSZ (Protection Buffer Size),
PROT (Data Channel Protection Level),
*/

void CmdTest::ftpCmdCONF() {

  if (isLocked()) return;

  notePad << _T("Security Command, Not Tested Here") << nCrlf;   finishCmd();
  }

void CmdTest::ftpCmdENC()  {ftpCmdCONF();}
void CmdTest::ftpCmdMIC()  {ftpCmdCONF();}


// Client / Server Identification
/*
CSID FTP command

The CSID command is used to identify the client software to the server. The response is used to identify the server to the client and provide important information about the server to the client. This command supersedes the CLNT command. When the command is sent to the server, the client must identify itself: CSID Name=FTP Voyager; Version=15.0.0.2.

Overview

The CSID command is used to identify the client software to the server. The response is used to identify the server to the client and provide important information about the server to the client. This command supersedes the CLNT command. When the command is sent to the server, the client must identify itself:

CSID Name=FTP Voyager; Version=15.0.0.2;

Name and Version are the only parameters supported. The server responds with information helpful to the client, for example:

200 Name=Serv-U; Version=7.0.0.5; OS=Windows XP; OSVer=5.1.2600; CaseSensitive=0; DirSep=/;

The CaseSensitive flag is used to indicate to the client whether or not the server's platform uses a case sensitive file system. The DirSep parameter is used to inform the client of the character used to separate directories on the server's file system.

Only the CaseSensitive and DirSep parameters are required in the server's response. The remaining parameters are optional and may not be provided if the server administrator would rather keep that information private.
Common responses

    200 FTP response code
    500 FTP response code , 502 FTP response code
*/


void CmdTest::ftpCmdCSID() {
String s;

  if (isLocked()) return;

  setupCmd();

  if (sftpSSL.sendCmd(_T("CSID"), _T("Name=FtpApp; Version=1.0.8;"), s)) dspLines(s);

  finishCmd();
  }


// Change to Parent Directory
/*
The CDUP command causes the server to change the client’s current working directory to the immediate parent directory of the current working directory. For example, if a client’s current working directory is /usr/files , issuing the CDUP command will change the working directory to /usr .
*/

void CmdTest::ftpCmdCDUP() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("CDUP"), 0, s)) dspLines(s);   finishCmd();
  }


// Change working directory
/*
CHANGE WORKING DIRECTORY (CWD)
This command allows the user to work with a different
directory or dataset for file storage or retrieval without
altering his login or accounting information. Transfer
parameters are similarly unchanged. The argument is a
pathname specifying a directory or other system dependent
file group designator.
*/

void CmdTest::ftpCmdCWD() {
String s;

  if (isLocked()) return;

  setupCmd();    path = siteID.localRoot;

    if (!getDirPathDlg(_T("Change Working Dir Corresponding to:"), path)) return;

    if (path.find(siteID.localRoot) >= 0)
                                        path = siteID.remoteRoot + path.substr(siteID.localRoot.length());

    if (sftpSSL.sendCmd(_T("CWD"), toRemotePath(path), s)) dspLines(s);

  finishCmd();
  }


/*
MAKE DIRECTORY (MKD)

MKD <SP> <pathname> <CRLF>

This command causes the directory specified in the pathname
to be created as a directory (if the pathname is absolute)
or as a subdirectory of the current working directory (if
the pathname is relative). See Appendix II.
*/

void CmdTest::ftpCmdMKD()
{String s;   setupCmd();   if (sftpSSL.sendCmd(_T("MKD"), _T("/testDir"), s)) dspLines(s);   finishCmd();}


// Remove a directory.
/*
REMOVE DIRECTORY (RMD)

RMD <SP> <pathname> <CRLF>

This command causes the directory specified in the pathname
to be removed as a directory (if the pathname is absolute)
or as a subdirectory of the current working directory (if
the pathname is relative). See Appendix II.
*/

void CmdTest::ftpCmdRMD()
{String s;   setupCmd();   if (sftpSSL.sendCmd(_T("RMD"), _T("/testDir"), s)) dspLines(s);   finishCmd();}


// Remove a directory tree

void CmdTest::ftpCmdRMDA() {
String s;   setupCmd();   if (sftpSSL.sendCmd(_T("RMDA"), _T("/testDir"), s)) dspLines(s);   finishCmd();
}


// Change to the parent of the current working directory

void CmdTest::ftpCmdXCUP()
            {String s;   setupCmd();   if (sftpSSL.sendCmd(_T("XCUP"), 0, s)) dspLines(s);   finishCmd();}


// Make a directory
/*
To solve these problems, upon successful completion of an XMKD
command, the server should return a line of the form:

  257<space>"<directory?name>"<space><commentary>

That is, the server will tell the user what string to use when
referring to the created directory. The directory name can
contain any character; embedded double?quotes should be escaped
by double?quotes (the "quote?doubling" convention).

For example, a user connects to the directory /usr/dm, and
creates a subdirectory, named child:

  XCWD /usr/dm
  200 directory changed to /usr/dm
  XMKD child
  257 "/usr/dm/child" directory created

An example with an embedded double quote:

  XMKD foo"bar
  257 "/usr/dm/foo""bar" directory created
  XCWD /usr/dm/foo"bar
  200 directory changed to /usr/dm/foo"bar

We feel that the prior existence of a subdirectory with the same
name should be interpreted as an error, and have implemented our
server to give an "access denied" error reply in that case.

  CWD /usr/dm
  200 directory changed to /usr/dm
  XMKD child
  521?"/usr/dm/child" directory already exists;
  521 taking no action.

We recommend that failure replies for XMKD be analogous to its
file creating cousin, STOR. Also, we recommend that an "access
denied" return be given if a file name with the same name as the
subdirectory will conflict with the creation of the subdirectory
(this is a problem on Unix, but shouldn't be one on Tops?20).

Essentially because the XPWD command returns the same type of
information as the successful XMKD command, we have implemented
the successful XPWD command to use the 257 reply code as well.
*/

void CmdTest::ftpCmdXMKD()
{String s;   setupCmd();   if (sftpSSL.sendCmd(_T("XMKD"), _T("testDir"), s)) dspLines(s);   finishCmd();}


// Print the current working directory
/*
XPWD print current working directory
  257 (251) "pathname"
  425 (451) random file system error
  506 (502) action not implemented
*/

void CmdTest::ftpCmdXPWD()
            {String s;   setupCmd();   if (sftpSSL.sendCmd(_T("XPWD"), 0, s)) dspLines(s);   finishCmd();}


// Remove the directory
/*
XRMD remove directory

  224 (250) deleted ok
  506 (502) action not implemented
  521 (450) access denied
  550 (501) bad pathname syntax or ambiguous
  425 (451) random file system error
*/

void CmdTest::ftpCmdXRMD()
{String s;   setupCmd();   if (sftpSSL.sendCmd(_T("XRMD"), _T("testDir"), s)) dspLines(s);   finishCmd();}


// Delete file
/*
DELETE (DELE)
This command causes the file specified in the pathname to be
deleted at the server site. If an extra level of protection
is desired (such as the query, "Do you really wish to
delete?"), it should be provided by the user?FTP process.
*/

void CmdTest::ftpCmdDELE() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("DELE"), _T("test00.txt"), s)) dspLines(s);   finishCmd();
  }


// Get the directory size

void CmdTest::ftpCmdDSIZ() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("DSIZ"), 0, s)) dspLines(s);   finishCmd();
  }


// Get the feature list implemented by the server

void CmdTest::ftpCmdFEAT() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("FEAT"), 0, s)) dspLines(s);   finishCmd();
  }


// Returns usage documentation on a command if specified, else a general help document is returned
/*
HELP (HELP)
This command shall cause the server to send helpful
information regarding its implementation status over the
control connection to the user. The command may take an
argument (e.g., any command name) and return more specific
information as a response. The reply is type 211 or 214.
It is suggested that HELP be allowed before entering a USER
command. The server may use this reply to specify
site?dependent parameters, e.g., in response to HELP SITE.
*/

void CmdTest::ftpCmdHELP() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("HELP"), _T("LIST"), s)) dspLines(s);   finishCmd();
  }


// Returns information of a file or directory if specified, else information of the current working
// directory is returned
/*
LIST (LIST)
This command causes a list to be sent from the server to the
passive DTP. If the pathname specifies a directory or other
group of files, the server should transfer a list of files
in the specified directory. If the pathname specifies a
file then the server should send current information on the
file. A null argument implies the user's current working or
default directory. The data transfer is over the data
connection in type ASCII or type EBCDIC. (The user must
ensure that the TYPE is appropriately ASCII or EBCDIC).
Since the information on a file may vary widely from system
to system, this information may be hard to use automatically
in a program, but may be quite useful to a human user.
*/


void CmdTest::ftpCmdLIST() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("TYPE A"), 0, s)) dspLines(s);

    if (!sftpSSL.initPassiveMode(_T("LIST"), 0)) return;

    sftpSSL.sftpTransport().read(AsciiFlTyp);   sftpSSL.sftpTransport().closeSkt();   dspDataLines();

  finishCmd();
  }


// Lists the contents of a directory in a standardized machine-readable format
/*
Format of MLSx Requests

The MLST and MLSD commands each allow a single optional argument.
This argument may be either a directory name or, for MLST only, a
file name. For these purposes, a "file name" is the name of any
entity in the server NVFS which is not a directory. Where TVFS is
supported, any TVFS relative pathname valid in the current working
directory, or any TVFS fully qualified pathname, may be given. If a
directory name is given then MLSD must return a listing of the
contents of the named directory, otherwise it issues a 501 reply, and
does not open a data connection. In all cases for MLST, a single set
of fact lines (usually a single fact line) containing the information
about the named file or directory shall be returned over the control
connection, without opening a data connection.

If no argument is given then MLSD must return a listing of the
contents of the current working directory, and MLST must return a
listing giving information about the current working directory
itself. For these purposes, the contents of a directory are whatever

If the Client?FTP sends an invalid argument, the server?FTP MUST
reply with an error code of 501.

The syntax for the MLSx command is:

  mlst = "MLst" [ SP pathname ] CRLF
  mlsd = "MLsD" [ SP pathname ] CRLF

Format of MLSx Response

The format of a response to an MLSx command is as follows:

  mlst?response    = control?response / error?response
  mlsd?response    = ( initial?response final?response ) / error?response

  control?response = "250?" [ response?message ] CRLF
                     1*( SP entry CRLF )
                     "250" [ SP response?message ] CRLF

  initial?response = "150" [ SP response?message ] CRLF
  final?response   = "226" SP response?message CRLF

  response?message = *TCHAR

  data?response    = *( entry CRLF )
  entry            = [ facts ] SP pathname
  facts            = 1*( fact ";" )
  fact             = factname "=" value
  factname         = "Size" / "Modify" / "Create" /
                     "Type" / "Unique" / "Perm" /
                     "Lang" / "Media?Type" / "CharSet" /
                     os?depend?fact / local?fact

  os?depend?fact   = <IANA assigned OS name> "." token
  local?fact       = "X." token
  value            = *SCHAR

Upon receipt of an MLSx command, the server will verify the
parameter, and if invalid return an error?response. For this
purpose, the parameter should be considered to be invalid if the
client issuing the command does not have permission to perform the
requested operation.

If the parameter is valid, then for an MLST command, the server?PI
will send the first (leading) line of the control response, the entry
for the pathname given, or the current directory if no pathname was
provided, and the terminating line. Normally exactly one entry would
be returned, more entries are permitted only when required to
represent a file that is to have multiple "Type" facts returned. In
this case, the pathname component of every response MUST be
identical.

Note that for MLST the fact set is preceded by a space. That is
provided to guarantee that the fact set cannot be accidentally
interpreted as the terminating line of the control response, but is
required even when that would not be possible. Exactly one space
exists between the set of facts and the pathname. Where no facts are
present, there will be exactly two leading spaces before the
pathname. No spaces are permitted in the facts, any other spaces in
the response are to be treated as being a part of the pathname.
If the command was an MLSD command, the server will open a data
connection as indicated in section 3.2 of STD 9, RFC 959 [3]. If
that fails, the server will return an error?response. If all is OK,
the server will return the initial?response, send the appropriate
data?response over the new data connection, close that connection,
and then send ttouhe final?response over the control connection. The
grammar above defines the format for the data?response, which defines
the format of the data returned over the data connection established.

The data connection opened for a MLSD response shall be a connection
as if the "TYPE L 8", "MODE S", and "STRU F" commands had been given,
whatever FTP transfer type, mode and structure had actually been set,
and without causing those settings to be altered for future commands.
That is, this transfer type shall be set for the duration of the data
connection established for this command only. While the content of
the data sent can be viewed as a series of lines, implementations
should note that there is no maximum line length defined.
Implementations should be prepared to deal with arbitrarily long
lines.

The facts part of the specification would contain a series of "file
facts" about the file or directory named on the same line. Typical
information to be presented would include file size, last
modification time, creation time, a unique identifier, and a
file/directory flag.

The complete format for a successful reply to the MLSD command would
be:

  facts SP pathname CRLF
  facts SP pathname CRLF
  facts SP pathname CRLF
  ...

Note that the format is intended for machine processing, not human
viewing, and as such the format is very rigid. Implementations MUST
NOT vary the format by, for example, inserting extra spaces for
readability, replacing spaces by tabs, including header or title
lines, or inserting blank lines, or in any other way alter this
format. Exactly one space is always required after the set of facts
(which may be empty). More spaces may be present on a line if, and
only if, the pathname presented contains significant spaces. The set
of facts must not contain any spaces anywhere inside it. Facts
should be provided in each output line only if they both provide
relevant information about the file named on the same line, and they
are in the set requested by the user?PI. See section 7.9 (page 51).
There is no requirement that the same set of facts be provided for
each file, or that the facts presented occur in the same order for
each file.

Error Responses to MLSx commands

Many of the 4xy and 5xy responses defined in section 4.2 of STD 9,
RFC 959 [3] are possible in response to the MLST and MLSD commands.
In particular, syntax errors can generate 500 or 501 replies. Giving
a pathname that exists but is not a directory as the argument to a
MLSD command generates a 501 reply. Giving a name that does not
exist, or for which access permission (to obtain directory
information as requested) is not granted will elicit a 550 reply.
Other replies (530, 553, 503, 504, and any of the 4xy replies) are
also possible in appropriate circumstances.

Standard Facts

This document defines a standard set of facts as follows:

  size       -- Size in octets
  modify     -- Last modification time
  create     -- Creation time
  type       -- Entry type
  unique     -- Unique id of file/directory
  perm       -- File permissions, whether read, write, execute is allowed for the login id.
  lang       -- Language of the file name per IANA [11] registry.
  media-type -- MIME media?type of file contents per IANA registry.
  charset    -- Character set per IANA registry (if not UTF?8)

Fact names are case-insensitive. Size, size, SIZE, and SiZe are the same fact.

Further operating system specific keywords could be specified by
using the IANA operating system name as a prefix (examples only):
  OS/2.ea    -- OS/2 extended attributes
  MACOS.rf   -- MacIntosh resource forks
  UNIX.mode  -- Unix file modes (permissions)

See RFC 3659 for additional details
*/

void CmdTest::ftpCmdMLSD() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("TYPE A"), 0, s)) dspLines(s);

    if (!sftpSSL.initPassiveMode(_T("MLSD"), 0)) return;

    sftpSSL.sftpTransport().read(AsciiFlTyp);   sftpSSL.sftpTransport().closeSkt();   dspDataLines();

  finishCmd();
  }


// Provides data about exactly the object named on its command line in a standardized machine-readable
// format.  See comment for MLSD and RFC 3659

void CmdTest::ftpCmdMLST() {
String s;

  if (isLocked()) return;

  setupCmd();   notePad << nFFace(_T("Courier New"));

  if (sftpSSL.sendCmd(_T("MLST"),              0, s)) dspLines(s);     s.clear();
  if (sftpSSL.sendCmd(_T("MLST"), _T("test.txt"), s)) dspLines(s);

  finishCmd();   notePad << nFont;
  }


// Returns a list of file names in a specified directory

void CmdTest::ftpCmdNLST() {
String s;

  if (isLocked()) return;

  setupCmd();   notePad << nFFace(_T("Courier New"));

  if (sftpSSL.sendCmd(_T("TYPE A"), 0, s)) dspLines(s);    s.clear();

  if (!sftpSSL.initPassiveMode(_T("NLST"), 0)) return;

  sftpSSL.sftpTransport().read(AsciiFlTyp);   sftpSSL.sftpTransport().closeSkt();   dspDataLines();

    if (sftpSSL.sendCmd(_T("TYPE A"), 0, s)) dspLines(s);

    if (!sftpSSL.initPassiveMode(_T("NLST"), _T("/Modules"))) return;

    sftpSSL.sftpTransport().read(AsciiFlTyp);   sftpSSL.sftpTransport().closeSkt();   dspDataLines();

  finishCmd();   notePad << nFont;
  }


/*
File Modification Time (MDTM)
The FTP command, MODIFICATION TIME (MDTM), can be used to determine
when a file in the server NVFS was last modified. This command has
existed in many FTP servers for many years, as an adjunct to the REST
command for STREAM mode, thus is widely available. However, where
supported, the "modify" fact that can be provided in the result from
the new MLST command is recommended as a superior alternative.

When attempting to restart a RETRieve, the user?FTP can use the MDTM
command or the "modify" fact to check if the modification time of the
source file is more recent than the modification time of the
partially transferred file. If it is, then most likely the source
file has changed, and it would be unsafe to restart the previously
incomplete file transfer.

Because the user? and server?FTPs' clocks are not necessarily
synchronised, user?FTPs intending to use this method should usually
obtain the modification time of the file from the server before the
initial RETRieval, and compare that with the modification time before
a RESTart. If they differ, the files may have changed, and RESTart
would be inadvisable. Where this is not possible, the user?FTP
should make sure to allow for possible clock skew when comparing
times.

When attempting to restart a STORe, the User FTP can use the MDTM
command to discover the modification time of the partially
transferred file. If it is older than the modification time of the
file that is about to be STORed, then most likely the source file has
changed, and it would be unsafe to restart the file transfer.

Note that using MLST (described below), where available, can provide
this information and much more, thus giving an even better indication
that a file has changed and that restarting a transfer would not give
valid results.

Note that this is applicable to any RESTart attempt, regardless of
the mode of the file transfer.

The syntax for the MDTM command is:  mdtm = "MdTm" SP pathname CRLF

mdtm?response = "213" SP time?val CRLF / error?response

Error Responses:

4xy replies are also possible
500 or 501 -- parsing issue
550 -- entity not available (doesn't exist or cannot be queried)
*/

void CmdTest::ftpCmdMDTM() {
String path = _T("/test00.txt");
Date   dt;
Date   dt1;

  if (isLocked()) return;

  setupCmd();
#if 1
    sftpSSL.date(path, dt);    notePad << _T("Local: ") << dt << nCrlf;
#else
String s;
int    code;
uint   pos;
//CTimeSpan offset(0, -7, 0, 0);

    if (!sendCmd(_T("MDTM"), _T("/test00.txt"), s)) return;   dspLines(s);

    code = s.stoi(pos);  if (code != 213 || s[pos] != _T(' ')) return;

    try {ToDate to(s.substr(pos+1));    dt = to();}
    catch (...) {notePad << _T("Failed Date Interpretation: ") << s << nCrlf;}
           //<< _T("GMT: ") << dt
#endif

  finishCmd();
  }


// MFCT --Modify the creation time of a file.
// MFF -- Modify fact (the last modification time, creation time, UNIX group/owner/mode of a file).
/*
MFCT FTP command

The MFCT command is used to modify a file or folder's creation date and time information. When a file
or folder is uploaded to an FTP server, the creation date and time of the file or folder is set to the
transfer date and time. Using MFCT, FTP clients can inform supporting FTP servers of the proper creation
date and time to use for the file or folder.

Overview

This article describes the use of the Modify Fact: Creation Time (MFCT) FTP command.

The MFCT command is used to modify a file or folder's creation date and time information.

When a file or folder is uploaded to an FTP server, the creation date and time of the file or folder is
set to the transfer date and time. Using MFCT, FTP clients can inform supporting FTP servers of the
proper creation date and time to use for the file or folder.

The format of the command is MFCT YYYYMMDDHHMMSS path, where:

    YYYY - the 4-digit year
    MM - the 2-digit month
    DD - the 2-digit day of the month
    HH - the hour in 24-hour format
    MM - the minute
    SS - the seconds

All times are represented in GMT/UTC.

Common responses

    213 FTP response code, 253 FTP response code
    500 FTP response code, 501 FTP response code, 502 FTP response code,
    530 FTP response code, 550 FTP response code
*/

void CmdTest::ftpCmdMFCT() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("MFCT"), _T("20230430010203 /test.txt"), s)) dspLines(s);   s.clear();
    if (sftpSSL.sendCmd(_T("MFF"),  _T("20230430010203 /test.txt"), s)) dspLines(s);   s.clear();

  finishCmd();
  }


// Modify the last modification time of a file.
// Syntax same as MFCT above

void CmdTest::ftpCmdMFMT() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("MFMT"), _T("20230430020304 /test.txt"), s)) dspLines(s);

  finishCmd();
  }


// Sets the transfer mode (Stream, Block, or Compressed).
/*
TRANSFER MODE (MODE)
The argument is a single Telnet character code specifying
the data transfer modes described in the Section on
Transmission Modes.

The following codes are assigned for transfer modes:

  S - Stream
  B - Block
  C - Compressed

The default transfer mode is Stream.
*/

void CmdTest::ftpCmdMODE() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("MODE"), _T("S"), s)) dspLines(s);   finishCmd();
  }


// Set file transfer structure
/*
FILE STRUCTURE (STRU)

The argument is a single Telnet character code specifying
file structure described in the Section on Data
Representation and Storage.

The following codes are assigned for structure:

  F - File (no record structure)
  R - Record structure
  P - Page structure

The default structure is File.
*/

void CmdTest::ftpCmdSTRU() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("STRU"), _T("F"), s)) dspLines(s);   finishCmd();
  }



// No operation (dummy packet; used mostly on keepalives).

void CmdTest::ftpCmdNOOP() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("NOOP"), 0, s)) dspLines(s);   finishCmd();
  }


// Select options for a feature

void CmdTest::ftpCmdOPTS() {
String s;

  if (isLocked()) return;

  setupCmd();
    if (sftpSSL.sendCmd(_T("OPTS"), _T("MLST type*;size*;modify*;"), s)) dspLines(s);
  finishCmd();
  }


// Print working directory. Returns the current directory of the host.

void CmdTest::ftpCmdPWD() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("PWD"), 0, s)) dspLines(s);   finishCmd();
  }


/*
LOGOUT (QUIT)
This command terminates a USER and if file transfer is not
in progress, the server closes the control connection. If
file transfer is in progress, the connection will remain
open for result response and the server will then close it.
If the user?process is transferring files for several USERs
but does not wish to close and then reopen connections for
each, then the REIN command should be used instead of QUIT.

An unexpected close on the control connection will cause the
server to take the effective action of an abort (ABOR) and a
logout (QUIT).
*/

void CmdTest::ftpCmdQUIT() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("QUIT"), 0, s)) dspLines(s);   finishCmd();
  }


// Retrieve a copy of the file

void CmdTest::ftpCmdRETR() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (!sendCmd(_T("TYPE"), _T("A"), s)) return;   dspLines(s);   s.clear();

    if (!initPassiveMode(_T("RETR"), _T("/test.txt"))) return;

    transport().clear();   transport().read(AsciiFlTyp);   transport().closeSkt();    dspLines(s);

    dspDataLines();

  finishCmd();
  }

//Rename from.
//Rename to.
/*
RENAME FROM (RNFR)
This command specifies the old pathname of the file which is
to be renamed. This command must be immediately followed by
a "rename to" command specifying the new file pathname.
RENAME TO (RNTO)
This command specifies the new pathname of the file
specified in the immediately preceding "rename from"
command. Together the two commands cause a file to be
renamed.
*/


void CmdTest::ftpCmdRNFR() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("RNFR"), _T("/test.txt"), s)) dspLines(s);

  finishCmd();
  }


void CmdTest::ftpCmdRNTO() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("RNTO"), _T("/testDir/test.txt"), s)) dspLines(s);

  finishCmd();
  }


// Sends site specific commands to remote server

void CmdTest::ftpCmdSITE() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("SITE"), _T("HELP"), s)) dspLines(s);

  finishCmd();
  }


// Return the size of a file.
/*
File SIZE

The FTP command, SIZE OF FILE (SIZE), is used to obtain the transfer
size of a file from the server?FTP process. This is the exact number
of octets (8 bit bytes) that would be transmitted over the data
connection should that file be transmitted. This value will change
depending on the current STRUcture, MODE, and TYPE of the data
connection or of a data connection that would be created were one
created now. Thus, the result of the SIZE command is dependent on
the currently established STRU, MODE, and TYPE parameters.

The SIZE command returns how many octets would be transferred if the
file were to be transferred using the current transfer structure,
mode, and type. This command is normally used in conjunction with
the RESTART (REST) command when STORing a file to a remote server in
STREAM mode, to determine the restart point. The server?PI might
need to read the partially transferred file, do any appropriate
conversion, and count the number of octets that would be generated
when sending the file in order to correctly respond to this command.
Estimates of the file transfer size MUST NOT be returned; only
precise information is acceptable.
*/

void CmdTest::ftpCmdSIZE() {
TCchar* path = _T("/test00.txt");
int     size;

  if (isLocked()) return;

  setupCmd();

#if 1
    if (sftpSSL.size(path, size)) notePad << path << _T(" is ") << size << _T(" Bytes") << nCrlf;
#else
    String s;
    if (sftpSSL.sendCmd(_T("SIZE"), _T("/test00.txt"), s)) dspLines(s);
#endif

  finishCmd();
  }


// Mount file structure
/*
STRUCTURE MOUNT (SMNT)

SMNT <SP> <pathname> <CRLF>

This command allows the user to mount a different file
system data structure without altering his login or
accounting information. Transfer parameters are similarly
unchanged. The argument is a pathname specifying a
directory or other system dependent file group designator.
*/

void CmdTest::ftpCmdSMNT() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("SMNT"), _T("/testDir/"), s)) dspLines(s);

  finishCmd();
  }


// Use single port passive mode (only one TCP port number for both control connections and passive-mode
// data connections)

void CmdTest::ftpCmdSPSV() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("SPSV"), 0, s)) dspLines(s);   finishCmd();
  }


// Returns information on the server status, including the status of the current connection

void CmdTest::ftpCmdSTAT() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("STAT"), 0, s)) dspLines(s);   finishCmd();
  }


// Return system type.
/*
SYSTEM (SYST)

This command is used to find out the type of operating
system at the server. The reply shall have as its first
word one of the system names listed in the current version
of the Assigned Numbers document [4].
*/

void CmdTest::ftpCmdSYST() {
String s;

  if (isLocked()) return;

  setupCmd();   if (sftpSSL.sendCmd(_T("SYST"), 0, s)) dspLines(s);   finishCmd();
  }


// Get a thumbnail of a remote image file

void CmdTest::ftpCmdTHMB() {

  if (isLocked()) return;

  ftpCmdNotImpl();
  }


// Sets the transfer mode (ASCII/Binary).
/*
REPRESENTATION TYPE (TYPE)

The argument specifies the representation type as described
in the Section on Data Representation and Storage. Several
types take a second parameter. The first parameter is
denoted by a single Telnet character, as is the second
Format parameter for ASCII and EBCDIC; the second parameter
for local byte is a decimal integer to indicate Bytesize.
The parameters are separated by a <SP> (Space, ASCII code
32).

The following codes are assigned for type:

            \    /
  A - ASCII |    | N - Non-print
            |-><-| T - Telnet format effectors
  E - EBCDIC|    | C - Carriage Control (ASA)
            /    \
  I - Image

  L <byte size> - Local byte Byte size

The default representation type is ASCII Non-print. If the
Format parameter is changed, and later just the first
argument is changed, Format then returns to the Non-print
default.
*/

void CmdTest::ftpCmdTYPE() {
String s;

  if (isLocked()) return;

  setupCmd();

    if (sftpSSL.sendCmd(_T("TYPE"), _T("E"),   s)) dspLines(s);   s.clear();
    if (sftpSSL.sendCmd(_T("TYPE"), _T("E T"), s)) dspLines(s);   s.clear();
    if (sftpSSL.sendCmd(_T("TYPE"), _T("E C"), s)) dspLines(s);   s.clear();
    if (sftpSSL.sendCmd(_T("TYPE"), _T("I"),   s)) dspLines(s);   s.clear();
    if (sftpSSL.sendCmd(_T("TYPE"), _T("A T"), s)) dspLines(s);   s.clear();
    if (sftpSSL.sendCmd(_T("TYPE"), _T("A C"), s)) dspLines(s);   s.clear();
    if (sftpSSL.sendCmd(_T("TYPE"), _T("A"),   s)) dspLines(s);   s.clear();

  finishCmd();
  }


// Authentication username
/*
USER NAME (USER)

The argument field is a Telnet string identifying the user.
The user identification is that which is required by the
server for access to its file system. This command will
normally be the first command transmitted by the user after
the control connections are made (some servers may require
this). Additional identification information in the form of
a password and/or an account command may also be required by
some servers. Servers may allow a new USER command to be
entered at any point in order to change the access control
and/or accounting information. This has the effect of
flushing any user, password, and account information already
supplied and beginning the login sequence again. All
transfer parameters are unchanged and any file transfer in
progress is completed under the old access control
parameters.


PASSWORD (PASS)

The argument field is a Telnet string specifying the user's
password. This command must be immediately preceded by the
user name command, and, for some sites, completes the user's
identification for access control. Since password
information is quite sensitive, it is desirable in general
to "mask" it or suppress typeout. It appears that the
server has no foolproof way to achieve this. It is
therefore the responsibility of the user-FTP process to hide
the sensitive password information.

username Doe <CR> USER Doe<CRLF>????>
                  <---- 331 User name ok, need password<CRLF>.
password mumble   <CR> PASS mumble<CRLF>????>
                  <---- 230 User logged in<CRLF>.
*/

void CmdTest::ftpCmdUSER() {
Baffle user = siteID.loadBaffle(_T("swde.com Site"), UserIDKey);
Baffle pass = siteID.loadBaffle(_T("swde.com Site"), PasswordKey);
String t;
String s;

  if (isLocked()) return;

  if (!open(_T("swde.com"))) return;

    user >> t;   if (sendCmd(_T("USER"), t, s)) dspLines(s);   s.clear();

    pass >> t;   if (sendCmd(_T("PASS"), t, s)) dspLines(s);   t.clear();

  finishCmd();
  }


void CmdTest::ftpCmdPASS() {

  if (isLocked()) return;

  notePad << _T("See USER command test.") << nCrlf;    finishCmd();
  }



// Not test here

void CmdTest::ftpCmdNotImpl() {

  if (isLocked()) return;

  notePad << _T("Not Tested Here") << nCrlf;   finishCmd();
  }


bool CmdTest::isLocked(bool prevent) {return doc()->isLocked(prevent);}


bool CmdTest::setupCmd() {notePad.clear();   return login();}


void CmdTest::dspLines(String& s) {
String ss = s;
int    pos;
String t;

  while (!ss.isEmpty()) {
    pos = ss.find(_T('\n'));   t = ss.substr(0, pos);   notePad << t << nCrlf;   ss = ss.substr(pos+1);
    }
  }



void CmdTest::dspDataLines() {
SftpDataIter ix(sftpSSL.fileData());
String*      s;

  notePad << nFFace(_T("Courier New"));

  for (s = ix(); s; s = ix++) notePad << *s << nCrlf;

  notePad << nFont;
  }



void CmdTest::finishCmd() {doc()->display();}


