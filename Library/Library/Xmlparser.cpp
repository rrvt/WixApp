// XML Parser -- Data goes into the StoreX (see StoreX.h)


#include "pch.h"
#include "XmlParser.h"
#include "XmlElement.h"


void XMLparser::load(XmlElement& elem) {
XMLType  ty;
String   elementName;
XmlElement* element = 0;

  for (ty = next(); ty != EofXml; ty = next()) {

    switch (ty) {
      case NilXml   : break;
      case CntnXml  : elem.addContent(frag, tag, CntnTag); break;
      case SmStrtXml: load(elem.addElement(SmplTag, tag, attributes, frag)); break;
      case EnStrtXml: load(elem.addElement(EnhcTag, tag, attributes, frag)); break;
      case EndTgXml : elem.addContent(frag, tag, EndTag); return;
      case OneTgXml : elem.addElement(ElemTag, tag, attributes, frag); break;
      case DeclXml  : elem.addContent(frag, tag, EnhcTag); break;
      }
    }
  }



typedef enum {
  other, white, bang, tab, lower, letre, upper, letrE, digit, sglqt,
  quote, lbsgn, atsgn, lparen, rparen, astrck, pluS, comma, minuS, dot, slash, colon, qustn, dollr,
  semi, lt, equaL, gt, lbrkt, bslsh, rbrkt, hat, sep, eol, cr, amper, lbrac, rbrac, vrtbr,
  sqgl, delch, eofch
  } Character_Classes;


// character class table

static Character_Classes character_class_table[] = {
  eofch, other, other, other, other, other, other, other, // 00 -
  other, tab,   eol,   other, white, cr,    other, other, // 08 -
  other, other, other, other, other, other, other, other, // 16 -
  other, other, eofch, other, other, other, other, other, // 24 -
  white, bang,  quote, lbsgn, dollr, other, amper, sglqt, // 32 -
  lparen,rparen,astrck,pluS,  comma, minuS, dot,   slash, // 40 -
  digit, digit, digit, digit, digit, digit, digit, digit, // 48 -
  digit, digit, colon, semi,  lt,    equaL, gt,    qustn, // 56 -
  atsgn, upper, upper, upper, upper, letrE, upper, upper, // 64 -
  upper, upper, upper, upper, upper, upper, upper, upper, // 72 -
  upper, upper, upper, upper, upper, upper, upper, upper, // 80 -
  upper, upper, upper, lbrkt, bslsh, rbrkt, hat,   sep,   // 88 -
  other, lower, lower, lower, lower, letre, lower, lower, // 96 -
  lower, lower, lower, lower, lower, lower, lower, lower, //104 -
  lower, lower, lower, lower, lower, lower, lower, lower, //112 -
  lower, lower, lower, lbrac, vrtbr, rbrac, sqgl,  delch, //120 -
  other, other, other, other, other, other, other, other, //128
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //Safety
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //
  other, other, other, other, other, other, other, other, //  -255
  };

// LessThan, FirstWht, EnhcStart, AlphaEnhc, LessSeen, SlashSeen, Slash, FindGT, GreaterThan, Question,
// GetAngl,


enum State {StartSt, SecondCh, GetContnt, GetTag, SmpSlsh, GetEnhc, GotSlsh};


XMLType XMLparser::next() {
State             state   = StartSt;
XMLType           typ     = NilXml;
Character_Classes chCls;

  frag.clear();   tag.clear();   attributes.clear();

  loop {

    nextChar();  chCls = character_class_table[tch & 0xff];

    switch (state) {

      case StartSt: switch (chCls) {
                      case lt   : state = SecondCh; break;
                      case white: acceptChar(); continue;
                      case eol  :
                      case cr   : acceptChar(); continue;
                      case eofch: return EofXml;
                      case amper: tch = getEntity();
                      default   : state = GetContnt; break;
                      }
                     frag += tch; acceptChar(); break;

      case SecondCh : switch (chCls) {
                        case slash: frag += tch; acceptChar();
                                    switch (getTag()) {
                                      case SmplTagTy  :
                                      case EnhcTagTy  :
                                      case EndSlsTy   :
                                      case EnhEndSlsTy:
                                      default         : return EndTgXml;
                                      }

                        case qustn: frag += tch; acceptChar();
                                    switch (getTag()) {
                                      case SmplTagTy  :
                                      case EnhcTagTy  :
                                      case EndSlsTy   :
                                      case EnhEndSlsTy:
                                      default         : return OneTgXml;
                                      }
                        case eol  : acceptChar(); return SmStrtXml;
                        case cr   : acceptChar(); continue;
                        case eofch: return SmStrtXml;
                        default   : switch (getTag()) {
                                      case SmplTagTy  : return SmStrtXml;
                                      case EnhcTagTy  : return EnStrtXml;
                                      case EndSlsTy   :
                                      case EnhEndSlsTy: return OneTgXml;
                                      default         : return EnStrtXml;
                                      }

                        }
                      frag += tch; acceptChar(); break;

      case GetContnt: switch (chCls) {
                        case lt   : state = StartSt; return CntnXml;
                        case eol  : acceptChar();
                        case eofch: state = StartSt; return CntnXml;
                        case cr   : acceptChar(); continue;
                        case amper: tch = getEntity();
                        default   : break;
                        }
                      frag += tch; acceptChar(); break;
      }
    }

  return NilXml;
  }


