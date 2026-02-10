

WixApp.chm : *.htm WixApp.hhp WixApp.hhc App.css
  -1 "C:\Program Files (x86)\HTML Help Workshop\hhc" WixApp


Clean.cln    :
  @echo "Cleaning"
  cd $(MAKEDIR)
  if exist WixApp.chm del /F WixApp.chm


$(TgtFolder)/WixApp.chm : WixApp.chm
  copy $(MAKEDIR)\WixApp.chm  $(MAKEDIR)\..\$(TgtFolder)\WixApp.chm


$(TgtFolder)/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\$(TgtFolder)
  if exist WixApp.chm del /F WixApp.chm
  cd $(MAKEDIR)


$(TgtFolder)/Rebuild.rbd : $(TgtFolder)/Clean.cln $(TgtFolder)/WixApp.chm


