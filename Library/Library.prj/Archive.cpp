// Replaces CArchive to allow for Unicode internally and ansi in external files
// Copyright Bob -- K6RWY, 2019


#include "pch.h"
#include "Archive.h"



ArchManip aCrlf;                  // add to stream to terminate a line on display: dsp << "xyz" << dCrlf;


void Archive::initialize() {aCrlf.n = this; aCrlf.func = Archive::doCrlf;}

