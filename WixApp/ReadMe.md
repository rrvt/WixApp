# WixApp -- Produces a Product.wxs file from a list of files.

WixApp is a single dialog box that allows information about executable, dll, help and other files to
be combined into one install "msi" file using the WiX Toolset v3.  It does this by acceptng names
of files and producing a product.wxs file (preserving older versions).  The product.wxs file is compiled
by Visual Studio using the Wix Toolset compilation.

## Why is this project useful?

Wix, while powerful is difficult code to write correctly the first time.  After many attempts at
getting it right, I just sat down one day and started this program.  It has been useful to me as
I don't need to think too carefully about getting the WiX input right, I just need to make sure I
know all the files needed in the output.

## Update May 2020

It became clear when building the MakeApp program that specifying the Program Files Directory and Start
Menu Name for each Component was a drag.  They were moved to the Feature section.  Also there was a
tendency of the File Specification Dialog Boxes to start in leftover (and wrong places) so a bit of work
was done to narrow the default directory of everything to a new project and keep track of those default
directories.  Some bugs were also found and fixed, but alas some still remain.  It is still robust enough
to allow recovering from most losses without too much pain.

## Getting Started

This version was created with Visual Studion 2017.  It was compiled with the following properties:
   o  Windows SDK Version: 10.0.18362.0
   o  Platfor Toolset: visual Studio 2017 (v141)
   o  MFC: Use MFC in a Shared DLL
   o  Character Set:  Use Unicode Character Set
   o  Additional Include Directories:  $(SolutionDir)..\SrcFiles\;$(SolutionDir)..\..\Library\LibFiles\;
                                       $(SolutionDir)..\..\Library\AboutFiles\
   o  Precompiled Header:  Not Using Precompiled Headers
   o  Linker/Additional Dependencies:  Htmlhelp.lib

The HTML Help Workshop (HHW), Version 4.74.8702.0 was used to prepare the help file (WixApp.chm).  It is
copied into the Release directory.  I used Dreamweaver (DW) to do most of the content production of the
help files that the HTML Help Workshop produces (i.e. HHW is used to produce the pages/files needed
and DW is used to fill in the content).

Once the executable is created it can be used to create an installer.  Take a look at the help file for
details.

### Prerequisites

The WiX Toolset must be installed in Visual Studio.  The "HTML Help Workshop" (google it) must be
installed.  Visual Studio 2017 or later.

## Installing

Use Visual Studio 2017 to compile a debug or release version.  The WixApp.exe executable is then ready
to be used.  Create a small test program (e.g. Hello World).  Add a new project to the Hello World
projec:

 - In Visual Studio highlignt the Solution line in the Solution Explorer
 - Select Menu File/Add/New Project or right click Solution line and select Add/New Project
 - Select WiX Toolset/v3/Setup Project for WiX v3
 - Give it a name (e.g. Installer) and I usually place each project in a directory with the same parent
   as the the parent of the solution directory (See WixApp directories for an example)

Now that the Installer directory has been created one should see a Product.wxs file (among others) in that
directory.  This is the file that will be used to produce the installer output file something.msi.  The
problem is that this file is complex to write.  At this point I list all of the files needed to produce
an application in the Program Files (x86) directory.  WixApp is then used to create a new Product.wxs
file.

Start WixApp (debug or release version, doesn't matter).  A dialog box appears.  Here are the steps to
create a new Product.wxs file:
 - If this is the first time WixApp is used for a project then use the Menu item File/New.
 - The dialog box has a little explanation and a Next Button.  Press the Next Button
 - A Solution File dialog box will appear and now the user must find the Solution file (e.g. xxx.sln)
   in the directories of the project.  I usually put in a directory with the name of the project, for
   example a project named "Foo" would be in the directory D:\Sources\Foo.  That is all files related
   directly to Foo would be in D:\Sources\Foo.  The solution file would be found at:
   D:\Sources\Foo\Foo\Foo.sln.  The project files would be found in another directory.
 - Fill in the Product Name.  The Product Name will be copied to several other edit boxes as a
   convenience.
 - Add a Company Name and Control Panel Icon (e.g. Foo.ico).  Edit the Installer Name if desired.

Now a bit of explanation is needed before the next steps.  A Feature is a collection of Components.  All
of the Components of a Feature share some attributes, namely the directory in the Program Files (x86)
directory and the Menu (directory) in the Windows Start Menu.

The Components also share another feature which is hidden from the user, namely the default source
directory.  The Components describe files in the project.  Some files come from the Project's Release
directory.  Some files may come from other directories in the project.  It is convenient when searching
for a file that the File Selection dialog box start in a convenient place, namely the default directory
for the Feature.  The default directory starts at the Solution File directory and changes with each
selection of a file in a feature.  So one organization of a Feature would be all files in one source
directory.

Any number of features can be used in the installer, i.e. WixApp.  Any number of Components may be
included in a Feature.  Be careful of the spelling of Program Files Directory Name and Start Menu Name.

The Component Name for an executable that is listed in the Start Menu or plopped onto the desktop is the
name that appears in the Start Menu and/or on the desktop.  Choose the Component Name with care.

The next steps are to describe the files in the project that are to be installed in the Windows system.

 - The initial Feature Name is the Product Name, change if you wish a different name
 - Add the Program Files Dir Name, just the Directory Name, in our example that would be something like
   "Foo".  If a more complex directory structure is needed the "Foo\Mumble\XYZ" is also allowed.  Whatever
   you place in that edit window is where the files will be placed.
 - Add the Start Menu Name, e.g. "Foo" or "MyCompany\Foo"
 - When it comes time to add a different collection of files to the installation, press the New Button
   next to the Feature Name edit box.
 - If a feature is garbage, press the Delete Dutton and the feature and all of its components will
   disappear.
 - Use the drop down button to bring up another Feature for editing or examination.

Components may now be added to the Feature selected.  The New Button should be pressed after the first
one is described.  A Component may be deleted with the Delete Button.

Components have a few distinct attributes that are used in the installer.  The first is the path to the
file.  The Description is added to the application in windows and may be found in the properties box.
The check boxes specify

 - In Start Menu -- Add to start menu as specified in the Feature
 - Desk Top -- Place application of the Desktop
 - Version No Available -- The version number is part of the Windows system for discouraging loading older
   applications over newer ones.  The version comes out of the resource (e.g. Foo.rc) file and is in
   one of two places near "VS_VERSION_INFO VERSIONINFO".
 - On Path -- Used for Command Prompt (DosBox) applications so that command line application can be found.
   Requres the program SetEnv be available.
 - Win7+ -- Adds a condition that the OS be Win7 or later
 - WinXP -- Adds a condition that the OS be WinXP
 - Win2K -- Adds a condition that the OS be Win200

Finally, in order for the desktop and Start Menu to illustrate the application with an icon, the Icon must
be specified.  This is done by using the drop down button (triangle on right side of edit box) or by
browsing to the file.

When all the files have been specified in WixApp use the Create Button.  It will produce a new Product.wxs
and save the old one with a date/time appendage.  It will also save the Foo.wxd (WixApp Data File).  The
next step is to use Visual Studio (or Slickedit) to compile the whole project.  Note, if there are files
missing the msi file will not be created.  Use the VS17 menu items to control the order of the build and
the Configuration Manager to specify compiling the msi file only for the release build.

When things do not go well because something is mispelled or missing in the Product.wxs, edit the wxd
file by using the File/Open menu item in WixApp.  Most things are easily editted.

## Authors

Robert R. Van Tuyl, rrvt@swde.com

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details