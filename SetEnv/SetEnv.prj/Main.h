// Main -- Get started with arguments and do everything by calling others


#pragma once


enum Operation {NilOp, SetOP, AppendOp, DeleteOp, PrePendOp, ViewOp, CopyOp, SanitizeOp};


class Main {
int       nArgs;
int       argX;
TCchar**  args;
TCchar**  env;

Operation operation;
bool      isSystemEnv;       // Default to system variable (/u = user)

String    name;
String    value;

public:

  Main(int argc, TCchar* argv[], TCchar* envp[]);
 ~Main() {}

  bool processArgs();
  bool setEnv();

private:

  void init();
  void Usage();
  void ShowExamples();
  void ShowCopyright();

  Main() {}
  };

