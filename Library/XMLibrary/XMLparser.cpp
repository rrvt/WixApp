// XML Parser -- Data goes into the Store (see Store.h)


#include "pch.h"
#include "XMLparser.h"
#include "MessageBox.h"
#include "XMLstore.h"


static void isTagError(String& start, String& end);


// Used to find all the tags in a line

class XMLtag {
TCchar* p;

public:

  XMLtag(String& s) : p(s) { }                    // line on which to work

  XMLType get(String& tag);

private:

  XMLtag() : p(0){ }
  };



void XMLparser::input(Archive& ar) {
String   line;
XMLType  typ;
String   tag;
XMLtree* element = 0;
XMLleaf*  attrib;

  while (readLine(ar, line)) {

    typ = getType(line, tag);

    switch (typ) {
      case ElementTag: attrib = xmlStore.getAttrib();         // <tag ... />
                       attrib->typ  = AttribTag;
                       attrib->tag  = tag;
                       attrib->line = line;
                       xmlStore.add(attrib);   break;

      case StartTag  : element = xmlStore.getElement();       // <tag ... >
                       element->typ        = ElementTag;
                       element->tag        = tag;
                       element->start.typ  = typ;
                       element->start.tag  = tag;
                       element->start.line = line;
                       xmlStore.add(element);   break;

      case EndTag    : element = xmlStore.getCurrent();       // </tag>
                       isTagError(element->start.tag, tag);
                       element->end.typ  = typ;
                       element->end.tag  = tag;
                       element->end.line = line;
                       xmlStore.setParent();    break;

      case DeclTag   :                                        // <?tag ...>
      case AttribTag :                                        // First char not space, tab or '<'
      default        : attrib = xmlStore.getAttrib();
                       attrib->typ  = typ;
                       attrib->tag  = tag;
                       attrib->line = line;
                       xmlStore.add(attrib);   break;
      }
    }
  }



bool XMLparser::readLine(Archive& ar, String& s) {
Tchar  tch;

  s.clear();

  while (ar.read(tch)) {
    if (tch == _T('\n')) return true;
    if (tch == _T('\r')) continue;
    s += tch;
    }

  return !s.isEmpty();
  }


XMLType XMLparser::getType(String& s, String& tag) {XMLtag xmlTag(s);   return xmlTag.get(tag);}


// Get line type and the tag if any, tags come in several flavors
// <tag ... />                      // single line
// <tag .. > ... </tag>             // single line double tag (start and end)
// <tag ... >                       // start tag followed by multiple lines to an end tag
// </tag>                           // An end tag which must match a start tag at the same level
                                    // in the tree
enum TagState {StartSt,   GetTypeSt,  GetStartSt, GetCmpltSt,
               FindEndSt, FindSlshSt, FinEndSt,   GetEndSt, GetDeclSt
               };

XMLType XMLtag::get(String& tag) {
Tchar    tch;
TagState state   = StartSt;
XMLType  typ     = NilTag;
bool     collect = true;
String   s;

  if (!p) return NilTag;    tag.clear();

  for (tch = *p++; tch; tch = *p++) {
    switch (state) {

      case StartSt    : if (tch == _T('<')) {state = GetTypeSt;  typ = StartTag;   break;}
                        if (tch >  _T(' ')) return AttribTag;
                        break;

      case GetTypeSt  : if (tch == _T('/')) {state = GetEndSt;    typ = EndTag;     break;}
                        if (tch == _T('?')) {state = GetDeclSt;   typ = DeclTag;    break;}
                        state = GetStartSt;                         // fall through to GetStartSt

      case GetStartSt : if (tch == _T('>')) {state   = FindEndSt;  break;}
                        if (tch == _T('/')) {state   = GetCmpltSt; break;}
                        if (tch == _T(' ')) {collect = false;      break;}
                        if (collect) tag += tch;                   break;

      case FindEndSt  : if (tch == _T('<'))  state = FindSlshSt;   break;

      case FindSlshSt : if (tch != _T('/')) {state = FindEndSt;    break;}
                        state = FinEndSt;                          break;

      case FinEndSt   : if (tch == _T('>')) {isTagError(tag, s);   return ElementTag;}
                        s += tch; break;

      case GetCmpltSt : if (tch == _T('>')) return ElementTag;
                        if (collect) {tag += _T('/'); tag += tch;}
                        state = GetStartSt; break;

      case GetEndSt   : if (tch == _T('>')) return typ;
                        if (tch == _T(' ')) collect = false;
                        if (collect)        tag += tch;
                        break;

      case GetDeclSt  : if (tch == _T(' ')) return typ;
                        tag += tch; break;
      }
    }

  return typ;
  }


void isTagError(String& start, String& end) {
String err;

  if (start != end) {
    err.format(_T("Tag Error: %s, %s"), start, end);   messageBox(err);
    }

  }


void XMLparser::output(Archive& ar) {xmlStore.output(ar);}




//////-----------------

#if 0
      case EndTag     : element->endTag = line;
                        if (elementName != element->name) {
                          messageBox(_T("End XMLtag name does not match Start XMLtag name.")); return;
                          }
                        element = xmlStore.parent(element);    break;

#endif

