// A Little Stack to save everything related to output to a device


#pragma once
#include "Device.h"


class DeviceData {
bool   loaded;
Device data;

public:

  DeviceData() : loaded(false) {data.clear();}
 ~DeviceData() { }

  void save(   Device& dev) {data.clear();   data = dev;   loaded = true;}
  void restore(Device& dev) {if (loaded) {dev.clear();    dev  = data;   loaded = false;}}
  };


