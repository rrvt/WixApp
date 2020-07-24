// Lexical Analyzer
// rrvt 9/1/94
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Archive.h"
#include "NotePad.h"
#include "Token.h"
#include <float.h>
#include <math.h>


class Csv {

typedef enum {
  nil, begin_tok, collect_symbol, collect_string, got_backslash, got_quote, beginHex,
  got_eol, end_of_file, illegal_tok
  } States;

Archive&  ar;

CFileException fileError;

String    source_line;
String    source_line1;
String*   source[2];              // = {source_line, source_line1};
int       current_source;         // Index of current source line
String*   pline;                  // Pointer to end of source line String
int       pos;                    // Initial offset to convert tabs to spc

String*   ptok;                   // Pointer to end of word String

unsigned  long val;               // Collect integer part of number
unsigned  long last_val;          // Used to check for overflow
long      no_digits;              // Number of digits seen
long      frac;                   // collect fraction part of number
long      divisor;                // collect divisor (10^n, n = 0..9)
int       exponent;               // collect exponent value
bool      neg_exponent;           // negative exponent seen when true
double    ten_exp;                // value of 10^exponent

States    state;                  // Current state

bool      startHex;               // Start Hex Integer when true

Tchar     ch;                     // Current character
bool      getNext;
Tchar     quote_ch;               // Current quote character

bool      print_source_line;      // print source line when true
int       line_number;            // current line number
int       tokenNo;               // Number of token from left margin

public:

Token* token;                   // token returned    static const
Token* token1;                  // Next token in input stream

  Csv(Archive& arch);
 ~Csv() {NewAlloc(Token);  FreeNode(token);  FreeNode(token1); token = token1 = 0;}

  bool        initialize();                       // Initialize input mechanism

  void        set_print_flag(bool flag);         // Set print flag, true will cause
  bool        get_print_flag(void);              // source line to be printed

  // Set flag so that the third token will be processed as a hex number if it is one.

  void        set_collect_hex() {state = beginHex; startHex = true;}

  // get next token, values set in globals

  TokenCode   get_token(void);

  void        accept_token(void);                // Set first token to NO_Token
  void        accept_two_tokens(void);           // Set both tokens to No_Token

  void        error(Token* token, Tchar* stg);    // Error processing

private:

// Local Functions

  void next_tok(Token* tok);

  void start_token(Token* tok)   {ptok = &tok->name; ptok->clear(); tokenNo++;}

  void nextChar()      {if (getNext) {if (!ar.read(ch)) ch = 26;}   getNext = false;}

  void accept_char()   {getNext = true;}

  void add_to_line()   {*pline += ch;}

  void move_char()     {getNext = true; *ptok += ch; *pline += ch;}


  void terminate(Token* tok, String* source[]) {

    tok->line_number = line_number; tok->tokenNo = tokenNo; tok->psource = source[current_source];

    if (tok->code == EolToken) {
      if (print_source_line) display_source_line(tok);
      current_source = (current_source + 1) % 2;   line_number++;   tokenNo = 0;
      pline = source[current_source]; pline->clear();
      }
    }

  // display source line associated with current token.

  void display_source_line(Token* token);
  };


