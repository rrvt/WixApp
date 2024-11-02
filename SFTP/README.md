# SFTP -- An application for updating encrypted login web sites with ftp protocols

First let me point out that this application works on "turbify" Web Host.  I do not
have the resources to test it at many, several or even two different web hosts.

Web Hosts are getting a bit smarter about login protocols for updating web sites.  An old version of
Dreamweaver stopped updating my web site because they are now requiring login to the site to be encrypted.
However, just because the login is encrypted the rest of the transactions to update a web site are
just plain old ftp.

This app uses OpenSSL to perform the encrypted part of the update process and then the ftp is allowed
to go forward.

When this first happened I turned to OTS products to update my web site.  FileZilla Pro seemed to work
and I am able to update the web site with it.  However it requires the user to figure out which files
need updating.  Filezilla helps by highlighting various files it deems might require updating but there
a lot of false positives.  SFTP attempts to make this job easier.

There are two comparisons that may be performed in this application:
  - Compare the present and absence of files in the web host and the local PC.
  - Compare the size and date of each file at the last transfer between the local PC and web host and
the current size and date of each file.

The present/absence comparison is useful when starting out.  It produces a list of files to upload
(transfer files from the local PC to the web host) and/or download (transfer files from the web host to
the local PC).  It just attempts to make the two copies the same.  It is useful to get an initial
cownload of a website either to compare with an old copy or to start making changes to an existing web
site for which no local PC copy exists.  During this transfer a record is made of the size and date of
each local PC file for use later.

The size/date comparison is useful when updating local PC files wherein the local PC files and web host
files started out identical (even though the date and sizes may not be identical the content is).  Thus
make a spelling change in one or more of the local PC files and use SFTP size/date comparison to upload
only those files that have changed since the last update.

The size/date comparison results in two operations:
  - Upload a file that has changed on the local PC
  - Delete a file on the host that has been removed from the list of files on the local PC

Now having described the comparisons please note that the comparisons only mark a list of the web site
files for being updated.  The Update command is used implement the operations.  However there is a little
gotcha that the update command tries to avoid by using a check box for each operation so that the
user must check a box for each operation to be performed.  Of course there is a do-all button too.

What is the gotcha?  The present/absence comparison only marks uploads and downloads.  The size/date
comparison only marks uploads and deletions.  The same missing file in the current list will produce
different operations (download and deletion) for the same file.  So the operations marked by one
comparison are not overruled by the second comparison if one chooses to use both comparions.  That seemed
to be the more sensible rule but I could be wrong about that.

This gotcha is due to the point of view. The present/absence comparison is trying to make the local PC
files match the web host files and vica versa. The size/date comparison of the local files is trying to
make the web host match what the author has prepared in the local PC file list. Both comparisons have
their uses.

However, there is a little issue at the beginning.  If the web site already exists and SFTP is
being used for the first time to update the web site then there is no past history database for
the web site.  So if the web host files are downloaded into an empty directory on the local PC they can
be compared on the local PC with a difference program to determine which of the files are different.
Then correct the files that are out of date and upload only those files to get a record of the current
size and date.  After the initial upload SFTP will only indicate changed files for uploading.

Of course if there is no local PC copy of the database then the download itself will create the size and
date records.

In any event this application works more like Dreamweaver in that it alerts the user to files that
need updating on the web site and facilitating the update.

## Getting Started

This version was created with Visual Studion 2022.  It was compiled with the following properties:

  o Windows SDK Version: Latest Win10 SDK
  o Platform Toolset: visual Studio 2022 (v141)
  o MFC: Use MFC in a Shared DLL
  o Character Set:  Use Unicode Character Set
  o Precompiled Header:  Not Using Precompiled Headers
  o Linker/Additional Dependencies:  Htmlhelp.lib, OpenSSL.3.1\libssl.lib, OpenSSL.3.1\libcrypto.lib
  o OpenSSL.3.1.2 compiled separately for both debug and release.  Details below...

The HTML Help Workshop (HHW), Version 4.74.8702.0 was used to prepare the help file (WixApp.chm).  It is
copied into the Release directory.  I used Dreamweaver (DW) to do most of the content production of the
help files that the HTML Help Workshop produces (i.e. HHW is used to produce the pages/files needed
and DW is used to fill in the content).

