#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "scanner.h"
using namespace std;


 void bioutput(int n) {
 	unsigned char c = n >> 24;
   cout << c;
   c = n >> 16;
   cout << c;
   c = n >> 8;
   cout << c;
   c = n;
   cout << c;
 }


int readlable(vector<Token> &tokenLine, map<string, int> &table, int address){
  string name = tokenLine[0].getLexeme();
  if(table.count(name) == 0){
    table[name] = address;
    name = name.substr(0, name.size() - 1);
    cerr << name << " " << address << endl;
  }else{
    tokenLine.clear();
    throw ScanningFailure("ERROR");
  }
  tokenLine.erase(tokenLine.begin());
  return 0;
}


int readword(vector<Token> &tokenLine, map<string, int> &table){
  if(tokenLine.size() != 2){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
  }
  if(tokenLine[1].getKind() == Token::INT){
    if(tokenLine[1].toNumber() <= 4294967295 && tokenLine[1].toNumber() >= -2147483648){
      int64_t t_1 = tokenLine[1].toNumber();
      bioutput(t_1);
    }else{
      tokenLine.clear();
      throw ScanningFailure("ERROR");
    }
  }
  else if(tokenLine[1].getKind() == Token::HEXINT){
    if(tokenLine[1].toNumber() <= 0xffffffff && tokenLine[1].toNumber() >= 0){
      int64_t t_2 = tokenLine[1].toNumber();
      bioutput(t_2);
    }else{
      tokenLine.clear();
      throw ScanningFailure("ERROR");
    }
  }
  else if(tokenLine[1].getKind() == Token::ID){
    string name = tokenLine[1].getLexeme();
    name.push_back(':');
    if(table.count(name) > 0){
      int64_t t_3 = table[name];
      bioutput(t_3);
    }else{
      tokenLine.clear();
      throw ScanningFailure("ERROR");
    }
  }
  tokenLine.clear();
  return 0;
}

int readjr(vector<Token> &tokenLine){
  if(tokenLine.size() != 2){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
  }else{
    if(tokenLine[1].getKind() != Token::REG){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
    }else{
      int64_t reg = tokenLine[1].toNumber();
      int64_t num = 0;
      if(reg > 31 || reg < 0){
          tokenLine.clear();
          throw ScanningFailure("ERROR");
        }else{
          if(tokenLine[0].getLexeme() == "jr"){
             num = (0 << 26) | (reg << 21) | (0 << 16) | (0 << 11) | (0 << 6) | 8;
          }else{
             num = (0 << 26) | (reg << 21) | (0 << 16) | (0 << 11) | (0 << 6) | 9;
          }
          bioutput(num);
        }
    }
  }
  tokenLine.clear();
  return 0;
}

int readsix(vector<Token> &tokenLine){
  if(tokenLine.size() != 6){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
  }else{
    if(tokenLine[1].getKind() != Token::REG 
    || tokenLine[2].getKind() != Token::COMMA 
    || tokenLine[3].getKind() != Token::REG
    || tokenLine[4].getKind() != Token::COMMA   
    || tokenLine[5].getKind() != Token::REG ){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
    }else{
      int64_t reg1 = tokenLine[1].toNumber();
      int64_t reg2 = tokenLine[3].toNumber();
      int64_t reg3 = tokenLine[5].toNumber();
      int64_t num;
      if(reg1 > 31 || reg1 < 0 || reg2 > 31 || reg2 < 0 || reg3 > 31 || reg3 < 0){
          tokenLine.clear();
          throw ScanningFailure("ERROR2");
        }else{
          if(tokenLine[0].getLexeme() == "add"){
            num = (0 << 26) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | 32;
          }else if(tokenLine[0].getLexeme() == "sub"){
            num = (0 << 26) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | 34;
          }else if(tokenLine[0].getLexeme() == "slt"){
            num = (0 << 26) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | 42;
          }else if(tokenLine[0].getLexeme() == "sltu"){
            num = (0 << 26) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | 43;
          }
          bioutput(num);
        }
    }
  }
  tokenLine.clear();
  return 0;
}

