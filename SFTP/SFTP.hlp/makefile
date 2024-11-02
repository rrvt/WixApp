

SFTP.chm : *.htm SFTP.hhp SFTP.hhc App.css
  -1 "C:\Program Files (x86)\HTML Help Workshop\hhc" SFTP


Clean.cln    :
  @echo "Cleaning"
  cd $(MAKEDIR)
  del /F SFTP.chm


Release/SFTP.chm : SFTP.chm
  copy $(MAKEDIR)\SFTP.chm  $(MAKEDIR)\..\Release\SFTP.chm


Release/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Release
  del SFTP.chm
  cd $(MAKEDIR)


Debug/SFTP.chm : SFTP.chm
  copy $(MAKEDIR)\SFTP.chm  $(MAKEDIR)\..\Debug\SFTP.chm


Debug/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Debug
  del SFTP.chm
  cd $(MAKEDIR)


Release/Rebuild.rbd : Release/Clean.cln Release/SFTP.chm


Debug/Rebuild.rbd   : Debug/Clean.cln Debug/SFTP.chm


