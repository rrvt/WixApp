// Lexical Analyzer
// rrvt 9/1/94
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "LexErr.h"
#include "Token.h"
#include <float.h>
#include <math.h>

class String;


struct Input {

               Input() {}
  virtual     ~Input()       = 0;

  virtual bool set(void* p)  {return false;}
  virtual void initialize()  = 0;
  virtual Tchar operator()()  = 0;             // Returns one character or zero (end of file)
  virtual void acceptChar()  = 0;
  virtual void backOneChar() = 0;             // Use only once before going forward again...
  };


inline Input::~Input() {}


template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
class LexT {

typedef enum {
  nil, begin_tok, collect_symbol, got_slash, collect_white,
  collect_comment, collect_bounded_comment, get_right_slash,
  collect_integer, got_dot, collect_fraction, got_E, collect_exponent,
  collect_string, collect_single_quote, sqt_backslash, collect_final_sq, got_backslash, got_colon,
  got_l_angle_brkt, got_r_angle_brkt, beginHex, got_eol, end_of_file, illegal_tok
  } States;

CFileException fileError;

Tchar    backOne;

String   source_line;
String   source_line1;
String*  source[2];             // = {source_line, source_line1};
int      current_source;        // Index of current source line
String*  pline;                 // Pointer to end of source line String
int      pos;                   // Initial offset to convert tabs to spc

String*  ptok;                  // Pointer to current token name field

unsigned long val;              // Collect integer part of number
unsigned long last_val;         // Used to check for overflow
long     no_digits;             // Number of digits seen
long     frac;                  // collect fraction part of number
long     divisor;               // collect divisor (10^n, n = 0..9)
int      exponent;              // collect exponent value
bool     neg_exponent;          // negative exponent seen when true
double   ten_exp;               // value of 10^exponent

States   state;                 // Current state

bool     startHex;              // Start Hex Integer when true

Tchar    ch;                    // Current character

bool     print_source_line;     // print source line when true
int      line_number;           // current line number
int      offset;                // offset to token

int      error_count;           // number of syntax errors
LexErr   lexErr;

public:

Input   input;                   // Input is performed with this class whatever it is.

Token*  token;                   // token returned    static const
Token*  token1;                  // Next token in input stream

  LexT();
 ~LexT() {NewAlloc(Token); FreeNode(token); FreeNode(token1); token = token1 = 0;}

  void        initialize();                      // Initialize input mechanism

  bool        setInput( void* p) {return input.set(p);}
  void        setOutput(void* p) {output.set(p);}

  void        set_print_flag(bool flag);         // Set print flag, true will cause
  bool        get_print_flag(void);              // source line to be printed

  // Set flag so that the third token will be processed as a hex number if it is one.

  void        set_collect_hex() {state = beginHex; startHex = true;}

  // get next token, values set in globals

  TokenCode   get_token(void);

  void        accept_token(void);                // Set first token to NO_Token
  void        accept_two_tokens(void);           // Set both tokens to No_Token

  void        error(Token* token, TCchar* stg);    // Error processing

  int         noErrors()  {return error_count;}
  LexErr&     lastError() {return lexErr;}

private:

// Local Functions

  void finishInit();

  void next_tok(Token* tok);

  void start_token(Token* tok)   {ptok = &tok->name; ptok->clear();}

  void accept_char()   {input.acceptChar();}

  void add_to_line()   {*pline += ch; offset++;}

  void move_char()     {accept_char(); *ptok += ch; *pline += ch; offset++;}      // add_to_line() too

  void terminate(Token* tok, String* source[])
                                  {tok->line_number = line_number; tok->psource = source[current_source];}

  // display source line associated with current token.
  // Return offset of current token from left margin

  void display_source_line(Token* token);

  void output(TCchar* tc);
  };



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



template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
             LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::LexT() : pline(0), backOne(0), error_count(0) {
  NewAlloc(Token); token = AllocNode; token1 = AllocNode;
  source[0] = &source_line; source[1] = &source_line1;
  }


// open lexical analyser file

template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
                              void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::initialize() {
  input.initialize();  finishInit();
  }



template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
                              void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::finishInit() {
  state = begin_tok;

  current_source = 0; pline = source[current_source]; pline->clear();
  token->line_number = line_number = 1;
  offset      = 0;
  val = last_val = no_digits = frac = divisor = exponent = 0;
  neg_exponent = startHex = false;
  accept_two_tokens(); startHex = false; ch = backOne = 0;
  print_source_line = false;   error_count = 0; lexErr.clear();
  }


// Set print flag, true will cause source line to be printed

template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
            void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::set_print_flag(bool flag)
                                                                              {print_source_line = flag;}


template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
                bool LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::get_print_flag(void)
                                                                              {return print_source_line;}


