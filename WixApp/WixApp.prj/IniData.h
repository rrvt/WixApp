// Get data from ini file determined by debug state


#pragma once

class IniData {

public:

  IniData() { }
 ~IniData() { }

  bool read(TCchar* key, String& val, TCchar* dflt);
  void write(TCchar* key, TCchar* val);
  void delStg(TCchar* key);
  };


extern IniData iniData;

