// Insert node into a sorted array
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once



template <typename N>

void insertSorted(N& node, N ar[], int& noN) {
N   xNode;
N   nextNode;
int i;

  for (i = 0; i < noN; i++) if (ar[i] >= node) {if (ar[i] == node) return; break;}

  for (xNode = node, noN++; i < noN; i++) {nextNode = ar[i]; ar[i] = xNode; xNode = nextNode;}
  }