// accept token by clearing the token code

template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
                      void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::accept_token(void)
                                                                                {token->code = NoToken;}

template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
  void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::accept_two_tokens(void)
                                                                  {token->code = token1->code = NoToken;}


// get next token when token is empty, values set in globals

template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
                      TokenCode LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::get_token(void) {
Token* p = 0;

  if (token->code != NoToken) return token->code;
  p = token; token = token1; token1 = p;
  if (token->code == NoToken) next_tok(token);
  next_tok(token1);
  return token->code;
  }


template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
                       void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::next_tok(Token* tok) {
Character_Classes ch_class;                 // character class of current character

  ptok = &tok->name; ptok->clear();

  loop {

    ch = backOne ? backOne : input(); backOne = 0; ch_class = character_class_table[ch];

    switch (state) {

      // Begin Token State, Look for the first character of each token type
      // Ignore spaces, return, line feed (end-of-line)
      // Convert tabs to spaces (the display always moves to specific columns)

      case begin_tok:

        switch (ch_class) {

          case white:   if (WhiteSpace) {state = collect_white; start_token(tok);}   break;

proc_tab: case tab:     pos = offset;
                        offset = (offset + 8) / 8; offset = offset * 8;
                        while (pos++ < offset) *pline += ' ';
                        if (WhiteSpace) {state = collect_white; start_token(tok); break;}
                        accept_char(); continue;

          case cr:      accept_char(); continue;

          case eol:     if (print_source_line) {terminate(tok, source); display_source_line(tok);}
                        current_source = (current_source + 1) % 2;
                        state = got_eol;
fin_eol:                line_number++; offset = 0; pline = source[current_source]; pline->clear();
                        if (WhiteSpace) {tok->code = EolToken; goto fin_op;}
                        ptok = &tok->name; ptok->clear(); accept_char(); continue;

          case letre:
          case letrE:
          case lower:
          case upper:   state = collect_symbol; start_token(tok); break;

          case digit:   val = ch - '0'; no_digits = 1; start_token(tok);
                        state = collect_integer; break;

          case sglqt:   state = collect_single_quote; start_token(tok); break;

          case quote:   state = collect_string;
                        start_token(tok); accept_char(); add_to_line(); continue;

          case dot:     state = got_dot; val = 0; no_digits = 0;
                        start_token(tok); break;

          case equaL:   tok->code = EQToken;        goto fin_op;
          case bang:    tok->code = BangToken;      goto fin_op;
          case astrck:  tok->code = AsterickToken;  goto fin_op;
          case lbsgn:   tok->code = LbSignToken;    goto fin_op;
          case atsgn:   tok->code = AtSignToken;    goto fin_op;
          case lparen:  tok->code = LParenToken;    goto fin_op;
          case rparen:  tok->code = RParenToken;    goto fin_op;
          case dollr:   tok->code = DollarToken;    goto fin_op;
          case minuS:   tok->code = MinuSToken;     goto fin_op;
          case pluS:    tok->code = PluSToken;      goto fin_op;
          case lbrkt:   tok->code = LBracketToken;  goto fin_op;
          case rbrkt:   tok->code = RBracketToken;  goto fin_op;
          case semi:    tok->code = SemicolonToken; goto fin_op;
          case comma:   tok->code = CommaToken;     goto fin_op;
          case amper:   tok->code = AndToken;       goto fin_op;
          case lbrac:   tok->code = LBraceToken;    goto fin_op;
          case rbrac:   tok->code = RBraceToken;    goto fin_op;
          case hat:     tok->code = HatToken;       goto fin_op;
          case sqgl:    tok->code = SqglToken;      goto fin_op;
          case delch:   tok->code = DelChToken;     goto fin_op;
          case vrtbr:   tok->code = OrToken;
fin_op:                 start_token(tok); move_char(); terminate(tok, source); return;

          case slash:   state = got_slash;        start_token(tok); break;
          case colon:   state = got_colon;        start_token(tok); break;

          case lt:      state = got_l_angle_brkt; start_token(tok); break;
          case gt:      state = got_r_angle_brkt; start_token(tok); break;

          case eofch:   goto eof;

          default:      start_token(tok); state = illegal_tok;
          }
        move_char(); continue;


      case collect_white:
        switch (ch_class) {
          case white: break;
          default   : tok->code = WhiteToken; goto fin_tok;
          }
        move_char(); continue;


// Collect symbol composed of a letter followed by letters, digits or underscore

      case collect_symbol:

        switch (ch_class) {
          case digit:
          case sep:
          case letre:
          case letrE:
          case lower:
          case upper:   break;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      tok->code = IdentToken;
                        terminate(tok, source);
                        state = begin_tok; return;
          }
        move_char(); continue;


// Collect Integer (or possibly a real)

      case collect_integer:

        switch (ch_class) {

            case digit: last_val = val; val = 10 * val + (ch - '0'); no_digits++;

                        if ((val < last_val || no_digits > 11) && no_digits <= 12) {
                          move_char(); terminate(tok, source);
                          error(tok, _T("Integer overflow")); continue;
                          }
                        break;

          case dot:     state = got_dot; break;

          case letrE:
          case letre:   tok->value.real = (float) val;
                        state = got_E; tok->code = RealLitToken; break;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      tok->code = IntLitToken; tok->value.integer = val;
                        tok->noDigits = no_digits;
fin_tok:                state = begin_tok; terminate(tok, source); return;
          }
        move_char(); continue;


// Continue processing a real

      case got_dot:
        switch (ch_class) {

          case digit:   frac = ch - '0'; divisor = 10; state = collect_fraction;
                        if (no_digits > FLT_DIG) {
                          move_char(); terminate(tok, source);
                          error(tok, _T("Invalid Number")); continue;
                          }
                        break;

          case letrE:
          case letre:   tok->value.real = (float) val;
                        state = got_E; tok->code = RealLitToken; break;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      if (no_digits > 0) {
                          tok->code = RealLitToken; tok->value.real = (float) val;
                          }
                        else tok->code = PeriodToken;
                        goto fin_tok;
          }
        move_char(); continue;


// Get fractional part of a real

      case collect_fraction:

        switch (ch_class) {

          case digit:   frac = 10*frac + (ch - '0'); divisor *= 10; no_digits++;
                        if (no_digits > FLT_DIG ) {
                          move_char(); terminate(tok, source);
                          error(tok, _T("Invalid Number")); continue;
                          }
                        break;

          case letrE:
          case letre:   tok->value.real = (float) val + (float) frac / (float) divisor;
                        state = got_E; tok->code = RealLitToken; break;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      tok->code = RealLitToken;
                        tok->value.real = (float) val + (float) frac / (float) divisor;
                        goto fin_tok;
          }
        move_char(); continue;


      case got_E:
        switch (ch_class) {

          case minuS:   neg_exponent = true;
                        exponent = 0; state = collect_exponent; break;

          case pluS:    neg_exponent = false;
                        exponent = 0; state = collect_exponent; break;

          case digit:   exponent = ch - '0'; neg_exponent = false;
                        state = collect_exponent; break;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      terminate(tok, source); error(tok, _T("Invalid Exponent"));
                        goto fin_tok;
          }
        move_char(); continue;


// Collect exponent value of real

      case collect_exponent:
        switch (ch_class) {

          case digit:   exponent = 10*exponent + (ch - '0'); break;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      if (exponent <= (FLT_MAX_10_EXP - (neg_exponent?1:0)))
                          ten_exp = pow(exponent, 10.0);
                        else {
                          terminate(tok, source); error(tok, _T("Invalid Exponent"));
                          ten_exp = 1.0;
                          }

                        if (neg_exponent) tok->value.real /= ten_exp;
                        else              tok->value.real *= ten_exp;
                        goto fin_tok;
          }
        move_char(); continue;


// Collect String or character literal with hardly any discrimination between
// the two

      case collect_string:

        switch(ch_class) {
          case bslsh:   if (BSinQuote) break;
                        state = got_backslash; accept_char(); add_to_line(); continue;

          case quote:   tok->code = StringToken; accept_char(); add_to_line(); goto fin_tok;

          case cr:
          case eol:     if (QuoteEol) {
                          terminate(tok, source); error(tok, _T("Expected closing quote"));
                          tok->code = StringToken; goto fin_tok;
                          }
          default:      break;
          }
        move_char(); continue;


// Look for second quote as a method or allowing a quote to be in the String

      case got_backslash:
        switch (ch_class) {
          case cr:
          case eol:     state = collect_string; continue;
          default:      state = collect_string; break;
          }
        move_char(); continue;


      case collect_single_quote:
        switch (ch_class) {
          case cr   :
          case eol  :
          case eofch: tok->code = ApostropheToken; terminate(tok, source); state = begin_tok; return;
          case bslsh: state = sqt_backslash; break;
          default   : state = collect_final_sq; break;
          }
        move_char(); continue;

      case sqt_backslash:
        switch (ch_class) {
          case cr   :
          case eol  :
          case eofch: tok->code = ApostropheToken; terminate(tok, source); state = begin_tok; return;
          default   : state = collect_final_sq; break;
          }
        move_char(); continue;

      case collect_final_sq:
        switch (ch_class) {
          case sglqt  : tok->code = CharToken; accept_char(); add_to_line(); goto fin_tok;

          default     : tok->code = ApostropheToken;
                        backOne = (*ptok)[1]; ptok->resize(1); input.backOneChar();
                        int n = pline->size(); pline->resize(n-1); offset--;
                        terminate(tok, source); state = begin_tok; return;
          }
        continue;

      case got_colon:

        switch (ch_class) {

          case equaL:   tok->code = ColonEqualToken; move_char(); goto fin_tok;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      tok->code = ColonToken; goto fin_tok;
          }


      case got_l_angle_brkt:

        switch (ch_class) {

          case equaL:   tok->code = LEToken; move_char(); goto fin_tok;

          case gt:      tok->code = NEToken; move_char(); goto fin_tok;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      tok->code = LTToken; goto fin_tok;
          }


      case got_r_angle_brkt:

        switch (ch_class) {

          case equaL:   tok->code = GEToken; move_char(); goto fin_tok;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      tok->code = GTToken; goto fin_tok;
          }


      case got_slash:

        switch (ch_class) {

          case slash:   state = collect_comment; break;
          case astrck:  state = collect_bounded_comment; break;

          case white:   add_to_line();
          case cr:      if (!WhiteSpace) accept_char();
          default:      tok->code = SlashToken; goto fin_tok;
          }
        move_char(); continue;


// Collect comment = // ... end of line

      case collect_comment:

        switch (ch_class) {

          case cr:      accept_char(); continue;

          case eol:
          case eofch:   tok->code = CommentToken; terminate(tok, source); state = begin_tok; return;

          default:      break;
          }
        move_char(); continue;


      case collect_bounded_comment:

        switch (ch_class) {
          case astrck:  state = get_right_slash; break;

          case eol:     if (print_source_line) {terminate(tok, source); display_source_line(tok);}
                        current_source = (current_source + 1) % 2;
                        goto fin_eol;

          case eofch:   goto eof;

          default:      break;
          }
        move_char(); continue;


      case get_right_slash:

        switch (ch_class) {
          case slash:   state = begin_tok; break;

          case eol:     if (print_source_line) {terminate(tok, source); display_source_line(tok);}
                        current_source = (current_source + 1) % 2;
                        goto fin_eol;

          case eofch:   goto eof;

          default:      state = collect_bounded_comment; break;
          }
        move_char(); continue;


      case got_eol:

        switch (ch_class) {

          case white:   if (WhiteSpace) {state = collect_white; start_token(tok);}   break;

          case tab:     goto proc_tab;

          case cr:      accept_char(); continue;

          case eol:     if (print_source_line) {terminate(tok, source); display_source_line(tok);}
                        state = begin_tok; goto fin_eol;

eof:      case eofch:   tok->code = EOFToken;
                        state = end_of_file; terminate(tok, source);
                        if (print_source_line) display_source_line(tok);
                        return;

          default:      state = begin_tok; continue;
          }
        move_char(); continue;


// End of file has been seen, do nothing except return end of file tokens

      case end_of_file: tok->code = EOFToken; terminate(tok, source); return;


// Illegal token, gather all illegal characters into one token and return as a
// group

      case illegal_tok:
        switch (ch_class) {
          case white:  case tab:   case cr:    case eol:
          case letre:  case letrE: case lower: case upper:
          case digit:  case quote: case dot:   case astrck:
          case lparen: case rparen:case minuS: case pluS:  case equaL:
          case lbrkt:  case rbrkt: case semi:  case comma: case amper:
          case vrtbr:  case slash: case colon: case lt:    case gt:
          case eofch:
                        tok->code = IllegalToken; goto fin_tok;

          default:      break;
          }
        move_char(); continue;


// This should never happen, but ...

      default:          state = begin_tok; continue;
      };
    }
  }


// Functions

template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
           void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::error(Token* token, TCchar* stg) {
int    lng    = (int) _tcslen(stg);
String f;

  display_source_line(token);

  f.format(_T("%i: Token: %s - Error: %s\n"),  error_count, token->name, stg);

  output(stg);   error_count++;
  }


// display source line associated with current tok.
// Return offset of current tok

template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
            void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::display_source_line(Token* tok) {
String* pstg   = tok->psource;
String  f;

  pstg->trim();

  if (pstg->empty()) {output(f);}                         //notePad << f << nCrlf;

  else if (offset  < 74) {f.format(_T("%3i:  %s\n"), tok->line_number, pstg->str()); output(f);}

  else {
    f.format(_T("%3i:  %s\n"), tok->line_number, pstg->substr(0, 74).str());   output(f);

    f.format(_T("      %s\n"), pstg->substr(74).str()); output(f);
    }
  }


template<class Input, bool WhiteSpace, bool QuoteEol, bool BSinQuote>
void LexT<Input, WhiteSpace, QuoteEol, BSinQuote>::output(TCchar* tc) {lexErr.addLine(tc);}


