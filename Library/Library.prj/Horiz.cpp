// Horizonal line management for display/printer output


#include "pch.h"
#include "Horiz.h"


void Horiz::clear() {position = maxPos = leftBnd = rightBnd = 0;}


void Horiz::cr() {if (position > maxPos) maxPos = position;  position = leftBnd;}


