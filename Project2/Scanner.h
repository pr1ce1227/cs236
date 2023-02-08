#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Token.h"
#include <cctype>
#include <vector>
using namespace std; 


class Scanner {
private:
	string input;
    vector<Token> tokens; 
    int lineCount; 
    

public:
	Scanner(const string& input) : input(input) {
        if (input.empty()) {
            lineCount = 1; 
        }
        else {
            lineCount = 1; 
        }
    }

    // Compares input to matching keyword, returns true or false, verify is keyword and check is current input
    bool checkKeyWord(string check, string verify) {

        // run until check is emptry or verification is finished
        while (!check.empty()) {
            
            // if both strings are empty then word was found 
            if (verify.empty() && check.empty())
                return true;

            // if verify is empty and check is followed by whitespace or colon then word is found 
            else if (verify.empty() && !check.empty()) {
                if (isspace(check.at(0)) || check.at(0) == ':')
                    return true;
                else 
                    return false; 
            }

           // If verify = check, Decrease each string by one to reset for the next loop
           else if (check.at(0) == verify.at(0)) {
                check = check.substr(1, check.length()); 
                verify = verify.substr(1, verify.length()); 
            } 

            // Words are not equal 
            else
                return false;
        }

        // if while loop finishs and verify is empty then the word was found
        if (verify.empty())
            return true;
        else
            return false; 
    }

    // Checks for matching ' in string returns location else returns -1 
    int checkString(string check) {
        if (check.find('\'') == string::npos) {
            return -1;
        }
        else
            return check.find('\''); 
    }

    vector<Token> scanToken() {
        Token eoff; 
        while (!input.empty()) {

            // Token is assigned by case that is triggered then push onto list
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

                // check for token :- else accept colon as token 
                input = input.substr(1, input.length());
                if (!input.empty() && input.at(0) == '-') {
                    t = { COLON_DASH, ":-", lineCount };
                }

                else {
                    t = { COLON, ":", lineCount };
                    input.insert(0, "a");
                }
                break;

            case('*'):
                t = { MULTIPLY, "*", lineCount };
                break;

            case('+'):
                t = { ADD, "+", lineCount };
                break;

            case('S'):

                // Check for key word Schemes else go to default
                if (checkKeyWord(input, "Schemes")) {
                    t = { SCHEMES, "Schemes", lineCount };
                    input = input.substr(6, input.length());
                }
                else {
                    goto Default;
                }
                break;

            case('F'):

                // Check for keyword "Facts" else go to default 
                if (checkKeyWord(input, "Facts")) {
                    t = { FACTS, "Facts", lineCount };
                    input = input.substr(4, input.length());
                }
                else {
                    goto Default;
                }
                break;

            case('R'):

                // Check for key word "Rules" else go to default 
                if (checkKeyWord(input, "Rules")) {
                    t = { RULES, "Rules", lineCount };
                    input = input.substr(4, input.length());
                }
                else {
                    goto Default;
                }
                break;

            case('Q'):

                // check for key word Queries else default 
                if (checkKeyWord(input, "Queries")) {
                    t = { QUERIES, "Queries", lineCount };
                    input = input.substr(6, input.length());
                }
                else {
                    goto Default;
                }
                break;

            case('\''):

                // check to for matching ', if not returns -1 
                index = checkString(input.substr(1));
                if (index == -1){
                    t = { UNDEFINED, input, lineCount };

                    // count and update number of lines 
                    while (!input.empty()) {
                        if (input.at(0) == '\n')
                            lineCount++; 
                        input = input.substr(1, input.length()); 
                    }

                    // Dummy variable to be removed when switch exits 
                    input = "a"; 
                }

                // update token and input 
                else {
                    t = { STRING, input.substr(0,index + 2), lineCount };
                    input = input.substr(index+1); 
                }
                    
                break; 

            case('#'): 

                // continue to add chars until new line character is hit, update input 
                while (!input.empty() && input.at(0) != '\n') {
                    string str(1, input.at(0)); 
                    name = name.append(str); 
                    input = input.substr(1, input.length()); 
                }

                // add dummy variable to be removed after switch, update token 
                input.insert(0,"a"); 
                t = { COMMENT, name, lineCount }; 
                break; 
            
            // if Enter into key word case and proven mismatch goto defualt case
            Default:
            default: 

                // if char is letter then creat ID token, 
                if (isalpha(input.at(0))) {
                    string id; 

                    // Continue to add chars until space or empty input 
                    while (!input.empty() && !isspace(input.at(0)) && (isalpha(input.at(0)) || isdigit(input.at(0)))) {
                        string letter(1, input.at(0)); 
                        id = id.append(letter);
                        input = input.substr(1, input.length()); 
                    }

                    // add dummy variable to be removed after switch, update token 
                    input.insert(0, "a"); 
                    t = { ID, id, lineCount }; 
                }

                // alL other symbols are undefined
                else {
                    string value(1, input.at(0));
                    t = { UNDEFINED, value, lineCount };
                }
                    break; 

            }

            // only push token onto list if not empty space, Skip comments
            if(!isspace(input.at(0)) && t.getType() != COMMENT)
                tokens.push_back(t); 

            // Update input string by removing 1 char
            input = input.substr(1, input.length());

        }

        // push eof onto back of list one input is empty 
        eoff = { eof, "", lineCount }; 
        tokens.push_back(eoff); 

        return tokens; 
    }
};