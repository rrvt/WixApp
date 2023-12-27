// Qsort Template for an array of something
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

// Requires the following operations for node N* p, N* q, N* s
//  *p >  *q
//  *p <= *q
//  *s =  *p
// Call as follows:
// qsort(&array[firstIndex], &array[lastIndex]);   or equivalent

#pragma once


template <class N>

void qsort(N* p, N* q) {
enum {split = 15};

N* r;
N* s;
N* pivot;
N  temp;

  if (p >= q) return;                     // Nothing to sort!

  do {
    if (q - p <= split) {
      for (r = p+1; r <= q; r++) {        // Insertion Sort is faster
        if (*(r-1) > *r) {                // for small vectors
          temp = *r;
          for (s = r-1; s >= p; s--) {
            *(s+1) = *s;
            if (s == p || *(s-1) <= temp) break;
            }
          *s = temp;
          }
        }
      return;
      }

    r = p - 1; s = q; pivot = s;

    while (r < s) {
      while (*pivot > *(++r)) ;

      while (--s > r && *s > *pivot) ;

      if (r < s) {temp = *r; *r = *s; *s = temp;}
      }

    temp = *r; *r = *pivot; *pivot = temp;

    if (r - p < q - r) {qsort(p, r-1); p = r+1;}
    else         {qsort(r+1, q); q = r-1;}
    } while (p < q);
  }

