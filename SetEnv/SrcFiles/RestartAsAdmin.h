// Restart as Administrator
// A program cannot just raise their capability to admin level, so just restart with the same arguments


#pragma once


class RestartAsAdmin {
int     nArgs;
TCchar** args;
public:

  RestartAsAdmin();

  void init(int argc, TCchar* argv[]);

  bool elevate();
  bool isAdmin();

private:

  String getArgString();
  };


extern RestartAsAdmin restart;
