

#include "pch.h"
#include "Devices.h"


Devices devices;


void Devices::initialize() {
//DvsIter iter(*this);
//Device* dev;


  }


void Devices::clear() {
//DvsIter iter(*this);
//Device* dev;


  }



Device* Devices::get(DeviceID id) {
DeviceNode* node = find(id);

  if (!node) {node = data.allocate();   node->setID(id);   data += node;}

  return (Device*) node;
  }


DeviceNode* Devices::find(DeviceID id) {
DvsIter     iter(*this);
DeviceNode* node;

    for (node = iter(); node; node = iter++) if (id == node->deviceID) return node;

  return 0;
  }

