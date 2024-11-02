// Update Local or Remote Site


#pragma once

class SiteFileDscs;


class UpdateCmd {

int noFiles;

public:

          UpdateCmd() : noFiles(0) { }
         ~UpdateCmd() { }

  void    start();

  LRESULT finishMsg(WPARAM wparam, LPARAM lParam);

private:

  bool    isLocked(bool prevent = false);

  bool    doXfer(SiteFileDscs& dscs);

  void    display();

  friend UINT updateThrd(void* param);
  };


extern UpdateCmd updateCmd;
