// Devices, a collection of Device


#pragma once
#include "Device.h"
#include "ExpandableP.h"
#include "IterT.h"


class DeviceNode : Device {

DeviceID deviceID;

public:

  DeviceNode() : deviceID(NilDevID) { }

  void setID(DeviceID id) {deviceID = id;}

  friend class Devices;
  };


class Devices;
typedef IterT<Devices, DeviceNode>      DvsIter;
typedef DatumPtrT<DeviceNode, DeviceID> DeviceP;


class Devices {
ExpandableP<DeviceNode, DeviceID, DeviceP, 4> data;

public:
              Devices() { }
             ~Devices() { }

  void        initialize();
  void        clear();

  Device*     get(DeviceID id);

private:

  DeviceNode* find(DeviceID id);

  // returns either a pointer to data (or datum) at index i in array or zero

  DeviceNode* datum(int i) {return 0 <= i && i < nData() ? data[i] : (DeviceNode*) 0;}

  int         nData()      {return data.end();}           // returns number of data items in array
                                                          // not necessarily private
  void        removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename DvsIter;
  };


extern Devices devices;
