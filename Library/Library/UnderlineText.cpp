

#include "pch.h"
#include "UnderlineText.h"
#include "CDCex.h"

#include "FileIO.h"



void UnderlineText::start(int x, int y) {underlining = true;   curPoint(x, y);}


bool UnderlineText::draw(int x, int y) {
Point pts[2];

  if (!underlining || curPoint.x == x || curPoint.y != y) return false;

  pts[0] = curPoint;   pts[1](x,y);   drawLine(pts);   curPoint = pts[1];   return true;
  }


/*  // 1. Create a logical pen (e.g., PS_SOLID with a width of 5 pixels)
    CPen pen(PS_SOLID, 5, RGB(255, 0, 0)); // Red, 5 pixels thick

    // 2. Select the pen into the device context and store the old pen
    CPen* pOldPen = pDC->SelectObject(&pen);

    // 3. Define the points for your polyline
    CPoint points[] = { CPoint(50, 50), CPoint(200, 50), CPoint(200, 200), CPoint(50, 200) };

    // 4. Draw the polyline
    pDC->Polyline(points, 4);

    // 5. Restore the original pen
    pDC->SelectObject(pOldPen);     */

void UnderlineText::drawLine(Point* pts) {
CPen  pen;
CPen* prevPen;
int   thickness = cdcEx.scaleY(1);
uint  black     = RGB(0,0,0);

  pen.CreatePen(PS_SOLID, thickness, black);

  prevPen = cdcEx.selectObj(&pen);

    cdcEx.polyLine(pts, 2);

  cdcEx.selectObj(prevPen);
  }



void UnderlineText::debug(Point* pts) {
static FileIO fo;
static bool   opened{false};
String s;

  if (!opened) {
    opened = fo.open(_T("..\\Data\\Log.txt"), FileIO::Write|FileIO::Create);
    fo.setMode(FileIO::Read | FileIO::Write);
    }
  if (!fo.isOpen()) {fo.reOpen();   fo.seekEnd();}

  s.format(_T("(%i,%i), (%i,%i)"), pts[0].x, pts[0].y, pts[1].x, pts[1].y);

  fo.write(s);   fo.crlf();
  fo.close();
  }




////////------------------
#if 0
  data[0](x, y);
  String s;   s.format(_T("Start: (%i, %i), n = %i"), data[0].x, data[0].y, n);   messageBox(s);
  n++;
#endif

//String s;  s.format(_T("%s: d[%i](%i, %i)"), prefix, n, x, y);   messageBox(s);
#if 0
  data[n](x, y);

  s.format(_T("End: (%i, %i)"), data[n].x, data[n].y);   messageBox(s);

  n++;
#endif
#if 0
  data[n](x, y);   n++;
#endif
#if 0
if (x > 2000 || curPoint.x > 2000) {
String s;  s.format(_T("(%i, %i) - (%i, %i)"), curPoint.x, curPoint.y, x, y);
  messageBox(s);
  }
#endif

