// DisplayDevice


#include "pch.h"
#include "DevDsply.h"


void DevDsply::prepare(CDC* dc)
                     {printing = false;   dvx.set(dc);   initTxt();   dvx.wdwDim();   /*initPageSize();*/}

