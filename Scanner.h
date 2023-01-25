#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Token.h"
#include <cctype>
#include <list>
using namespace std; 


class Scanner {
private:
	string input;
    list<Token> tokens; 
    int lineCount; 
    

public:
	Scanner(const string& input, int lineCount) : input(input), lineCount(lineCount) { }

    bool checkKeyWord(string check, string verify) {
        while (!check.empty()) {
            if (verify.empty()) 
                return true; 
           else if (check.at(0) == verify.at(0)) {
                check = check.substr(1, check.length()); 
                verify = verify.substr(1, verify.length()); 
            }   
            else
                return false;
        }
        if (verify.empty())
            return true;
        else
            return false; 
    }

    bool checkID(string check) {
        if (isalpha(check.at(0))) {
            return true; 
        }
        else
            return false; 
    }

    int checkString(string check) {
        if (check.find('\'') == string::npos) {
            return -1;
        }
        else
            return check.find('\''); 
    }

    list<Token> scanToken() {
        Token eoff; 
        while (!input.empty()) {
            Token t; 
            int index = 0; 
            string name = "";

            switch (input.at(0)) {

            case('\n'):
                    lineCount++;
                    break; 
            case(','): 
                    t = { COMMA, ",", lineCount };
                    break; 

            case('.'):
                    t = { PERIOD, ".", lineCount };
                    break; 

            case('?'):
                    t = { Q_MARK, "?", lineCount };
                    break; 

            case('('):
                    t = { LEFT_PAREN, "(", lineCount };
                    break; 

            case(')'):
                    t = { RIGHT_PAREN, ")", lineCount };
                    break; 

            case(':'):
                if (checkKeyWord(input, ":-")) {
                     t = { COLON_DASH, ":-", lineCount };
                     input = input.substr(1, input.length()); 
                    }
                else {
                    t = { COLON, ":", lineCount };
                }
                    break; 

            case('*'):
                    t = { MULTIPLY, "*", lineCount };
                    break; 

            case('+'):
                    t = { ADD, "+", lineCount };
                    break; 

            case('S'): 
                if (checkKeyWord(input, "Schemes")) {
                    t = {SCHEMES, "Schemes", lineCount }; 
                    input = input.substr(6, input.length()); 
                }
                else {
                    goto Default; 
                }
                break; 

            case('F'):
                if (checkKeyWord(input, "Facts")) {
                    t = { SCHEMES, "Facts", lineCount };
                    input = input.substr(4, input.length());
                }
                else {
                    goto Default; 
                }
                break;

            case('R'):
                if (checkKeyWord(input, "Rules")) {
                    t = { SCHEMES, "Rules", lineCount };
                    input = input.substr(4, input.length());
                }
                else {
                    goto Default; 
                }
                break;

            case('Q'):
                if (checkKeyWord(input, "Queries")) {
                    t = { SCHEMES, "Schemes", lineCount };
                    input = input.substr(6, input.length());
                }
                else {
                    goto Default; 
                }
                break;

            case('\''): 
                index = checkString(input.substr(1)); 
                if (index == -1)
                    goto Default; 
                else {
                    t = { STRING, input.substr(0,index + 2), lineCount };
                    input = input.substr(index+1); 
                }
                    
                break; 

            case('#'): 
                while (!input.empty() && input.at(0) != '\n') {
                    string str(1, input.at(0)); 
                    name = name.append(str); 
                    input = input.substr(1, input.length()); 
                }
                input.insert(0,"a"); 
                t = { COMMA, name, lineCount }; 
                break; 
            
            Default:
            default: 
                if (isalpha(input.at(0))) {
                    string id; 
                    while (!input.empty() && !isspace(input.at(0)) && isalpha(input.at(0))) {
                        string letter(1, input.at(0)); 
                        id = id.append(letter);
                        input = input.substr(1, input.length()); 
                    }
                    input.insert(0, "a"); 
                    t = { ID, id, lineCount }; 
                }
                else {
                    string value(1, input.at(0));
                    t = { UNDEFINED, value, lineCount };
                }
                    break; 

            }
            if(!isspace(input.at(0)))
                tokens.push_back(t); 
            input = input.substr(1, input.length());

        }
        eoff = { eof, "EOF", lineCount }; 
        tokens.push_back(eoff); 
        return tokens; 
    }
};