TagTy XMLparser::getTag() {
State             state   = GetTag;
TagTy             typ     = NilTagTy;
Character_Classes chCls;

  tag.clear();

  loop {

    nextChar();  chCls = character_class_table[tch & 0xff];

    switch (state) {

      case GetTag:  switch (chCls) {
                      case gt   : frag += tch; acceptChar(); return SmplTagTy;
                      case slash: state = SmpSlsh; break;
                      case white: state = GetEnhc; getAttributes(); continue;
                      case eol  : acceptChar(); return SmplTagTy;
                      case cr   : acceptChar(); continue;
                      case eofch: return SmplTagTy;
                      default   : tag += tch; break;
                      }
                    frag += tch; acceptChar(); break;

      case SmpSlsh: switch (chCls) {
                      case gt   : frag += tch; acceptChar(); return EndSlsTy;
                      case white: state = GetEnhc; break;
                      case eol  : acceptChar(); return SmplTagTy;
                      case cr   : acceptChar(); continue;
                      case eofch: return SmplTagTy;
                      default   : tag += tch; state = GetTag; break;
                      }
                    frag += tch; acceptChar(); break;

      case GetEnhc: switch (chCls) {
                      case gt   : frag += tch; acceptChar(); return EnhcTagTy;
                      case slash: state = GotSlsh; break;
                      case eol  : acceptChar(); return EnhcTagTy;
                      case cr   : acceptChar(); continue;
                      case eofch: return EnhcTagTy;
                      default   : break;
                      }
                    frag += tch; acceptChar(); break;

      case GotSlsh: switch (chCls) {
                      case gt   : frag += tch; acceptChar(); return EnhEndSlsTy;
                      case white: break;
                      case eol  : acceptChar(); return EnhEndSlsTy;
                      case cr   : acceptChar(); continue;
                      case eofch: return EnhEndSlsTy;
                      default   : state = GetEnhc; break;
                      }
                    frag += tch; acceptChar(); break;
      }
    }
  }


enum ModState {WhiteMod, NameMod, EqualMod, QuoteMod, ValueMod};


void XMLparser::getAttributes() {
Character_Classes chCls;
ModState          state = WhiteMod;
String            s;
XmlAttribute*     attr = 0;

  attributes.clear();

  loop {

    nextChar();  chCls = character_class_table[tch & 0xff];

    switch (state) {
      case WhiteMod : switch (chCls) {
                        case white: frag += tch; acceptChar(); continue;
                        case letrE:
                        case letre:
                        case lower:
                        case upper: state = NameMod; s.clear(); attr = 0; break;
                        default   : return;
                        }
                      s += tch; frag += tch; acceptChar(); break;


      case NameMod  : switch (chCls) {
                        case equaL: frag += tch; attr = attributes.add(s);
                                    state = QuoteMod; acceptChar(); continue;
                        case white: frag += tch; attr = attributes.add(s);
                                    state = EqualMod; acceptChar(); continue;
                        case letrE:
                        case letre:
                        case lower:
                        case upper: break;
                        default   : attributes.add(s); return;
                        }
                      s += tch; frag += tch; acceptChar(); break;

      case EqualMod : switch (chCls) {
                        case white: break;
                        case quote: state = ValueMod; break;
                        case equaL: state = QuoteMod; break;
                        default   : return;
                        }
                      frag += tch; acceptChar(); break;

      case QuoteMod : switch (chCls) {
                        case white: break;
                        case quote: state = ValueMod; break;
                        default   : return;
                        }
                      frag += tch; acceptChar(); break;

      case ValueMod : switch (chCls) {
                        case quote: frag += tch; if (attr) attr->value = s; acceptChar();
                                    state = WhiteMod; continue;
                        case amper: tch = getEntity(); break;
                        case eol  :
                        case cr   :
                        case eofch:
                        case gt   : if (attr) attr->value = s; return;
                        default   : break;
                        }
                      s += tch; frag += tch; acceptChar(); break;
      }
    }
  }


/*
There are five predefined entities:

    &lt;   represents "<";
    &gt;   represents ">";
    &amp;  represents "&";
    &apos; represents "'";
    &quot; represents '"'.
*/

enum EntityState {StartEnt, GotAEnt, SemiEnt};


Tchar XMLparser::getEntity() {
EntityState state = StartEnt;
Tchar       x;

  acceptChar();

  loop {

    nextChar();

    switch (state) {

      case StartEnt : switch (tch) {
                        case _T('l'): x = _T('<'); state = SemiEnt; break;
                        case _T('g'): x = _T('>'); state = SemiEnt; break;
                        case _T('q'): x = _T('"'); state = SemiEnt; break;
                        case _T('a'): state = GotAEnt; break;;
                        default     : return _T('&');
                        }
                      frag += tch; acceptChar(); break;;

      case GotAEnt  : switch (tch) {
                        case _T(';'): frag += tch; acceptChar(); return _T('&');
                        case _T('p'): x = _T('\''); state = SemiEnt; break;
                        case _T('m'):
                        default     : x = _T('&');  state = SemiEnt; break;
                        }
                      frag += tch; acceptChar(); break;;

      case SemiEnt  : frag += tch;
                      if (tch != _T(';')) {acceptChar(); continue;}
                      acceptChar(); return x;
      }
    }
  }



void XMLparser::nextChar() {

  loop {if (getNext) {if (!ar.read(tch)) tch = 26;}   if (++cnt > 3 || tch < 0xBB) break;}

  getNext = false;
  }

