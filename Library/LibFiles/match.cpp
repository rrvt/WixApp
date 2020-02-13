// Regular Expression Match
// rrvt 11/16/96

// Regular expressions are made up of the following meta characters and ordinary
// characters:
//
//    ^  - At beginning of pattern indicates matches the beginning of line,
//         otherwise just an ordinary character.
//    $  - At end of pattern indicates matches the end of the line, otherwise
//         just an ordinary character.
//    .  - Matches any character.
//    [] - The set of characters contained within the brackets matches the
//         character at the corresponding position in the line
//    *  - Kleene Closure of the preceding pattern (Tchar, metachar or set).
//    \  - Escape character, next character is used literally (even metachar).
//         \t indicates the tab character.
// Sets
//    ^  - When the first Tchar in the set [^...] indicates that the match is for
//         any character NOT in the set.
//    -  - When dash (-) is not the first or last character in the set,
//         indicates that a range of characters is to be placed in the set.  For
//         example [0-9] indicates all the digits.
//    \  - When not the last character indicates that the next character is to
//         be placed in the set literally (not a meta character).  For example
//         [0-9\-] indicates the digits and dash is in the set.



#include "stdafx.h"
#include "match.h"


#define      setIndex(ch)  ((ch >> 5) & 0x0007)
#define      indexMask(ch) ((ulong) 1 << (ch & 0x001f))


// Search a file for a regular expression

void RegExpr::searchFile(FILE* lu, processLine&) {

  while (_fgetts(line, noElements(line), lu)) if (match(line)) processLine(line);
  }


bool RegExpr::match(TCchar* stg) {

  if (pattern[0].key == Bol) {
    if (anchoredMatch(stg, &pattern[1])) return true;
    }

  else if (unanchoredMatch(stg, pattern)) return true;

  return false;
  }


// Unanchored Match pattern to substring anywhere in line

Tchar* RegExpr::unanchoredMatch(TCchar* line, Pattern pat[]) {

  if (!line) return 0;

  for (; *line; line++) if (anchoredMatch(line, pat)) return (Tchar*) line;

  return 0;
  }



// Anchored Match pattern to line
//   Returns pointer to next position in line or
//   zero if no match

Tchar* RegExpr::anchoredMatch(TCchar* line, Pattern pat[]) {
short    i;
short    rslt;
Pattern* p;
TCchar*  subStg;
Tchar*   nextStg;

  for (i = 0; i < NoPattern && pat[i].key != EndOfPat; i++) {
    if (pat[i].key != KleeneClosure) {

      rslt = omatch(line, &pat[i]);

      if      (rslt < 0) return 0;
      else if (rslt > 0) line++;
      }

    else {
      p = &pat[i+1]; subStg = line;

      for (; *subStg;) {
        rslt = omatch(subStg, p);

        if      (rslt < 0) break;
        else if (rslt > 0) subStg++;
        }

      for (; subStg >= line; subStg--) {
        nextStg = anchoredMatch(subStg, &pat[i+2]);
        if (nextStg) return nextStg;
        }

      return 0;
      }
    }

  return (Tchar*) line;
  }


// Match one character to one pattern
// Return -1 - fails to match
//     0 - succeeds but do not advance
//     1 - succeeds and advance

short RegExpr::omatch(TCchar* s, Pattern* pat) {
Tchar ch = *s;

  switch (pat->key) {

    case Any          : if (ch && ch != _T('\n')) return 1;     break;

    case Ch           : if (pat->ch == ch) return 1;       break;

    case Eol          : if (ch == _T('\n') || !ch) return 0; break;

    case CharClass    : if (inSet(ch, pat->set)) return 1;   break;

    case KleeneClosure: break;
    }

  return -1;
  }


// Translate a file name with wildcard characters into a regular expression

bool RegExpr::setWildCardPattern(TCchar* name) {
TCchar* p = name;
Tchar   re[512];
Tchar*  q = re;
Tchar*  e = re + noElements(re) - 3;

  *q++ = _T('^');

  for (; *p && q < e; p++) {
    if      (*p == _T('?'))  *q++ = _T('.');

    else if (*p == _T('*')) {*q++ = _T('.');  *q++ = _T('*');}

    else if (*p == _T('.')) {
      if (*(p+1))            {*q++ = _T('\\'); *q++ = *p;}
      else                   continue;
      }

    else if (*p == _T('$')) {*q++ = _T('\\'); *q++ = *p;}

  else                     *q++ = *p;
    }

  *q++ = _T('$'); *q = 0;

  return setPattern(re);
  }


// get pattern from String

bool RegExpr::setPattern(TCchar* pat) {
short i;
Tchar ch;

  if (!pat) return false;

  for (i = 0; (ch = *pat) != 0 && i < noElements(pattern); pat++, i++) {

    switch (ch) {
      case _T('^')    : if (i) goto processChar;
                        pattern[i].key = Bol; break;

      case _T('$')    : if (*(pat + 1)) goto processChar;
                        pattern[i].key = Eol; break;

      case _T('.')    : pattern[i].key = Any; break;

      case _T('[')    : pattern[i].key = CharClass;
                        if (!collectSet(&pat, &pattern[i])) return false;
                        break;

      case _T('*')    : if (!installKleene(i)) return false;
                        break;

      case pathSepChar: ch = *++pat;
                        if (!ch) return false;
                        if (ch == _T('t')) ch = _T('\t');   // Tab character?
                                                      // else fall through
processChar:
      default         : pattern[i].key = Ch; pattern[i].ch = ch; break;
      }
    }

  pattern[i].key = EndOfPat; return true;
  }


bool RegExpr::collectSet(TCchar** s, Pattern* p) {
TCchar* stg    = *s;
Tchar   ch;
Tchar   lastch = 0;
bool    hat    = false;

  clrSet(p->set);

  ch = *++stg;

  if (ch && ch == _T('^')) {hat = true; stg++;}

  for (; (ch = *stg) != 0 && ch != _T(']'); stg++) {

    if (ch == _T('-') && lastch && *(stg+1)) {
      ch = *++stg;  if (lastch > ch) return false;

      for (; lastch <= ch; lastch++) setBit(lastch, p->set);

      lastch = 0; continue;
      }

    if (ch == pathSepChar && *(stg+1)) {
      ch = *++stg;  if (ch == _T('t')) ch = _T('\t');
      }

    setBit(ch, p->set); lastch = ch;
    }

  *s = stg;

  if (hat) {setBit(0, p->set); setBit(_T('\n'), p->set); invertSet(p->set);}
  return true;
  }


bool RegExpr::installKleene(short i) {
short  j = i-1;
PatKey key;

  if (j < 0) return false;

  key = pattern[j].key;
  if (key == Bol || key == Eol || key == KleeneClosure) return false;

  pattern[i] = pattern[j]; pattern[j].key = KleeneClosure; return true;
  }


void RegExpr::clrSet(ulong set[]) {
short i;

  for (i = 0; i < SetSize; i++) set[i] = 0;
  }


void RegExpr::setBit(Tchar ch, ulong set[]) {set[setIndex(ch)] |= indexMask(ch);}


bool RegExpr::inSet(Tchar ch, ulong set[]) {return (set[setIndex(ch)] & indexMask(ch)) != 0;}


void RegExpr::invertSet(ulong set[]) {
short i;

  for (i = 0; i < SetSize; i++) set[i] ^= -1L;
  }
