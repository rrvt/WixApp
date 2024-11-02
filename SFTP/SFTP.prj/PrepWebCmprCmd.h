// Prepare Upload for update command


#pragma once

class WebNode;


class PrepWebCmprCmd {
int noFiles;

public:

          PrepWebCmprCmd() { }
         ~PrepWebCmprCmd() { }

  void    start();

  LRESULT finishMsg(WPARAM wparam, LPARAM lParam);

private:

  bool    isLocked(bool prevent = false);

  void    setCurFileDscs(WebNode& node);
  void    setCurFileDir(TCchar* webDirPath);

  void    setChecks();

  void    display();

  friend UINT prepUploadThrd(void* param);
  };


extern PrepWebCmprCmd prepWebCmprCmd;

