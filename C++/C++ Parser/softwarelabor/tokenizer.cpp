#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "tokenizer.h"


string showTok(Token_t t) {
  switch(t) {
  case EOS:   return "EOS";
  case ZERO:  return "ZERO";
  case ONE:   return "ONE";
  case TWO:   return "TWO";
  case THREE: return "THREE";
  case FOUR:  return "FOUR";
  case FIVE:  return "FIVE";
  case SIX:   return "SIX";
  case SEVEN: return "SEVEN";
  case EIGHT: return "EIGHT";
  case NINE:  return "NINE";
  case OPEN:  return "OPEN";
  case CLOSE: return "CLOSE";
  case PLUS:  return "PLUS";
  case MULT:  return "MULT";
  }
  // NOTE: The (clang) compiler is able to figure out that
  // along all control-flow paths, a return statement will be reached.
    return "";
}

Token_t Tokenize::next() {
    if(s.length() <= pos)
      return EOS;

    while(1) {

       if(s.length() <= pos)
         return EOS;

       switch(s[pos]) {
       case '0': pos++;
                 return ZERO;
       case '1': pos++;
                 return ONE;
       case '2': pos++;
                 return TWO;
        case '3': pos++;
                return THREE;
        case '4': pos++;
                return FOUR;
        case '5': pos++;
                 return FIVE;
        case '6': pos++;
                 return SIX;
        case '7': pos++;
                 return SEVEN;
        case '8': pos++;
                 return EIGHT;
        case '9': pos++;
                 return NINE;
       case '(': pos++;
                 return OPEN;
       case ')': pos++;
                 return CLOSE;
       case '+': pos++;
                 return PLUS;
       case '*': pos++;
                 return MULT;
       default:  // we simply skip all other symbols !
                 pos++;
             break;
       }
    }
} // next


vector<Token_t> Tokenize::scan() {
    vector<Token_t> v;
    Token_t t;

    do {
      t = next();
      v.push_back(t);
    }
    while(t != EOS);

    return v;
} // scan


string Tokenize::show() {
    vector<Token_t> v = this->scan();
    string s;

    for(unsigned int i=0; i < v.size(); i++) {
      s += showTok(v[i]);
      if(i+1 < v.size())
    s += ";" ;         // Add delimiter
    }
    return s;
} // show
