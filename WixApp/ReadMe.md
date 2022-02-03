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

## Updates

### Update 01/31/22

Updates to the library, mostly about the toolbar, but also NewAllocator, and a minor addition to
Expandable.

Also scrubbed the project files to remove all references to precompiled header files which I turned off
in VS17.  Unfortnately VS does not remove all references and pch files kept cropping up.  Not only does
that slow down the compiler but the files are large...

### Update 12/18/21

While creating a template for a dialog box only application with a toolbar the Library was added to and
changed to hopefully simplify toolbar creation.  This version of the toolbar includes buttons, edit boxes,
combo boxes and drop down menus as well as the traditional 16x15 icons.

WixApp is a dialog box but does not have a toolbar yet.

### Update 11/13/21

But the major changes are in the library:

  - Expandable and ExpandableP -- moved the functions out of the class to make the compilation smaller,
also fixed one bug (or feature) in the ExpandableP insert sorted function that would throw out entries
that were already in in the array.
  - FileIO -- Changed the type of path from "String&" to "TCchar*".  It turns out that automatic
conversion from String to TCchar* is easy, but the reverse is is not easy.  Also added a function to
get the length of a file given a path.
  - FineName -- Added a function to get the directory name of from a path
  - NewAllocator -- Corrected the debug dependency from if to ifdef
  - ToolBar -- Added data for drop down menus

### Update 10/31/21

Changed Resources (i.e. xxx.rc data) access so that it will work with a dll.  Changed the logic in the
About Box code.  Some other library changes.  Some corresponding changes in the application.  Similar
changes were made in SetEnv.

An iterator was introduced in the IniFile module and it is being used in the WixData module to clear
the sections.

### Update 4/20/21

Library changes only.

### Update 2/20/21

Noticed, found and fixed a small memory leak.

### Update 2/19/21

Well I tend to move code around and this program would choke every time that I wanted to make a new
installer.  So When the file is read each path is checked.  If the path is not available then the
user is asked to provide the correct path to the specific file.  This should help.

### Update 2/15/21

The backup file date was dependent on the wrong year parameter which unfortunately worked during 2020
because it was using the century, not the year (Ugh!!!).  Also the library has been update for other
applications and this version is to be sure everything still works.

### Update 9/10/20

Library improvement project.  Changed the Expandable Array classes, added templates for a pointer only
class in the ExpandableP class definition.  It turned out I was defining this RcdPtr class many times
worrying about exactly how it should be defined.  So I turned it into a template, which of course then
I needed to go back and add it to all the programs.  Severl iterations later I added some documentation
and I am uploading the programs today.

### August 16, 2020

Exchanged the Help and About Menus (make them more like other Applications, at least mine).

### August 15, 2020

Moved the Istaller directory current path to the WixApp.ini file.  Do a better job of updating the path
in the ini file.

Added logic to update the combo boxes when a new entry is created.  They were being stored in the interior
data stores but feature combo box was not updated.  Also do a better job with the Nil identifier both
adding it to the stores and with presenting a blank combo box edit region when dealing with a new entry.

### July 24, 2020

Add a Save Wix Data button and made an attempt to save everything input.  There is some logic in the
program to not save some things that are unused.  However, that prevented the user from incrementally
adding things to the list of file.  The insisting that everything be written when requested seems like
a better algorithm.

Changed the way the input and output are handled.  Upon selecting the Save Wix Data button a save file
dialog box is displayed and the user must specify where the data is to go.  It is initialized with the
last place the data was read from so that it is not a big chore to confirm the location.  This path
is then used for initial path to the product.wxs file.  This change allows the product name to be
different from the installer name.

Made some changes in the way loops are formulated using
  OBIter iter(obj);                      // Where OBIter is the name of a class that implements the iter
  ...
  for (p = iter(); p; p = iter++) { ... }

instead of

  for (p = obj.startLoop(); p; p = obj.nextItem()) { ... }

For non-template classes the OBIter class is formed from a simple template typedef.  There is a slightly
more complex version of the declaration of the Iterator class for template classes but the loop is
created in the same way.  The iterator always returns a pointer or zero.

### May 2020

It became clear when building the MakeApp program that specifying the Program Files Directory and Start
Menu Name for each Component was a drag.  They were moved to the Feature section.  Also there was a
tendency of the File Specification Dialog Boxes to start in leftover (and wrong places) so a bit of work
was done to narrow the default directory of everything to a new project and keep track of those default
directories.  Some bugs were also found and fixed, but alas some still remain.  It is still robust enough
to allow recovering from most losses without too much pain.

## Getting Started

This version was created with Visual Studion 2017.  It was compiled with the following properties:
  o Windows SDK Version: 10.0.18362.0
  o Platfor Toolset: visual Studio 2017 (v141)
  o MFC: Use MFC in a Shared DLL
  o Character Set:  Use Unicode Character Set
  o Additional Include Directories:
    * $(ProjectDir)
    * $(SolutionDir)..\WixApp.prj\;
    * $(SolutionDir)..\..\Library\AboutFiles\
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

Robert R. Van Tuyl, romeo romeo victor tango alpha tango sam whisky delta echo dot charlie oscar mike

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
