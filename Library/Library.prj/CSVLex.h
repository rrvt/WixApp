// Csv Lexical Analyzer

/*
The template for loading, storing and accessing CSV files is contained in several files:
  *  CSVLex   -- A Lexical Analyser (FSM) for reading CSV files
  *  CSVOut   -- Contains a class for outputing to a CSV file (managing commas and quotes withing fields)
  *  CSVRcdB  -- A base file for your CSV Record to provide basic functionality
  *  CSVRcdsT -- Contains a template for creating a container for CSV Records.  May be used as a base
                 class for extending functionality with a subclass.

The principal functions are get_token() and accept_token();  Get token returns the token code for token
and makes sure that both token and token1 are the next two tokens in the input stream.  Accept token
removes token from the input stream so that the next get token reads another token from the input stream.
Until accept token is called no changes in the two tokens are done.

Many simple decisions about the input stream may be made by just observing the next two tokens and they
are provided by CSVLex.
*/

#pragma once
#include "Archive.h"
#include "NotePad.h"


//  CSVtok codes

enum CSVtokCode {NoToken, WhiteToken, StringToken, CommaToken, EolToken, EOFToken, IllegalToken};


struct CSVtok {

CSVtokCode code;                            // code of current token
String     name;

String*    psource;                         // pointer to source line holding token
int        line_number;                     // line number of source line
int        tokenNo;                         // Number of token from left margin

  CSVtok() : code(NoToken), line_number(0), tokenNo(0), psource(0) { }

  void clear() {code = NoToken; name.clear(); psource = 0; line_number = 0; tokenNo = 0;}

  CSVtok& operator= (CSVtok& tok) {
    code = tok.code; name = tok.name;
    psource = tok.psource; line_number = tok.line_number; tokenNo = tok.tokenNo;  return *this;
    }

  friend class CSVLex;
  };




class CSVLex {
public:

CSVtok   token;                  // token returned    static const
CSVtok   token1;                 // Next token in input stream

private:

enum States {nil, begin_tok, collect_symbol, got_quote, collect_string, got_backslash, got_eol,
                                                                              end_of_file,  illegal_tok};
Archive& ar;
String   source_line;
String   source_line1;
String*  source[2];              // = {source_line, source_line1};
int      current_source;         // Index of current source line

String*  ptok;                   // ptr to current token's name, used during character input
String*  pline;                  // Ptr to the current source line, used during character input

States   state;                  // Current state

Tchar    ch;                     // Current character
bool     getNext;
Tchar    quote_ch;               // Current quote character

bool     print_source_line;      // print source line when true
int      line_number;            // current line number
int      tokenNo;                // Number of token from left margin

public:

  CSVLex(Archive& arch);
 ~CSVLex() { }

  bool        initialize();                       // Initialize input mechanism

  void        set_print_flag(bool flag);          // Set print flag, true will cause
  bool        get_print_flag();                   // source line to be printed

  CSVtokCode  get_token();                        // get next token, values set in globals

  void        accept_token();                     // Set first token to NO_Token
  void        accept_two_tokens();                // Set both tokens to No_Token

  void        error(CSVtok& token, Tchar* stg);   // Error processing

private:

  void next_tok(CSVtok& tok);

  void start_token(CSVtok& tok)   {ptok = &tok.name; ptok->clear(); tokenNo++;}

  void nextChar()      {if (getNext) {if (!ar.read(ch)) ch = 26;}   getNext = false;}

  void accept_char()   {getNext = true;}

  void add_to_line()   {*pline += ch;}

  void move_char()     {getNext = true; *ptok += ch; *pline += ch;}

  void terminate(CSVtok& tok, String* source[]);

  void display_source_line(CSVtok& token);         // display source line associated with current token.
  };

