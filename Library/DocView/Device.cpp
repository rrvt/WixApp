// Device Display/Printer Output Base Class


#include "pch.h"
#include "Device.h"
#include "Devices.h"


Device::Device() : noPages(0) { }


Device& Device::operator= (Device& dev)
                       {TextAccm::copy((TextAccm&) dev);   noPages  = dev.noPages;   return *this;}


void Device::setHorzMgns(double left, double right) {initHoriz(left, right);}


void Device::setVertMgns(double top,  double bot) {vert.initBounds(top, bot);}


void Device::restore(DeviceID id) {
Device& dev = *devices.get(id);

  DeviceContext::restore(dev);
  }


void Device::setEndPage() {if (vert.withinBounds()) {vert.setEndPage(); horz.cr();}}



bool Device::doEndPageChk() {

  if (!vert.isPrinting() || !vert.isEndPage()) return false;

  vert.atEndPageCond();  return true;
  }


////--------------

//Device& Device::operator= (Device& dev)
//{TextAccm::copy((TextAccm&) dev);   noPages  = dev.noPages;   return *this;}
                                             //  leftMgn = left;   rightMgn = right;


