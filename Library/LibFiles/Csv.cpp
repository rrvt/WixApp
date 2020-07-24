// csv


#include "stdafx.h"
#include "Csv.h"





typedef enum {other, quote, comma, sChar, bslsh, eol, cr, delch, eofch} Character_Classes;


// character class table

static Character_Classes character_class_table[] = {
  other, other, other, other, other, other, other, other, // 00 -
  other, sChar,   eol, other, sChar, cr,    other, other, // 08 -
  other, other, other, other, other, other, other, other, // 16 -
  other, other, eofch, other, other, other, other, other, // 24 -
  sChar, sChar, quote, sChar, sChar, sChar, sChar, sChar, // 32 -
  sChar, sChar, sChar, sChar, comma, sChar, sChar, sChar, // 40 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, // 48 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, // 56 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, // 64 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, // 72 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, // 80 -
  sChar, sChar, sChar, sChar, bslsh, sChar, sChar, sChar, // 88 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, // 96 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //104 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //112 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, delch, //120 -
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //128
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //Safety
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //
  sChar, sChar, sChar, sChar, sChar, sChar, sChar, sChar, //  -255
  };



Csv::Csv(Archive& arch) : ar(arch), getNext(true) {
NewAlloc(Token);

  token = AllocNode; token1 = AllocNode; source[0] = &source_line; source[1] = &source_line1;
  initialize();
  }


// open lexical analyser file

bool Csv::initialize() {

  state = begin_tok;

  current_source = 0; pline = source[current_source]; pline->clear();
  token->line_number = line_number = 1;  tokenNo = 0;
  val = last_val = no_digits = frac = divisor = exponent = 0;
  neg_exponent = startHex = false;
  accept_two_tokens(); startHex = false; ch = quote_ch = 0;
  print_source_line = false;
  return true;
  }


// Set print flag, true will cause source line to be printed

void Csv::set_print_flag(bool flag) {print_source_line = flag;}


bool Csv::get_print_flag(void) {return print_source_line;}


// accept token by clearing the token code

void Csv::accept_token(void)       {token->code = NoToken;}

void Csv::accept_two_tokens(void) {token->code = token1->code = NoToken;}


// get next token when token is empty, values set in globals


TokenCode Csv::get_token() {
Token* p = 0;

  if (token->code != NoToken) return token->code;
  p = token; token = token1; token1 = p;
  if (token->code == NoToken) next_tok(token);
  next_tok(token1);
  return token->code;
  }


void Csv::next_tok(Token* tok) {
Character_Classes ch_class;                 // character class of current character

  ptok = &tok->name; ptok->clear();

  loop {

    nextChar();   ch_class = character_class_table[ch];

    switch (state) {

      // Begin Token State, Look for the first character of each token type
      // Ignore spaces, return, line feed (end-of-line)
      // Convert tabs to spaces (the display always moves to specific columns)

      case begin_tok:

        switch (ch_class) {

          case cr:      accept_char(); continue;

          case eol:     state = got_eol;
fin_eol:                ptok = &tok->name; ptok->clear(); tok->code = EolToken; goto fin_op;

          case other:
          case sChar:   state = collect_symbol; start_token(tok); break;


          case quote:   state = collect_string; quote_ch = ch; val = 0;
                        start_token(tok); accept_char(); add_to_line(); continue;

          case comma:   tok->code = CommaToken;
fin_op:                 start_token(tok); move_char(); terminate(tok, source); return;

          case eofch:   goto eof;

          default:      start_token(tok); state = illegal_tok;
          }
        move_char(); continue;


// Collect symbol composed of a letter followed by letters, digits or underscore

      case collect_symbol:

        switch (ch_class) {
          case other:
          case sChar:   break;

          case cr:      accept_char();
          default:      tok->code = StringToken; terminate(tok, source); state = begin_tok; return;
          }
        move_char(); continue;


// Collect String or character literal with hardly any discrimination between
// the two

      case collect_string:

        switch(ch_class) {
          case bslsh:   state = got_backslash; accept_char(); add_to_line(); continue;

          case quote:   accept_char(); add_to_line();
          case cr   :   state = got_quote; continue;

          case eofch:   tok->code = StringToken; goto fin_tok;

          default   :   val++; break;
          }
        move_char(); continue;


// Look for second quote as a method or allowing a quote to be in the String

      case got_quote:
        switch (ch_class) {
          case quote  : state = collect_string; break;
          default     : tok->code = StringToken;
fin_tok:                state = begin_tok; terminate(tok, source); return;
          }
        move_char(); continue;


      case got_backslash:
        switch (ch_class) {
          case cr:
          case eol:     state = collect_string; continue;
          default:      state = collect_string; val++;
          }
        move_char(); continue;


      case got_eol:

        switch (ch_class) {

          case cr:      accept_char(); continue;

          case eol:     if (print_source_line) {terminate(tok, source); display_source_line(tok);}
                        goto fin_eol;

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
          case cr: case eol:  case sChar: case quote: case comma: case eofch:
                   tok->code = IllegalToken; goto fin_tok;

          default: break;
          }
        move_char(); continue;



// This should never happen, but ...

      default:          state = begin_tok; continue;
      };
    }
  }


// Globals

static int error_count;     /* number of syntax errors */


// Functions

void Csv::error(Token* tok, Tchar* stg) {
String f;
String name = tok->name;

  display_source_line(tok);   error_count++;

  if (tok->code == EolToken) name = _T("<eol>");

  f.format(_T("       Token #%i: \"%s\" -> Error %i: %s\n"), tok->tokenNo, name.str(), error_count, stg);
  notePad << f << nCrlf;
  }


// display source line associated with current tok.
// Return offset of current tok

void Csv::display_source_line(Token* tok) {
String* pstg   = tok->psource;
String  f;

  pstg->trim();

  if (pstg->empty()) {notePad << f << nCrlf;}

  else {f.format(_T("%3i: %s\n"), tok->line_number, pstg->str()); notePad << f << nCrlf;}
  }