int readbe(vector<Token> &tokenLine, map<string, int> &table, int address){
  if(tokenLine.size() != 6){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
  }else{
    if(tokenLine[1].getKind() != Token::REG 
    || tokenLine[2].getKind() != Token::COMMA 
    || tokenLine[3].getKind() != Token::REG
    || tokenLine[4].getKind() != Token::COMMA   
    || (tokenLine[5].getKind() != Token::INT 
        && tokenLine[5].getKind() != Token::HEXINT
        && tokenLine[5].getKind() != Token::ID)){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
    }else{
      int64_t reg1 = tokenLine[1].toNumber();
      int64_t reg2 = tokenLine[3].toNumber();
      if(reg1 > 31 || reg1 < 0 || reg2 > 31 || reg2 < 0){
          tokenLine.clear();
          throw ScanningFailure("ERROR");
       }else if(tokenLine[5].getKind() == Token::INT){
        int64_t num1 = tokenLine[5].toNumber();
        if(!(num1 >= -32768 && num1 <= 32767)){
          tokenLine.clear();
          throw ScanningFailure("ERROR");
        }else{
          if(tokenLine[0].getLexeme() == "beq"){
            int64_t reg = (4 << 26) | (reg1 << 21) | (reg2 << 16) | (num1 & 0xFFFF);
            bioutput(reg);
          }else if(tokenLine[0].getLexeme() == "bne"){
             int64_t reg = (5 << 26) | (reg1 << 21) | (reg2 << 16) | (num1 & 0xFFFF);
             bioutput(reg);
          }
        }
      }else if(tokenLine[5].getKind() == Token::HEXINT){
        int64_t num2 = tokenLine[5].toNumber();
        if(!(num2 <= 0xFFFF)){
          tokenLine.clear();
          throw ScanningFailure("ERROR");
        }else{
          if(tokenLine[0].getLexeme() == "beq"){
            int64_t reg = (4 << 26) | (reg1 << 21) | (reg2 << 16) | (num2 & 0xFFFF);
            bioutput(reg);
          }else if(tokenLine[0].getLexeme() == "bne"){
            int64_t reg = (5 << 26) | (reg1 << 21) | (reg2 << 16) | (num2 & 0xFFFF);
            bioutput(reg);
          }
        }
      }else if(tokenLine[5].getKind() == Token::ID){
          string belable = tokenLine[5].getLexeme();
          belable.push_back(':');
          if(table.count(belable) == 0){
            tokenLine.clear();
            throw ScanningFailure("ERROR");
        }else{
          int64_t lableval = table[belable];
          int64_t i = (lableval - address)/ 4;
          if((i >= -32768) && (i <= 32767)){
            int64_t num3 = i;
            if(tokenLine[0].getLexeme() == "beq"){
            int64_t reg = (4 << 26) | (reg1 << 21) | (reg2 << 16) | (num3 & 0xFFFF);
            bioutput(reg);
            }else if (tokenLine[0].getLexeme() == "bne"){
            int64_t reg = (5 << 26) | (reg1 << 21) | (reg2 << 16) | (num3 & 0xFFFF);
            bioutput(reg);
            }
          }else{
            tokenLine.clear();
            throw ScanningFailure("ERROR");
          }
        }
      }
    }
  }
  tokenLine.clear();
  return 0;
}


int readtwo(vector<Token> &tokenLine){
  if(tokenLine.size() != 2){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
  }else{
    if(tokenLine[1].getKind() != Token::REG){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
    }else{
      int64_t reg = tokenLine[1].toNumber();
      int64_t num = 0;
      if(reg > 31 || reg < 0){
          tokenLine.clear();
          throw ScanningFailure("ERROR");
        }else{
          if(tokenLine[0].getLexeme() == "mfhi"){
             num = (0 << 26) | (0 << 21) | (0 << 16) | (reg << 11) | (0 << 6) | 16;
          }else if(tokenLine[0].getLexeme() == "mflo"){
             num = (0 << 26) | (0 << 21) | (0 << 16) | (reg << 11) | (0 << 6) | 18;
          }else{
             num = (0 << 26) | (0 << 21) | (0 << 16) | (reg << 11) | (0 << 6) | 20;
          }
          bioutput(num);
        }
    }
  }
  tokenLine.clear();
  return 0;
}

int readfour(vector<Token> &tokenLine){
  if(tokenLine.size() != 4){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
  }else{
    if(tokenLine[1].getKind() != Token::REG 
    || tokenLine[2].getKind() != Token::COMMA 
    || tokenLine[3].getKind() != Token::REG){
    tokenLine.clear();
    throw ScanningFailure("ERROR");
    }else{
      int64_t reg1 = tokenLine[1].toNumber();
      int64_t reg2 = tokenLine[3].toNumber();
      int64_t num;
      if(reg1 > 31 || reg1 < 0 || reg2 > 31 || reg2 < 0 ){
          tokenLine.clear();
          throw ScanningFailure("ERROR");
        }else{
          if(tokenLine[0].getLexeme() == "mult"){
            num = (0 << 26) | (reg1 << 21) | (reg2 << 16) | (0 << 11) | 24;
          }else if(tokenLine[0].getLexeme() == "multu"){
            num = (0 << 26) | (reg1 << 21) | (reg2 << 16) | (0 << 11) | 25;
          }else if(tokenLine[0].getLexeme() == "div"){
            num = (0 << 26) | (reg1 << 21) | (reg2 << 16) | (0 << 11) | 26;
          }else if(tokenLine[0].getLexeme() == "divu"){
            num = (0 << 26) | (reg1 << 21) | (reg2 << 16) | (0 << 11) | 27;
          }
          bioutput(num);
        }
    }
  }
  tokenLine.clear();
  return 0;
}

