

WixApp.chm : *.htm WixApp.hhp WixApp.hhc App.css
  -1 "C:\Program Files (x86)\HTML Help Workshop\hhc" WixApp


Clean.cln    :
  @echo "Cleaning"
  cd $(MAKEDIR)
  del /F WixApp.chm


Release/WixApp.chm : WixApp.chm
  copy $(MAKEDIR)\WixApp.chm  $(MAKEDIR)\..\Release\WixApp.chm


Release/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Release
  del WixApp.chm
  cd $(MAKEDIR)


Debug/WixApp.chm : WixApp.chm
  copy $(MAKEDIR)\WixApp.chm  $(MAKEDIR)\..\Debug\WixApp.chm


Debug/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Debug
  del WixApp.chm
  cd $(MAKEDIR)


Release/Rebuild.rbd : Release/Clean.cln Release/WixApp.chm


Debug/Rebuild.rbd   : Debug/Clean.cln Debug/WixApp.chm


