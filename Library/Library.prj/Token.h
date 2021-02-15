// Token for use with a lexical analyser


#pragma once


//  Token codes

enum TokenCode {NoToken,      WhiteToken,
                IdentToken,   IntLitToken,    RealLitToken,   HexToken,       CharToken,      StringToken,
                BangToken,    PluSToken,      DollarToken,    MinuSToken,     AsterickToken,  SlashToken,
                ModToken,     NotToken,       AndToken,       OrToken,        ColonEqualToken,
                LParenToken,  RParenToken,    LBracketToken,  RBracketToken,  LBraceToken,
                RBraceToken,  HatToken,       ColonToken,     SemicolonToken, CommaToken,     PeriodToken,
                AtSignToken,  LbSignToken,    LTToken,        GTToken,        EQToken,        NEToken,
                LEToken,      GEToken,        SqglToken,      ApostropheToken, DelChToken,
                CommentToken, EolToken,       EOFToken,       IllegalToken
                };


struct Token {

TokenCode code;                         // code of current token
String    name;

union {
  long    integer;                      // integer value when type = Integer_Lit
  double  real;                         // real value    when type = Real_Lit
  } value;                              // value of literal, depends on type

int       noDigits;

String*   psource;                      // pointer to source line holding token
int       line_number;                  // line number of source line
int       tokenNo;                      // Number of token from left margin

  Token() : code(NoToken), noDigits(0), line_number(0), tokenNo(0), psource(0) {value.real = 0;}

  void clear() {
    code = NoToken; name.clear(); value.real = 0; noDigits = 0;
    psource = 0; line_number = 0; tokenNo = 0;
    }

  Token& operator= (Token& tok) {
    code = tok.code; name = tok.name; value.real = tok.value.real; noDigits = tok.noDigits;
    psource = tok.psource; line_number = tok.line_number; tokenNo = tok.tokenNo;
    return *this;
    }
  };