int readseven(vector<Token> &tokenLine){
  if(tokenLine.size() != 7){
    tokenLine.clear();
    throw ScanningFailure("ERROR1");
  }else{
    if(tokenLine[1].getKind() != Token::REG
    || tokenLine[2].getKind() != Token::COMMA  
    || (tokenLine[3].getKind() != Token::INT && tokenLine[3].getKind() != Token::HEXINT)
    || tokenLine[4].getKind() != Token::LPAREN
    || tokenLine[5].getKind() != Token::REG 
    || tokenLine[6].getKind() != Token::RPAREN){
    tokenLine.clear();
    throw ScanningFailure("ERROR2");
    }else{
      int64_t reg1 = tokenLine[1].toNumber();
      int64_t reg2 = tokenLine[5].toNumber();
      int64_t num = tokenLine[3].toNumber();
      if(reg1 > 31 || reg1 < 0 || reg2 > 31 || reg2 < 0){
          tokenLine.clear();
          throw ScanningFailure("ERROR3");
        }else if(tokenLine[3].getKind() == Token::INT){
          if((num < -32768) || (num > 32767)){
            tokenLine.clear();
            throw ScanningFailure("ERROR4");
          }else{
            if(tokenLine[0].getLexeme() == "sw"){
              int64_t reg = (43 << 26) | (reg2 << 21) | (reg1 << 16) | (num & 0xFFFF);
              bioutput(reg);
            }else{
              int64_t reg = (35 << 26) | (reg2 << 21) | (reg1 << 16) | (num & 0xFFFF);
              bioutput(reg);
            }
          }
        }else if(tokenLine[3].getKind() == Token::HEXINT){
          if((num < 0) || (num > 0xffffffffffffffff)){
            tokenLine.clear();
            throw ScanningFailure("ERROR5");
          }else{
            if(tokenLine[0].getLexeme() == "sw"){
              int64_t reg = (43 << 26) | (reg2 << 21) | (reg1 << 16) | (num & 0xFFFF);
              bioutput(reg);
            }else{
              int64_t reg = (35 << 26) | (reg2 << 21) | (reg1 << 16) | (num & 0xFFFF);
              bioutput(reg);
            }
          }
        }
    }
  }
  tokenLine.clear();
  return 0;
}






int main(){
  string line;
  map<string, int> table;
  vector< vector<Token> > tokenvec;
  int address = 0;
  try{
    //loop1
    while(getline(cin, line)){
      vector<Token> tokenLine = scan(line);
      tokenvec.push_back(tokenLine);
      while(tokenLine.size() > 0){
        if(tokenLine[0].getKind() == Token::WORD){
          address += 4;
          break;
        }else if(tokenLine[0].getKind() == Token::LABEL){
          readlable(tokenLine, table, address);
        }else if(tokenLine[0].getKind() == Token::ID){
          address += 4;
          break;
        }else{
          throw ScanningFailure("ERROR");
        }
      }
    }
    address = 0;
    //loop2
    for(int i = 0; i < (int)tokenvec.size(); i++){
      vector<Token> token = tokenvec[i];
      while(token.size() > 0){
        if(token[0].getKind() == Token::WORD){
          readword(token,table);
          address += 4;
        }else if(token[0].getKind() == Token::LABEL){
            token.erase(token.begin());
        }else if(token[0].getKind() == Token::ID){
          address += 4;
          string op = token[0].getLexeme();
          if(op == "jr" || op =="jalr"){
            readjr(token);
          }else if(op == "add" || op == "sub" || op == "slt" || op == "sltu"){
            readsix(token);
          }else if(op == "beq" || op == "bne"){
            readbe(token, table, address);
          }else if(op == "mfhi" || op == "mflo" || op == "lis"){
            readtwo(token);
          }else if(op == "mult" || op == "multu" || op == "div" || op == "divu"){
            readfour(token);
          }else if(op == "lw" || op == "sw"){
            readseven(token);
          }else{
            throw ScanningFailure("ERROR");
          }
        }else {
          cerr << "wrong" << endl;
        }
      }
    }
  }catch (ScanningFailure &f) {
    cerr << f.what() << endl;
    return 1;
  }
  return 0;
}



