// Prolog Section -- includes defines


#include "stdafx.h"
#include "filename.h"
#include "Prolog.h"
#include "WixUtilities.h"


Prolog prolog;



void Prolog::output() {

  wix.lit(0, _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));
  wix.lit(0, _T("<Wix xmlns=\"http://schemas.microsoft.com/wix/2006/wi\">\n"));
  }

