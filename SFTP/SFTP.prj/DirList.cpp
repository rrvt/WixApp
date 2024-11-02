// Directory List subject to a commom action


#include "pch.h"
#include "DirList.h"
#include "SiteFileDscs.h"


void DirList::add(SiteFileDsc* dsc)
                {DirItem item;   item.level = pathLevel(dsc->key.path);   item.dsc = dsc;   data = item;}

