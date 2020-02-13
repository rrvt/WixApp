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


class Token {
public:

TokenCode code;                         // code of current token
String    name;

union {
  long    integer;                      // integer value when type = Integer_Lit
  double  real;                         // real value    when type = Real_Lit
  } value;                              // value of literal, depends on type

int noDigits;

String*  psource;                       // pointer to source line holding token
int      line_number;                   // line number of source line
int      tokenNo;                       // Number of token from left margin

  Token() : code(NoToken), noDigits(0), line_number(0), tokenNo(0), psource(0) {value.real = 0;}

  void initialize()
    {code = NoToken; name.clear(); value.integer = 0; noDigits = 0; line_number = 0;}
  };
