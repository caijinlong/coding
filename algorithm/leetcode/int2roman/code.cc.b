// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 16:55:10
// File  : code.cc
// Brief :

#include "base/public/logging.h"

const char * bit[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
// begin with I V
const char * ten[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
// begin with X L
const char * hundred[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
// beign with C D
const char * thourand[] = {"", "M", "MM", "MMM", "MMM"};
// beign with M

const char * * ptr[] = {bit, ten, hundred, thourand};

std::string Int2Roman(int v) {
  std::string rs = "";
  int i = 0;
  while (v) {
    if (v % 10 != 0) rs = ptr[i][v % 10] + rs;
    i++;
    v /= 10;
  }
  return rs;
}

// enum {BIT = 0, XTEN = 1, LTEN, CHUN = 2, DHUN, THO = 3, END = 4} state;

enum STATE {BIT = 0, TEN = 1, HUN = 2, THO = 3, END = 4} state;

void StateOfTHO(std::string & str, int & index) {
  if (index < str.size() && str[index] == 'M') {
    index++;
  } else {
    state = END;
  }
}

void StateOfHUN(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'C' || str[index] == 'D' ||  str[index] == 'M')) {
    index ++;
  } else {
    state = END;
  }
}

void StateOfTEN(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'X' || str[index] == 'L' ||  str[index] == 'C')) {
    index ++;
  } else {
    state = END;
  }
}

void StateOfBIT(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'I' || str[index] == 'V' ||  str[index] == 'X')) {
    index ++;
  } else {
    state = END;
  }
}

void Start(std::string & str) {
  if (str.size() == 0) state = END;
  else {
    switch(str[0]) {
      case 'I':
      case 'V':
        state = BIT;
        break;
      case 'X':
      case 'L':
        state = TEN;
        break;
      case 'C':
      case 'D':
        state = HUN;
        break;
      case 'M':
        state = THO;
        break;
      default:
        state = END;
    }
  }
}

bool Token(std::string & str, std::string * rs) {
  if (str.size() == 0) return false;
  int index = 0;
  Start(str);
  while (state != END) {
    switch(state) {
      case BIT:
        StateOfBIT(str, index);
        break;
      case TEN:
        StateOfTEN(str, index);
        break;
      case HUN:
        StateOfHUN(str, index);
        break;
      case THO:
        StateOfTHO(str, index);
        break;
      case END:
       break;
    }
  }
  CHECK(index >= 0);
  *rs = str.substr(0, index);
  str = str.substr(index);
  return true;
}
/*
void StateOfCHUN(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'C' || str[index] == 'D' ||  str[index] == 'M')) {
    index ++;
  } else {
    state = END;
  }
}

void StateOfDHUN(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'C' || str[index] == 'D')) {
    index ++;
  } else {
    state = END;
  }
}

void StateOfXTEN(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'X' || str[index] == 'L' ||  str[index] == 'C')) {
    index ++;
  } else {
    state = END;
  }
}

void StateOfLTEN(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'L' || str[index] == 'X')) {
    index ++;
  } else {
    state = END;
  }
}
*/

int main(int argc, char** argv) {
  LOG(INFO) << Int2Roman(333);
  std::string str = "CCCIII";
  std::string foo = "";
  while (Token(str, &foo)) {
    LOG(INFO) << foo;
    // LOG(INFO) << str;
  }
  return 0;
}