Additionally the WIX Installer, The very old Help Workshop and SlickEdit were used to process the code.
The Product (Wix installation file) was produced by my WixApp but it can be manipulated by
hand for changes.

### SftpSSL

This version of a FtpLib was based on ideas first published by Thomas Pfau and modified by
magnus kulke.  I've reorganized those ideas into multiple classes with different concerns.

The interface to this library is in SftpSSL.h.  No other header file is included in SFTP modules.  This
was done to shield SFTP from OpenSSL header files (which could provide confusing interactions with
names in SFTP).

### OpenSSL

I've used OpenSSL.3.1.2 in the build.  it is compiled only for the libraries that are needed by the
SFtp Library.  It compiles into two libraries that are referenced in SFTP.

Compiling OpenSSL is a big, complex process best left to the originators of OpenSSL.  So there is a
command/script file (i.e. BuildForWin.cmd) that will build the OpenSSL libraries, test files,
OpenSSL, and other miscellaneous features.  The command file, BuildForWin.cmd, also
initializes the nMake facility for Visual Studio.  BuildForWin.cmd accepts three arguments:

  * The Configuration (Debug or Release)
  * The path to the files (e.g. D:\Source\OpenSSL.3.1\)
  * The word "static" to indicate static libraries.

Since it takes a long time to run this command file, it is not compiled when the other projects are
compiled.  Here is the command/script file:

```
rem first argument is the configuration (i.e. Debug or Release)
rem second argument is path to openSSL directory base (e.g. D:\Sources\OpenSSL.3.1\)
rem third argument is "static" to indicate a static library (rather than a dll library)

rem   Build Solution Line:  tcc  WinBuild %bn "%rw" static
rem   Be sure that %rw contains no spaces (too much appending is done that does not deal with quote marks
rem   Add Build Solution line to

echo on

set Configuration=%1
set WorkSpace=%~2
IF %WorkSpace:~-1%==\ SET WorkSpace=%WorkSpace:~0,-1%
set WorkSpace=%WorkSpace%..\

set OpenSSLsrc=%WorkSpace%

set OpenSSLtgt=%WorkSpace%%Configuration%\

set Dirs=--prefix=%OpenSSLtgt% --openssldir=%OpenSSLtgt% --libdir=..\%Configuration%

iff "%Configuration%" == "Debug" then
  set flavor=--debug
else
  set flavor=--release
endiff

iff "%3" == "static" then
  set Static=no-shared
else
  set Static=
endiff

cd %OpenSSLsrc%

rem Setup Compiler system variables -- required for nmake to run

set vs2022="C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars32.bat"
CALL %vs2022%

rem Create makefile

md /d/Ne %OpenSSLsrc%

perl Configure VC-WIN32 %flavor%  %Dirs% %Static% no-capieng


nmake
nmake test
nmake install
exit

```

The command/script is run in jpsoftware's Take Command (i.e. tcc). Their websit is https://jpsoft.com/.

```
tcc  BuildForWin Release D:\Sources\OpenSSL.3.1\ static
```


## Prerequisites

The WiX Toolset must be installed in Visual Studio.  The "HTML Help Workshop" (google it) must be
installed.  Visual Studio 2022 or later.  OpenSSL 3.1,2 or later.

### Installing

The /Release/FtpAppInstaller.msi will install the product.

## Updates

### Update 8/7/24

Library update.

### Update 6/14/24

Reorganize toolbar and Fix WinPos.

### Update 2/19/24

Reorganized the Library so that it compiles once into Debug and Release directories under the Library
directory.

### Update 1/8/24

Library and other changes.

### 12/26/23 Update

Library Update.

### 11/4/23 Update

First version that will create and delete directories on the web host when the local version of the site
contains new directories or directories are removed from the local site.  It will also create directories
on the local site that appear in the web host.  The app will not delete directories on the local site (as
that is the prerogative of the user).

### 10/22/23 Update

This update is the result of banging on the app and fixing things I found.

### 10/20/23

Shut down the logging during comparisons.

### 10/20/23 Update

The earlier version did not respond very well to changes in the PC when the application was running.
So I've made some changes to the logic to accomodate changes while the application is running.  It is
still a work in progress.

### 10/8/23 Update

Trying different things find holes in the logic and plugging them.

## Authors

Robert R. Van Tuyl

Contact:  romeo romeo victor tango alpha tango sam whisky delta echo dot charlie oscar mike

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details


